#include <iostream>
#include <map>
#include <memory>
#include<string>
#include <vector>
#include <ranges>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
using namespace std;
// Struktura przechowująca styl i wymiary (obsługuje px, vh, vw)

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
ALLEGRO_COLOR f_HTML(string html_color);
vector<string> split_manual(string s,const string delimiter);
void AllegroImageDeleter(ButtonImage* bi);
class ButtonFactory {
    //para klucz(string), wartość(słaby pointer buttonParameters)
    map<string,shared_ptr<ButtonParameters>> styles;
    public:
    shared_ptr<ButtonParameters> getOrCreate(string id, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={});
    private:
    void updateParams(shared_ptr<ButtonParameters> p, const vector<string>& res, const vector<ALLEGRO_COLOR>& col);

    };
class Button{
    string name;
    string posx, posy;
    shared_ptr<ButtonParameters> param;

public:
    Button(ButtonFactory& factory, string styleID, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam);
    Button(ButtonFactory& factory, string styleID, const vector<string>& res, string nam);
    ~Button();
private:
    void extractPosition(const vector<string>& res);
};