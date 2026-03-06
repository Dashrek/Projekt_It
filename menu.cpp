#include "menu.hpp"
#define add_event Strona_glowna->buttons[Strona_glowna->getKlucz()-1]->checkevent
#define Inny *Strona_glowna->buttons[Strona_glowna->getKlucz()-1]
#define add_time_event Strona_glowna->buttons[Strona_glowna->getKlucz()-1]->checkTimeEvent
void Pagedefault(ButtonFactory * Baza, Page * Strona_glowna){//Funkcja rysująca menu główne
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

    add_event=[page=Strona_glowna,factory=Baza]{
        PageNewGameSolo(factory,page);
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
    add_event=[page=Strona_glowna,factory=Baza]{
        zarejestruj(factory,page);
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
void zarejestruj(ButtonFactory * Baza,  Page *Strona_glowna) {
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
    add_event=[Strona_glowna,Baza]{ Pagedefault(Baza,Strona_glowna);};
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Zarejestruj.","60vw","80vh");
}
void PageNewGameSolo(ButtonFactory *Baza, Page *Strona_glowna) {
    Strona_glowna->makeEmpty();
    Strona_glowna->addElement<Atom>(*Baza, "Orange", vector<string>{"position-x:42.5vw",
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
    Strona_glowna->addElement<TextField>(*Baza,"TextField", vector<string>{"position-x:50vw",
                                                                          "position-y:32vh",
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
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Podaj liczbę liter w wierszu","42.5vw", "44vh");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-2],"Wpisz liczbę","50vw", "56vh");
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
    add_event=[Strona_glowna,Baza]{ Pagedefault(Baza,Strona_glowna);};
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Start","60vw","68vh");
    
}
