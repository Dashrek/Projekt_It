#include "menu.hpp"
#define add_event Strona_glowna->buttons[Strona_glowna->getKlucz()-1]->checkevent
#define Inny *Strona_glowna->buttons[Strona_glowna->getKlucz()-1]
#define poprzedni {Strona_glowna->getKlucz()-1,Strona_glowna->buttons[Strona_glowna->getKlucz()-1].get()}
#define add_time_event Strona_glowna->buttons[Strona_glowna->getKlucz()-1]->checkTimeEvent
#define ostatni_przycisk(at) Strona_glowna->buttons[Strona_glowna->getKlucz()-1-at]
string replaceAll(string str, const std::string& from, const std::string& to) {
    if(from.empty()) return str; // Zabezpieczenie przed pustym ciągiem
    size_t start_pos = 0;
    string dar=str;
    // Znajdź i zamień wszystkie wystąpienia
    while((start_pos = dar.find(from, start_pos)) != std::string::npos) {
        dar.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Przeskocz do końca nowo wstawionego fragmentu
    }
    return dar;
}
moved::moved(bool dir, int xx, int yy, int id) : direction(dir), x(xx), y(yy), move_id(id){};
string moved::s_str() {
    return to_string(move_id)+"|"+(direction?"r":"l")+"|"+ to_string(x)+"|"+to_string(y)+";";
}
size_t utf8_len(const std::string& s)
{
    size_t count = 0;
    for (unsigned char c : s)
        if ((c & 0xC0) != 0x80)
            count++;
    return count;
}
std::string floatToString(float value, int precision = 1) {
    std::ostringstream out;
    // fixed wymusza stałą liczbę miejsc po przecinku
    // setprecision określa ile ma ich być
    out << std::fixed << std::setprecision(precision) << value;
    return out.str();
}
Game::Game() : h_i(0), w_i(0), actual_word(""),no_validate(false),move_count(0){
}
Game::~Game(){
    if(h_i>=2) {
        for (int i = 0; i < h_i; i++) {
            tablica[i].clear();
            if (i < h_i - 1){
                przyciski[i].clear();
            }
        }
        tablica.clear();
        przyciski.clear();
        moves.clear();
    }
}
void Game::flush(){
    if(h_i>=2) {
        for (int i = 0; i < h_i; i++) {
            tablica[i].clear();
            if (i < h_i - 1){
                przyciski[i].clear();
            }
        }
        tablica.clear();
        przyciski.clear();
        moves.clear();
    }
    h_i=0;
    w_i=0;
    actual_word="";
    no_validate=false;
    move_count=0;

}
void Game::Start(Page* Strona_glowna, ButtonFactory *Baza, string word,string to_word, int length, int steps,bool timer, string timek){
    Strona_glowna->makeEmpty();
    move_count=0;
    int k= utf8_len(word);//długość słowa
    int d=k%length;
    w_i=length;
    h_i=k/length+(d>0 ? 1 : 0);
    //std::cout << "DEBUG: k=" << k << " length=" << length << " word=" << word << std::endl;
    word.resize(word.length()+(d>0 ? length-d:0), ' ');
    float h,w;
    w=100.0f/2.0f;
    h=100.0f*8.0f/9.0f;
    float t_s=0.4;
    float w_a=(w-(w_i+w_i-1)*2*t_s)/(w_i+(w_i-1)/3.0f);
    float h_a=(h-(h_i+h_i-1)*2*t_s)/(h_i+(h_i-1)/3.0f);
    tablica.assign(h_i, vector<AtomData>(w_i, {0,nullptr}));
    przyciski.assign((h_i-1)*2, vector<AtomData>(w_i-1, {0,nullptr}));
    int f=0;
    int g=0;
    for (size_t i = 0; i < word.size(); i = next_utf8((to_word=="" ? word : to_word), i))
    {
        size_t next = next_utf8(word, i);
        string znak = word.substr(i, next - i);
        if(f==0 && g==0) {
            Strona_glowna->addElement<Atom>(*Baza, "Orange", vector<string>{"position-x:"+floatToString(((100.0f-w)/2.0f+t_s+w_a/2.0f),1)+"vw",
                                                                            "position-y:"+ floatToString((100.0f-h)/2.0f+t_s+h_a/2.0f,1)+"vh",
                                                                            "font-size:"+floatToString(h_a/3.0)+"vh",
                                                                            "font-name:./fonts/orbitron-black.ttf",
                                                                            "font:#7a2160",
                                                                            "font-shadow:#290b20",
                                                                            "font-maxwidth:"+ floatToString(w_a-1,1)+"vw"},
                                            vector<string>{"width:"+floatToString(w_a,1)+"vw", "min-width:"+floatToString(w_a,1)+"vw", "max-width:"+floatToString(w_a,1)+"vw",
                                                           "height:"+ floatToString(h_a,1)+"vh", "min-height:"+ floatToString(h_a,1)+"vh", "max-height:"+ floatToString(h_a,1)+"vh",
                                                           "border-radius:"+ floatToString(t_s, 1)+"vw", "border-thickness:"+ floatToString(t_s,1)+"vw",
                                                           "shadow-offset-x:"+ floatToString(t_s,1)+"vw", "shadow-offset-y:"+ floatToString(t_s,1)+"vh"},
                                            vector<ALLEGRO_COLOR>{f_HTML("#f2bf41"), f_HTML("#7a6021"),
                                                                  f_HTML("#000000")}, znak);
        }else{
            Strona_glowna->addButton(*Baza,Inny,znak, floatToString(((100-w)/2+t_s+w_a/2.0f)+((float)f)*(w_a+w_a/3.0f+4*t_s))+"vw", floatToString(((100.0f-h)/2.0f+t_s+h_a/2.0f)+((float)g)*(h_a+h_a/3.0f+4*t_s))+"vh");
        }
        tablica[g][f]=poprzedni;
        f++;
        if(f==length){
            g++;
            f=0;
        }
    }
    for(g=0; g<h_i-1;g++){
        for(f=0;f<length-1;f++){
            if(f==0 && g==0){
                        Strona_glowna->addElement<TriangleButton>(*Baza,"Srebro",vector<string>{"position-x:"+ floatToString((100-w)/2+w_a+w_a/6+2*t_s)+"vw",
                                                                                        "position-y:"+ floatToString((100-h)/2+h_a+h_a/6+3*t_s)+"vh",
                                                                                        "font-size:"+floatToString(h_a/9.0)+"vh",
                                                                                        "font-maxwidth:"+floatToString((w_a)/3-0.5,1)+"vw",
                                                                                        "font-name:./fonts/orbitron-black.ttf",
                                                                                        "font:#FEBD27FF",
                                                                                        "font-shadow:#00000050"},vector<string>{"width:"+
                                                                                                                                floatToString(w_a/3-0.2)+"vw",
                                                                                                                                "min-width:"+floatToString(w_a/3-0.2)+"vw",
                                                                                                                                "max-width:"+floatToString(w_a/3-0.2)+"vw",
                                                                                                                                "height:"+floatToString(h_a/3-0.2)+"vh",
                                                                                                                                "min-height:"+floatToString(h_a/3-0.2)+"vh",
                                                                                                                                "max-height:"+floatToString(h_a/3-0.2)+"vh",
                                                                                                                                "border-radius:"+ floatToString(t_s, 1)+"vw", "border-thickness:"+ floatToString(t_s,1)+"vw",
                                                                                                                                "shadow-offset-x:"+ floatToString(t_s,1)+"vw", "shadow-offset-y:"+ floatToString(t_s,1)+"vh"}, vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                                                                                              f_HTML("#000000"),
                                                                                                                                                                              f_HTML("#FEF177"),
                                                                                                                                                                              f_HTML("#F25420"),
                                                                                                                                                                              f_HTML("#000000")},"ä:U");
                przyciski[2*g][f]=poprzedni;
                Strona_glowna->addElement<TriangleButton>(*Baza,"Brąz",vector<string>{"position-x:"+ floatToString((100.0f-w)/2.0f+w_a+w_a/6.0f+t_s*2.0f+0.4)+"vw",
                                                                                        "position-y:"+ floatToString((100.0f-h)/2.0f+h_a+h_a/6.0f+t_s*2.0f+0.4)+"vh",
                                                                                        "font-size:"+ floatToString(h_a/9.0)+"vh",
                                                                                        "font-maxwidth:"+floatToString((w_a)/3-0.5,1)+"vw",
                                                                                        "font-name:./fonts/orbitron-black.ttf",
                                                                                        "font:#FEBD27FF",
                                                                                        "font-shadow:#00000050"},vector<string>{"width:"+
                                                                                                                                floatToString(w_a/3-0.2)+"vw",
                                                                                                                                "min-width:"+floatToString(w_a/3-0.2)+"vw",
                                                                                                                                "max-width:"+floatToString(w_a/3-0.2)+"vw",
                                                                                                                                "height:"+floatToString(h_a/3-0.2)+"vh",
                                                                                                                                "min-height:"+floatToString(h_a/3-0.2)+"vh",
                                                                                                                                "max-height:"+floatToString(h_a/3-0.2)+"vh",
                                                                                                                                "border-radius:"+ floatToString(t_s, 1)+"vw", "border-thickness:"+ floatToString(t_s,1)+"vw",
                                                                                                                                "shadow-offset-x:"+ floatToString(t_s,1)+"vw", "shadow-offset-y:"+ floatToString(t_s,1)+"vh"}, vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                                                                                                                                                                     f_HTML("#000000"),
                                                                                                                                                                                                                                                     f_HTML("#FEF177"),
                                                                                                                                                                                                                                                     f_HTML("#F25420"),
                                                                                                                                                                                                                                                     f_HTML("#000000")},"ö:D");
                przyciski[2*g+1][f]=poprzedni;
            }else{
                Strona_glowna->addButton(*Baza,*(przyciski[0][0].pointer),"ä", floatToString(((100.0f-w)/2+w_a+w_a/6.0f+3.0f*t_s)+((float)f)*(w_a+w_a/3.0f+4.0f*t_s))+"vw", floatToString(((100.0f-h)/2.0f+h_a+h_a/6.0f+3.0f*t_s)+g*(h_a+h_a/3.0f+4.0f*t_s))+"vh");
                przyciski[2*g][f]=poprzedni;
                Strona_glowna->addButton(*Baza,*(przyciski[1][0].pointer),"ö", floatToString(((100.0f-w)/2+w_a+w_a/6.0f+3.0f*t_s+0.4)+((float)f)*(w_a+w_a/3.0f+4.0f*t_s))+"vw", floatToString(((100.0f-h)/2.0f+h_a+h_a/6.0f+3.0f*t_s+0.4)+g*(h_a+h_a/3.0f+4.0f*t_s))+"vh");
                przyciski[2*g+1][f]=poprzedni;
            }
            przyciski[2*g][f].pointer->checkevent=[Strona_glowna,Baza,this,g,f]{
                string t1,t2,t3,t4;
                t1=this->tablica[g][f].pointer->name;
                t2=this->tablica[g][f+1].pointer->name;
                t3=this->tablica[g+1][f+1].pointer->name;
                t4=this->tablica[g+1][f].pointer->name;
                this->tablica[g][f].pointer->name=t4;
                this->tablica[g][f+1].pointer->name=t1;
                this->tablica[g+1][f+1].pointer->name=t2;
                this->tablica[g+1][f].pointer->name=t3;
                Strona_glowna->addActive(this->tablica[g][f].id);
                Strona_glowna->addActive(this->tablica[g][f+1].id);
                Strona_glowna->addActive(this->tablica[g+1][f+1].id);
                Strona_glowna->addActive(this->tablica[g+1][f].id);
                Strona_glowna->addActive(this->przyciski[2*g][f].id);
                Strona_glowna->addActive(this->przyciski[2*g+1][f].id);
                if(!no_validate){
                    this->moves.push_back(moved(true,f,g,this->move_count++));
                    if (this->validate()){
                        for (int ii=0;ii<this->move_count;ii++) {
                            cout << moves[ii].s_str() << "\n";
                        }
                        Pagedefault(Baza,Strona_glowna,this);
                    }
                }
            };
            przyciski[2*g+1][f].pointer->checkevent=[Strona_glowna,Baza, this,g,f]{
                string t1,t2,t3,t4;
                t1=this->tablica[g][f].pointer->name;
                t2=this->tablica[g][f+1].pointer->name;
                t3=this->tablica[g+1][f+1].pointer->name;
                t4=this->tablica[g+1][f].pointer->name;
                this->tablica[g][f].pointer->name=t2;
                this->tablica[g][f+1].pointer->name=t3;
                this->tablica[g+1][f+1].pointer->name=t4;
                this->tablica[g+1][f].pointer->name=t1;
                Strona_glowna->addActive(this->tablica[g][f].id);
                Strona_glowna->addActive(this->tablica[g][f+1].id);
                Strona_glowna->addActive(this->tablica[g+1][f+1].id);
                Strona_glowna->addActive(this->tablica[g+1][f].id);
                Strona_glowna->addActive(this->przyciski[2*g][f].id);
                Strona_glowna->addActive(this->przyciski[2*g+1][f].id);

                if(!no_validate){
                    this->moves.push_back(moved(true,f,g,this->move_count++));
                    if (this->validate()){
                        Pagedefault(Baza,Strona_glowna,this);
                    }
                }
            };

        }
    }
    if(to_word==""){
        actual_word=word;
        no_validate=true;
        srand(time(0));
        for(int i=0; i<steps;i++){
            int a=rand()%(w_i-1);
            int b=rand()%(h_i-1);
            int c=rand()%2;
            przyciski[2*b+c][a].pointer->take_event();
        }
    }else{
        actual_word=to_word;
    }
    no_validate=false;
    Strona_glowna->addElement<Timer>(*Baza,"Molibden", vector<string>{"position-x:90vw",
                                                                      "position-y:10vh",
                                                                      "font-size:2.5vw",
                                                                      "font-maxwidth:170px",
                                                                      "font-minwidth:8vw",
                                                                      "font-name:./fonts/orbitron-black.ttf",
                                                                      "font:#FEBD27FF",
                                                                      "font-shadow:#00000050",
                                                                      (timer ? "time-format:hh;mm;ss": "time-format:hh;mm;ss"),
                                                                      (timer ? "crementation:-" : "crementation:+"),
                                                                      (timer ? "real-time:-":"real-time:+")},vector<string>{"width:120px",
                                                                                                    "min-width:10vw",
                                                                                                    "max-width:192px",
                                                                                                    "height:10vh",
                                                                                                    "min-height:20px",
                                                                                                    "max-height:108px",
                                                                                                    "border-radius:2px",
                                                                                                    "border-thickness:0.5vw",
                                                                                                    "shadow-offset-x:2px",
                                                                                                    "shadow-offset-y:2px"},vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                                                                 f_HTML("#000000"),
                                                                                                                                                 f_HTML("#FEF177"),
                                                                                                                                                 f_HTML("#F25420"),f_HTML("#000000")}, (timer ? timek : "00:00:00"));
    Strona_glowna->addCycle(Strona_glowna->getKlucz()-1);
    add_time_event=[Strona_glowna, ten=Strona_glowna->getKlucz()-1]{Strona_glowna->addActive(ten);};
    if(timer){
        add_event=[Strona_glowna,Baza, this]{ Pagedefault(Baza,Strona_glowna, this); };
    }
    Strona_glowna->addElement<Atom>(*Baza,"Złoto",
                                      vector<string>{"position-x:12.5vw",
                                                     "position-y:20vh",
                                                     "font-size:7.5vh",
                                                     "font-maxwidth:9vw",
                                                     "font-name:./fonts/orbitron-black.ttf",
                                                     "font:#FEBD27FF",
                                                     "font-shadow:#00000050"},vector<string>{"width:18vw",
                                                                                             "min-width:18vw",
                                                                                             "max-width:18vw",
                                                                                             "height:15vh",
                                                                                             "min-height:15vh",
                                                                                             "max-height:15vh",
                                                                                             "border-radius:2px",
                                                                                             "border-thickness:0.5vw",
                                                                                             "shadow-offset-x:2px",
                                                                                             "shadow-offset-y:2px"},
                                      vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                            f_HTML("#000000"),
                                                            f_HTML("#FEF177"),
                                                            f_HTML("#F25420"),
                                                            f_HTML("#000000")},actual_word);
    Strona_glowna->addElement<Button>(*Baza,"Czerń",
                                    vector<string>{"position-x:12.5vw",
                                                   "position-y:80vh",
                                                   "font-size:7.5vh",
                                                   "font-maxwidth:9vw",
                                                   "font-name:./fonts/orbitron-black.ttf",
                                                   "font:#FEBD27FF",
                                                   "font-shadow:#00000050"},vector<string>{"width:18vw",
                                                                                           "min-width:18vw",
                                                                                           "max-width:18vw",
                                                                                           "height:15vh",
                                                                                           "min-height:15vh",
                                                                                           "max-height:15vh",
                                                                                           "border-radius:2px",
                                                                                           "border-thickness:0.5vw",
                                                                                           "shadow-offset-x:2px",
                                                                                           "shadow-offset-y:2px"},
                                    vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                          f_HTML("#000000"),
                                                          f_HTML("#FEF177"),
                                                          f_HTML("#F25420"),
                                                          f_HTML("#000000")},"Powrót");
    add_event=[Strona_glowna,Baza, this]{ Pagedefault(Baza,Strona_glowna, this); };
}
bool Game::validate() {
    string check_word="";
    for(int g=0;g<h_i;g++){
        for(int f=0;f<w_i;f++){
            check_word+=tablica[g][f].pointer->name;
        }
    }
    if (check_word==actual_word){
        return true;
    }
    return false;
}
void Pagedefault(ButtonFactory * Baza, Page * Strona_glowna, Game * Gra){//Fun,kcja rysująca menu główne
    Strona_glowna->makeEmpty();
    Strona_glowna->addElement<Button>(*Baza,"Złoto",
                             vector<string>{"position-x:50vw",
                              "position-y:20vh",
                              "font-size:3vw",
                                 "font-maxwidth:340px",
                              "font-name:./fonts/orbitron-black.ttf",
                              "font:#FEBD27FF",
                              "font-shadow:#00000050"},vector<string>{"width:30vw",
                                                        "min-width:20px",
                                                        "max-width:350px",
                                                        "height:15vh",
                                                        "min-height:20px",
                                                        "max-height:200px",
                                                        "border-radius:2px",
                                                        "border-thickness:0.5vw",
                                                        "shadow-offset-x:2px",
                                                        "shadow-offset-y:2px"},
                             vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                              f_HTML("#000000"),
                              f_HTML("#FEF177"),
                              f_HTML("#F25420"),
                              f_HTML("#000000")},"Nowa gra Solo");//Przycisk nowej gry solo

    add_event=[page=Strona_glowna,factory=Baza, Gra]{
        PageNewGameSolo(factory,page, Gra);
    };
    //Przycisk nowej gry online
    Strona_glowna->addButton(*Baza,Inny,"Nowa Gra Online","50vw", "50vh");
    //Przycisk Nowej Gry Rankingowej
    Strona_glowna->addButton(*Baza,Inny,"Gra Rankingowa","50vw", "80vh");
    //Przycisk do zalogowania na nowe konto
    Strona_glowna->addElement<TriangleButton>(*Baza,"Srebro",vector<string>{"position-x:16vw",
                                                                            "position-y:16vh",
                                                                            "font-size:3vw",
                                                                            "font-maxwidth:170px",
                                                                            "font-minwidth:12vw",
                                                                            "font-name:./fonts/orbitron-black.ttf",
                                                                            "font:#FEBD27FF",
                                                                            "font-shadow:#00000050"},vector<string>{"width:30vw","min-width:20px",
                                                                                                                    "max-width:350px",
                                                                                                                    "height:30vh",
                                                                                                                    "min-height:20px",
                                                                                                                    "max-height:350px",
                                                                                                                    "border-radius:2px",
                                                                                                                    "border-thickness:0.5vw",
                                                                                                                    "shadow-offset-x:2px",
                                                                                                                    "shadow-offset-y:2px"},
                                              vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                    f_HTML("#000000"),
                                                                    f_HTML("#FEF177"),
                                                                    f_HTML("#F25420"),
                                                                    f_HTML("#000000")},"Zaloguj:U");
    add_event=[page=Strona_glowna,factory=Baza, Gra] {
        PageZaloguj(factory,page, Gra);
    };

    //Przycisk do zarejestrowania konta
    Strona_glowna->addElement<TriangleButton>(*Baza,"Brąz",vector<string>{"position-x:18vw",
                                                                            "position-y:18vh",
                                                                            "font-size:3vw",
                                                                            "font-maxwidth:170px",
                                                                            "font-minwidth:12vw",
                                                                            "font-name:./fonts/orbitron-black.ttf",
                                                                            "font:#FEBD27FF",
                                                                            "font-shadow:#00000050"},vector<string>{"width:30vw",
                                                                                                                    "min-width:20px",
                                                                                                                    "max-width:350px",
                                                                                                                    "height:30vh",
                                                                                                                    "min-height:20px",
                                                                                                                    "max-height:350px",
                                                                                                                    "border-radius:2px",
                                                                                                                    "border-thickness:0.5vw",
                                                                                                                    "shadow-offset-x:2px",
                                                                                                                    "shadow-offset-y:2px"},
                                              vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                    f_HTML("#000000"),
                                                                    f_HTML("#FEF177"),
                                                                    f_HTML("#F25420"),
                                                                    f_HTML("#000000")},"Zarejestruj:D");
    add_event=[page=Strona_glowna,factory=Baza, Gra]{
        zarejestruj(factory,page, Gra);
    };
    //Timer
    Strona_glowna->addElement<Timer>(*Baza,"Molibden", vector<string>{"position-x:90vw",
                                                                      "position-y:10vh",
                                                                      "font-size:2.5vw",
                                                                      "font-maxwidth:170px",
                                                                      "font-minwidth:8vw",
                                                                      "font-name:./fonts/orbitron-black.ttf",
                                                                      "font:#FEBD27FF",
                                                                      "font-shadow:#00000050",
                                                                      "time-format:hh;mm;ss",
                                                                      "crementation:+",
                                                                      "real-time:+"},vector<string>{"width:120px",
                                                                                                    "min-width:10vw",
                                                                                                    "max-width:192px",
                                                                                                    "height:10vh",
                                                                                                    "min-height:20px",
                                                                                                    "max-height:108px",
                                                                                                    "border-radius:2px",
                                                                                                    "border-thickness:0.5vw",
                                                                                                    "shadow-offset-x:2px",
                                                                                                    "shadow-offset-y:2px"},vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                                                                 f_HTML("#000000"),
                                                                                                                                                 f_HTML("#FEF177"),
                                                                                                                                                 f_HTML("#F25420"),f_HTML("#000000")}, "00;00;00");
    Strona_glowna->addCycle(Strona_glowna->getKlucz()-1);
    add_time_event=[Strona_glowna, ten=Strona_glowna->getKlucz()-1]{Strona_glowna->addActive(ten);};
}
void add_last_textfield(Page * Strona_glowna){
    Strona_glowna->addCycle(Strona_glowna->getKlucz()-1);
    add_time_event=[Strona_glowna, ten=Strona_glowna->getKlucz()-1]{Strona_glowna->addActive(ten);};
    add_event=[Strona_glowna,ten=Strona_glowna->getKlucz()-1]{
        (Strona_glowna->buttons[ten]->checker()==Clicked ? Strona_glowna->buttons[ten]->normal():Strona_glowna->buttons[ten]->clicked());
        (Strona_glowna->buttons[ten]->checker()==Clicked ? Strona_glowna->addTK(ten):Strona_glowna->addTK(0));
    };
}
void zarejestruj(ButtonFactory * Baza,  Page *Strona_glowna, Game *Gra) {
    Strona_glowna->makeEmpty();
    Strona_glowna->addElement<Atom>(*Baza, "Orange", vector<string>{"position-x:33.5vw",
                                                                        "position-y:20vh",
                                                                        "font-size:7vh",
                                                                        "font-name:./fonts/orbitron-black.ttf",
                                                                        "font:#7a2160",
                                                                        "font-shadow:#290b20",
                                                                        "font-maxwidth:15vw"},
                                                                    vector<string>{"width:17vw","min-width:17vw","max-width:17vw",
                                                                        "height:10vh","min-height:10vh","max-height:10vh",
                                                                        "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                                        vector<ALLEGRO_COLOR>{f_HTML("#f2bf41"),f_HTML("#7a6021"),f_HTML("#000000")}, "Podaj nick:");
    Strona_glowna->addElement<TextField>(*Baza,"TextField", vector<string>{"position-x:61vw",
                                                                          "position-y:20vh",
                                                                          "font-size:5vh",
                                                                          "font-name:./fonts/orbitron-black.ttf",
                                                                          "font:#7a2160",
                                                                          "font-shadow:#290b20",
                                                                          "font-maxwidth:30vw",
                                                                          "Background-color:#f0f0f0",
                                                                          "Frame-color:#101010"
                                                                          },vector<string>{"width:33vw","min-width:33vw","max-width:33vw",
                                                                                           "height:10vh","min-height:10vh","max-height:10vh",
                                                                                           "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                                            vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                  f_HTML("#000000"),
                                                                                                  f_HTML("#FEF177"),
                                                                                                  f_HTML("#F25420"),f_HTML("#000000"),
                                                                                                  f_HTML("#beff56")}, "Wpisz nick");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Podaj e-mail:","33.5vw", "32vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Wpisz e-mail","61vw", "32vh");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Powtórz e-mail:","33.5vw", "44vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Wpisz e-mail","61vw", "44vh");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Podaj hasło:","33.5vw", "56vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Wpisz hasło","61vw", "56vh");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Powtórz hasło:","33.5vw", "68vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Wpisz hasło","61vw", "68vh");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addElement<Button>(*Baza,"Złoto",
vector<string>{"position-x:40vw",
                                             "position-y:80vh",
                                             "font-size:5vh",
                                             "font-name:./fonts/orbitron-black.ttf",
                                             "font:#7a2160",
                                             "font-shadow:#290b20",
                                             "font-maxwidth:16vw",
                                             "Background-color:#f0f0f0",
                                             "Frame-color:#101010"
                                             },vector<string>{"width:17.5vw","min-width:17.5vw","max-width:17.5vw",
                                                              "height:10vh","min-height:10vh","max-height:10vh",
                                                              "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                               vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                     f_HTML("#000000"),
                                                                     f_HTML("#FEF177"),
                                                                     f_HTML("#F25420"),f_HTML("#000000"),
                                                                     f_HTML("#beff56")},"Powrót");
    add_event=[Strona_glowna,Baza,Gra]{ Pagedefault(Baza,Strona_glowna, Gra);};
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Zarejestruj.","60vw","80vh");
}
void PageZaloguj(ButtonFactory *Baza, Page *Strona_glowna, Game *Gra) {
    Strona_glowna->makeEmpty();
    Strona_glowna->addElement<Atom>(*Baza, "Orange", vector<string>{"position-x:33.5vw",
                                                                        "position-y:20vh",
                                                                        "font-size:7vh",
                                                                        "font-name:./fonts/orbitron-black.ttf",
                                                                        "font:#7a2160",
                                                                        "font-shadow:#290b20",
                                                                        "font-maxwidth:15vw"},
                                                                    vector<string>{"width:17vw","min-width:17vw","max-width:17vw",
                                                                        "height:10vh","min-height:10vh","max-height:10vh",
                                                                        "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                                        vector<ALLEGRO_COLOR>{f_HTML("#f2bf41"),f_HTML("#7a6021"),f_HTML("#000000")}, "Podaj nick:");
    Strona_glowna->addElement<TextField>(*Baza,"TextField", vector<string>{"position-x:61vw",
                                                                          "position-y:20vh",
                                                                          "font-size:5vh",
                                                                          "font-name:./fonts/orbitron-black.ttf",
                                                                          "font:#7a2160",
                                                                          "font-shadow:#290b20",
                                                                          "font-maxwidth:30vw",
                                                                          "Background-color:#f0f0f0",
                                                                          "Frame-color:#101010"
                                                                          },vector<string>{"width:33vw","min-width:33vw","max-width:33vw",
                                                                                           "height:10vh","min-height:10vh","max-height:10vh",
                                                                                           "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                                            vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                  f_HTML("#000000"),
                                                                                                  f_HTML("#FEF177"),
                                                                                                  f_HTML("#F25420"),f_HTML("#000000"),
                                                                                                  f_HTML("#beff56")}, "Wpisz nick");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Podaj hasło:","33.5vw", "32vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Wpisz hasło","61vw", "32vh");
    add_last_textfield(Strona_glowna);Strona_glowna->addElement<Button>(*Baza,"Złoto",
    vector<string>{"position-x:40vw",
                                                 "position-y:44vh",
                                                 "font-size:5vh",
                                                 "font-name:./fonts/orbitron-black.ttf",
                                                 "font:#7a2160",
                                                 "font-shadow:#290b20",
                                                 "font-maxwidth:16vw",
                                                 "Background-color:#f0f0f0",
                                                 "Frame-color:#101010"
                                                 },vector<string>{"width:17.5vw","min-width:17.5vw","max-width:17.5vw",
                                                                  "height:10vh","min-height:10vh","max-height:10vh",
                                                                  "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                   vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                         f_HTML("#000000"),
                                                                         f_HTML("#FEF177"),
                                                                         f_HTML("#F25420"),f_HTML("#000000"),
                                                                         f_HTML("#beff56")},"Powrót");
    add_event=[Strona_glowna,Baza, Gra]{ Pagedefault(Baza,Strona_glowna, Gra);};
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Zaloguj","60vw","44vh");
}
void PageNewGameSolo(ButtonFactory *Baza, Page *Strona_glowna, Game * Gra) {
    Strona_glowna->makeEmpty();
    Strona_glowna->addElement<Atom>(*Baza, "Orange", vector<string>{"position-x:33.5vw",
                                                                        "position-y:20vh",
                                                                        "font-size:7vh",
                                                                        "font-name:./fonts/orbitron-black.ttf",
                                                                        "font:#7a2160",
                                                                        "font-shadow:#290b20",
                                                                        "font-maxwidth:15vw"},
                                                                    vector<string>{"width:17vw","min-width:17vw","max-width:17vw",
                                                                        "height:10vh","min-height:10vh","max-height:10vh",
                                                                        "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                                        vector<ALLEGRO_COLOR>{f_HTML("#f2bf41"),f_HTML("#7a6021"),f_HTML("#000000")}, "Podaj słowo:");
    Strona_glowna->addElement<TextField>(*Baza,"TextField", vector<string>{"position-x:61vw",
                                                                          "position-y:20vh",
                                                                          "font-size:5vh",
                                                                          "font-name:./fonts/orbitron-black.ttf",
                                                                          "font:#7a2160",
                                                                          "font-shadow:#290b20",
                                                                          "font-maxwidth:30vw",
                                                                          "Background-color:#f0f0f0",
                                                                          "Frame-color:#101010"
                                                                          },vector<string>{"width:33vw","min-width:33vw","max-width:33vw",
                                                                                           "height:10vh","min-height:10vh","max-height:10vh",
                                                                                           "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                                            vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                  f_HTML("#000000"),
                                                                                                  f_HTML("#FEF177"),
                                                                                                  f_HTML("#F25420"),f_HTML("#000000"),
                                                                                                  f_HTML("#beff56")}, "Wpisz słowo");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Podaj liczbę liter w wierszu","33.5vw", "32vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"4","61vw", "32vh");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Podaj liczbę losowych przesunięć","33.5vw", "44vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"5","61vw", "44vh");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Podaj czas (puste pole oznacza brak limitu):","33.5vw", "56vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"00:05:00","61vw", "56vh");
    add_last_textfield(Strona_glowna);
    Strona_glowna->addElement<Button>(*Baza,"Złoto",
    vector<string>{"position-x:40vw",
                                                 "position-y:68vh",
                                                 "font-size:5vh",
                                                 "font-name:./fonts/orbitron-black.ttf",
                                                 "font:#7a2160",
                                                 "font-shadow:#290b20",
                                                 "font-maxwidth:16vw",
                                                 "Background-color:#f0f0f0",
                                                 "Frame-color:#101010"
                                                 },vector<string>{"width:17.5vw","min-width:17.5vw","max-width:17.5vw",
                                                                  "height:10vh","min-height:10vh","max-height:10vh",
                                                                  "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:1px","shadow-offset-y:1px"},
                                                   vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                         f_HTML("#000000"),
                                                                         f_HTML("#FEF177"),
                                                                         f_HTML("#F25420"),f_HTML("#000000"),
                                                                         f_HTML("#beff56")},"Powrót");
    add_event=[Strona_glowna,Baza,Gra]{ Pagedefault(Baza,Strona_glowna, Gra);};
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Start","60vw","68vh");
    add_event=[Strona_glowna,Baza,Gra]{

        Gra->Start(Strona_glowna,Baza, ostatni_przycisk(8)->name,"", stoi(ostatni_przycisk(6)->name), stoi(
                ostatni_przycisk(4)->name),(ostatni_przycisk(2)->name.empty() ? false : true), (ostatni_przycisk(2)->name.empty()? "00;00;00" : replaceAll(ostatni_przycisk(2)->name,":",";")));
    };
    
}
