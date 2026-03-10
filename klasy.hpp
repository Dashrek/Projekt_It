#include <iostream>
#include <map>
#include <memory>
#include<string>
#include <vector>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <mutex>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;
// Struktura przechowująca styl i wymiary (obsługuje px, vh, vw)
static mutex font_mutex;
inline int screen_width, screen_height;
typedef struct {
    ALLEGRO_BITMAP* normal;//wyliczona mapa normal
    ALLEGRO_BITMAP* hover;// wyliczona mapa przy najechaniu
    ALLEGRO_BITMAP* pressed;// wyliczona mapa przy naciśnięciu
    ALLEGRO_BITMAP* clicked;//Mapa dla przycisków i itemów, które mogą zostać kliknięte i podtrzymane, do czasu, aż nie kliknie się w nie drugi raz. np textfield.
} ButtonImage;
typedef struct {
    int x;
    int y;
}punkt;
class trojkat {
protected:
    punkt A;
    punkt B;
    virtual void addMorePointsH(punkt Cx);
private:
    punkt C;
    float a,b;
public:
    trojkat( punkt Ax, punkt Bx, punkt Cx={0,0});
    void addMorePoints(punkt Cx={0,0});
    virtual bool check(int x, int y, int typ);
};
class kwadrat:public trojkat{
protected:
    void addMorePointsH(punkt Cx) override;
public:
    kwadrat(punkt Ax, punkt Bx, punkt Cx={0,0});
    bool check(int x, int y, int typ) override;
};
typedef struct {
    // Geometria
    string width, maxwidth, minwidth;//parametry wysokości przycisku
    string height, maxheight, minheight;//parametry szerokości przycisku

    // Wygląd pudełka (Box Model)
    string border_radius;     // np. "15px"
    string border_thickness;  // np. "2px"
    // Cieniowanie obramowania/przycisku
    string shadow_offset_x;   // przesunięcie poziome cienia
    string shadow_offset_y;   // przesunięcie pionowe cienia
    // Kolory stanów obramowania i cienia
    ALLEGRO_COLOR border_normal;//kolor obramowania przycisku w normalnej pozycji-żółty
    ALLEGRO_COLOR button;//kolor główny przycisku
    ALLEGRO_COLOR border_hover;//kolor po najechaniu
    ALLEGRO_COLOR border_active;//kolor po naciśnięciu
    ALLEGRO_COLOR border_clicked;
    ALLEGRO_COLOR shadow_color;//kolor cienia
    int typ;
    shared_ptr<ButtonImage> images;
} ButtonParameters;
int actual_value(const string &value);
ALLEGRO_COLOR f_HTML(string html_color);
vector<string> split_manual(string s,const string delimiter);
void AllegroImageDeleter(ButtonImage* bi);
void AllegroGaussFilter(ALLEGRO_BITMAP* Source, ALLEGRO_BITMAP* Target, int w, int h);
void getCurrentDateTime(int &h, int &m, int &s, double &stamp);
string zfill(int number, int width);
string another_name(string nam, int pozycja_kursora);
size_t next_utf8(const std::string& s, size_t i);
size_t prev_utf8(const std::string& s, size_t i);
bool BakeFontToMemoryBitmap(ALLEGRO_BITMAP* dest,ALLEGRO_FONT* font,const string& text,ALLEGRO_COLOR color,int x = 0,int y = 0);
enum Typ{Przycisk,Pierwiastek, PoleTekstowe, TriangleU, TriangleD, Zegar};
enum Status{Normal,Hover,Active,Clicked};
class ButtonFactory {
    //para klucz(string), wartość(słaby pointer buttonParameters)
    map<string,shared_ptr<ButtonParameters>> styles;
public:
    ButtonFactory();
    shared_ptr<ButtonParameters> getOrCreate(string id,int typ=Przycisk, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={});
    void ReCreateRectangle();
private:
    void updateParams(shared_ptr<ButtonParameters> p, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, int typ);
    void createRectangle(shared_ptr<ButtonParameters> p);

};

