#include <iostream>
#include<string>
#include <vector>
#include <ranges>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
using namespace std;
// Struktura przechowująca styl i wymiary (obsługuje px, vh, vw)
typedef struct {
    // Geometria
    string width, maxwidth, minwidth;
    string height, maxheight, minheight;
    string pos_x, pos_y;

    // Wygląd pudełka (Box Model)
    string border_radius;     // np. "15px"
    string border_thickness;  // np. "2px"

    // Cieniowanie obramowania/przycisku
    string shadow_offset_x;   // przesunięcie poziome cienia
    string shadow_offset_y;   // przesunięcie pionowe cienia

    // Kolory stanów obramowania i cienia
    ALLEGRO_COLOR border_normal;
    ALLEGRO_COLOR border_hover;
    ALLEGRO_COLOR border_active;
    ALLEGRO_COLOR shadow_color;
} ButtonParameters;
typedef struct {
    ButtonParameters* parameters;
    ALLEGRO_BITMAP* normal;
    ALLEGRO_BITMAP* hover;
    ALLEGRO_BITMAP* pressed;
} ButtonImage;

ALLEGRO_COLOR f_HTML(string html_color) {
    int r=stoi(html_color.substr(1,2),nullptr,16);
    int g=stoi(html_color.substr(3,2),nullptr,16);
    int b=stoi(html_color.substr(5,2),nullptr,16);
    if (html_color.length()==7) {
        return al_map_rgb( r,g,b );
    }
    if (html_color.length()==9) {
        int a = stoi(html_color.substr(7, 2), nullptr, 16);
        return al_map_rgba( r,g,b ,a);
    }
};
vector<string> split_manual(string s, string delimiter) {
    size_t pos = 0;
    vector<string> res;
    while ((pos = s.find(delimiter)) != string::npos) {
        res.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);
    return res;
}
class Button{
    string name;
    public:
        Button(vector<string>& resolution,vector<ALLEGRO_COLOR>& Colors, string name);
        ~Button();

};

inline Button::Button(vector<string>& resolution, vector<ALLEGRO_COLOR>& Colors, string nam):name(nam){
/* resolution zawiera następujące wymiary:
 * wysokośćw formie zwykłej wysokości, szerokość w fomie zwykłej szerokości
 * potem są maxy i miny.
 * wszystko musi być dobrze napisane width: 110vh 100 vw, 200px- vh,vw, px i % jak w html
 * radius:promień zaokrąglenia
 *
 * Colors zawiera vector kolorów
 * k1- ramka1-kolor ramki
 * k2- ramka2-kolor cienia ramki
 * k3- kolor środka
 * k4- kolor fontu
 */
ButtonParameters
    for (auto i : resolution){
        vector<string> k= split_manual(i,":");
        string width, height;
        if (k[0]=="width"){
            width=k[1];
        }else if (k[0]=="min-width"){

        }
    }
};
