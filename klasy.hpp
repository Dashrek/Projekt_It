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
#include <math.h>
#include <functional>
using namespace std;
// Struktura przechowująca styl i wymiary (obsługuje px, vh, vw)

inline int screen_width, screen_height;
typedef struct {
    ALLEGRO_BITMAP* normal;//wyliczona mapa normal
    ALLEGRO_BITMAP* hover;// wyliczona mapa przy najechaniu
    ALLEGRO_BITMAP* pressed;// wyliczona mapa przy naciśnięciu
} ButtonImage;

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
    ALLEGRO_COLOR shadow_color;//kolor cienia
    shared_ptr<ButtonImage> images;
} ButtonParameters;
int actual_value(const string &value);
ALLEGRO_COLOR f_HTML(string html_color);
vector<string> split_manual(string s,const string delimiter);
void AllegroImageDeleter(ButtonImage* bi);
void AllegroGaussFilter(ALLEGRO_BITMAP* Source, ALLEGRO_BITMAP* Target, int w, int h);
bool BakeFontToMemoryBitmap(ALLEGRO_BITMAP* dest,ALLEGRO_FONT* font,const string& text,ALLEGRO_COLOR color,int x = 0,int y = 0);
class ButtonFactory {
    //para klucz(string), wartość(słaby pointer buttonParameters)
    map<string,shared_ptr<ButtonParameters>> styles;
    public:
        ButtonFactory();
        shared_ptr<ButtonParameters> getOrCreate(string id, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={});
        void ReCreateRectangle();
    private:
        void updateParams(shared_ptr<ButtonParameters> p, const vector<string>& res, const vector<ALLEGRO_COLOR>& col);
        void createRectangle(shared_ptr<ButtonParameters> p);
    };
class Button{
    //aspekty wizualne
    string name;
    string posx, posy;
    string fontsize,font,fontmaxwidth,fontminwidth;
    ALLEGRO_COLOR font_color,font_shadow_color;
    shared_ptr<ButtonParameters> param;
    function <void()> checkevent;
    //aspekty logiczne
    //wybór trybu wyświetlenia
    bool tryb[2];
public:
    Button(ButtonFactory& factory, string styleID,const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="");
    Button(const Button& Inny, ButtonFactory& factory, const string nazwa, const string pos_x, const string pos_y);
    void hover();
    void pressed();
    void normal();
    void take_event();
    ~Button();
    void build();
    void draw(ALLEGRO_COLOR color);
    void generateFont();
private:
    ALLEGRO_FONT *Font=nullptr;
    void extractPosition(const vector<string>& res);
};
class Page{
    vector<int> cykliczne;
    int aktywny_przycisk;
    ALLEGRO_BITMAP *przyciski;
    int aktualny_klucz;
public:
    const int getKlucz() const{return aktualny_klucz;};
    void createBitmap();
    void hover(int x, int y);
    map<int,unique_ptr<Button>> buttons;
    Page();
    void addButton(ButtonFactory& factory, string styleID, string nam="", const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={});
    void addButton(ButtonFactory &factory,const Button & Inny, const string nazwa,const string pos_x, const string pos_y);
    void buildButtons(ALLEGRO_DISPLAY *obraz);
    void ReloadFont();//ponowne załadowanie czcionki
    void receiveFunctions();
};