class Atom {
protected:
    //wyświetlana nazwa w atomie
    shared_ptr<ButtonParameters> param;//współdzielone tło atomów, przycisków- różnią się tylko napisem, zaś przyciski funkcjami
    string posx, posy;//miejsce na mapie wyrażone w jednej z trzech jednostek- vh,vw oraz px, znanych z html
    string fontsize,font,fontmaxwidth,fontminwidth;//kolejno rozmiar czcionki, nazwa czcionki na dysku, maksymalna i miniamalna szerokość czcionki
    ALLEGRO_COLOR font_color,font_shadow_color;//kolory czcionki i jej cienia;
    ALLEGRO_FONT *Font=nullptr;
    void virtual extractPositionH(const vector<string>& res);//funkcja wirtualna wyciągająca parametry przycisku niezwiązane z fabryką przycisków/ atomów
    void virtual generateFontH();// funkcja wirtualna tworząca czcionkę
    void virtual buildH();//funkcja wirtualna budująca przycisk/atom
    void take_time_event();
    Atom(ButtonFactory& Factory, string styleID, const vector<string>& font_h, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam,int typ);//Inicjator przycisku- tajny
public:
    string name;
    Atom(ButtonFactory& Factory, string styleID, const vector<string>& font_h, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam);
    Atom(const Atom& Inny, ButtonFactory& factory, const string nazwa, const string pos_x, const string pos_y);//jawne inicjatory przycisków
    void build();//build używany w inicjatorach- wywołuje buildH
    void extractPosition(const vector<string>& res);//extract position używany w inicjatorach- wywołuje extractPositionH
    void generateFont();//generate font używany w inicjatorach- wywołuje generateFontH;
    void virtual ReaddRange(){};//pusta funkcja pełniąca zadanie w tworzeniu aktualnego zasięgu przycisku na display
    virtual std::unique_ptr<Atom> clone(ButtonFactory& factory,
                                        const std::string& nazwa,
                                        const std::string& pos_x,
                                        const std::string& pos_y) const
    {
        return std::make_unique<Atom>(*this, factory, nazwa, pos_x, pos_y);
    }//obiekt klonujący przyciski
    function <void()> checkTimeEvent;
    function <void()> checkevent;
    void virtual take_event(){};
    int virtual checker(){return 0;};
    virtual void hover(){};//funkcje przycisków w atomie puste, takie jak w html
    virtual void pressed(){};
    virtual void normal(){};
    virtual void clicked(){};// funkcja dla wciśniętych przycisków
    virtual void thic(){};//funkcja zmieniająca parametry przycisku w czasie
    virtual void draw(ALLEGRO_COLOR color){};//funkcja alternatywna dla ReaddRange
    virtual bool check(int x, int y){return false;};//check funkcja sprawdzająca w przyciskach, czy jest najechany kursorem
    virtual ~Atom();//destruktor przycisku
    virtual void add(const ALLEGRO_EVENT& ev){cout<<"Ta funkcja nie jest przeznaczona dla atomu\n";};
};
class Timer : public Atom{//klasa, która podaje timer, która rysuje zegar i odlicza czas:
protected:
    int hours;//można wyświetlać
    int minutes;
    int seconds;
    string time_format; // hh:mm:ss, mm:ss;
    bool inc_decr;//inkrementacja,dekrementacja; uzupełnianie czasu przez
    bool real_time;//inkrementacja uzupełnia czas przez odczyt systemowy, może być tylko w formacie hh mm ss i tylko w inkrementacji- to zegar;
    //jeżeli name jest pusty, to pobierany jest czas systemowy, a
    void extractPositionH(const vector<string>& res) override;
    double timer;

public:
    void take_event() override;
    void thic() override;
    Timer(ButtonFactory& Factory, string styleID, const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="");
};

