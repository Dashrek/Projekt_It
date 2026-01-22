#include <iostream>
#include<string>
#include <vector>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
using namespace std;
struct ButtonImage{
    string width;
    string maxwidth;
    string minwidth;
    string height;
    string maxheight;
    string minheight;
    ALLEGRO_BITMAP* normal;
    ALLEGRO_BITMAP* hover;
    ALLEGRO_BITMAP* pressed;
};
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
    name(name);
}
