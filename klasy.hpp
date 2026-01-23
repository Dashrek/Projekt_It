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
    ButtonImage* images;
} ButtonParameters;
ALLEGRO_COLOR f_HTML(string html_color);
vector<string> split_manual(string s,const string delimiter);

class Button{
    string name;
    string posx;
    string posy;
    shared_ptr<ButtonParameters> param;
    public:
        Button(const string id, const vector<string>& resolution,const vector<ALLEGRO_COLOR>& Colors,string nam);
        ~Button();

};
class ButtonFactory {
    //para klucz(string), wartość(słaby pointer buttonParameters)
    map<string,weak_ptr<ButtonParameters>> styles;
    public:
    shared_ptr<ButtonParameters> getStyle(string styleID);


};