class Button : public Atom{
    //aspekty wizualne


public:
    Button(ButtonFactory& factory, string styleID,const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="");
    Button(const Button& Inny, ButtonFactory& factory, string nazwa, string pos_x, string pos_y);
    void hover() override;
    void pressed() override;
    void normal() override;
    void clicked() override;
    int checker() override;
    void take_event() override;
    void draw(ALLEGRO_COLOR color) override;
    void ReaddRange() override;
    bool check(int x, int y) override;
    std::unique_ptr<Atom> clone(ButtonFactory& factory,
                                const std::string& nazwa,
                                const std::string& pos_x,
                                const std::string& pos_y) const override
    {
        return std::make_unique<Button>(*this, factory, nazwa, pos_x, pos_y);
    }
protected:
    unique_ptr<trojkat> t;
    bool tryb[2];
    void buildH() override;

    Button(ButtonFactory& Factory, string styleID, const vector<string>& font_h, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam,int typ);
};
class TextField: public Button {
protected:
    bool kursor, hoverx,clickedx;
    int pozycja_kursora;
    void buildH() override;
    void extractPositionH(const vector<string> &res) override;
    double timer;
    ALLEGRO_COLOR Background, Rama;
    ALLEGRO_BITMAP* Ramka;
    string name_another;
    void generateFontH() override;
public:

    void thic() override;
    TextField(ButtonFactory& Factory, string styleID, const vector<string>& font_h, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam);
    TextField(const TextField& Inny, ButtonFactory& factory, const string nazwa, const string pos_x, const string pos_y);
    std::unique_ptr<Atom> clone(ButtonFactory& factory,
                               const std::string& nazwa,
                               const std::string& pos_x,
                               const std::string& pos_y) const override
    {
        return std::make_unique<TextField>(*this, factory, nazwa, pos_x, pos_y);
    }
    void add(const ALLEGRO_EVENT& ev) override;
};
class TriangleButton : public Button{
    //aspekty wizualne
public:
    TriangleButton(ButtonFactory& factory, string styleID,const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="");
    TriangleButton(const TriangleButton& Inny, ButtonFactory& factory, string nazwa, string pos_x, string pos_y);
    void draw(ALLEGRO_COLOR color) override;
    void ReaddRange() override;
    std::unique_ptr<Atom> clone(ButtonFactory& factory,
                                const std::string& nazwa,
                                const std::string& pos_x,
                                const std::string& pos_y) const override
    {
        return std::make_unique<TriangleButton>(*this, factory, nazwa, pos_x, pos_y);
    }
protected:
    void buildH() override;
};
class Page{
    vector<int> cykliczne;
    vector<int> aktywne;
    int aktywny_przycisk;
    ALLEGRO_BITMAP *przyciski;
    ALLEGRO_BITMAP *Backbuffer;
    int aktualny_klucz;

public:
    Page();
    ~Page();
    int tekstowy_klucz;
    const int getKlucz() const{return aktualny_klucz;};
    void saveBackbuffer(ALLEGRO_DISPLAY *display);
    void createBitmap();
    bool start;
    void createBackbuffer();
    Atom* getButton(int key);
    void addCycle(int a);
    void addActive(int a);
    void changeRanges();
    void flushCycle();
    void makeEmpty();
    void thicCycle();
    bool findButtonHover(int x, int y, bool &a);
    void findButtonActive(int x, int y, bool a);
    void addTK(int a);
    bool hover(int x, int y);
    map<int,unique_ptr<Atom>> buttons;

    template<typename T, typename... Args> void addElement(Args&&... args){
        buttons[aktualny_klucz] =
                std::make_unique<T>(std::forward<Args>(args)...);

        aktualny_klucz = (aktualny_klucz == 255 ? 1 : aktualny_klucz + 1);
    }
    void addButton(ButtonFactory &factory,const Atom & Inny, const string nazwa,const string pos_x, const string pos_y);
    void buildButtons(ALLEGRO_DISPLAY *obraz);
    void ReloadFont();//ponowne załadowanie czcionki
    void receiveFunctions();
};