//
// Created by karol on 23.01.2026.
//
#include "klasy.hpp"
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
vector<string> split_manual(string s,const string delimiter) {
    size_t pos = 0;
    vector<string> res;
    while ((pos = s.find(delimiter)) != string::npos) {
        res.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);
    return res;
}
Button::Button(const vector<string>& resolution,const vector<ALLEGRO_COLOR>& Colors,string nam):name(nam){
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
    param= new ButtonParameters;
    param->images=new ButtonImage;
    for (auto i : resolution){
        vector<string> k= split_manual(i,":");
        if (k[0]=="width"){
            param->width=k[1];
        }else if (k[0]=="min-width"){
            param->minwidth=k[1];
        }else if (k[0]=="max-width") {
            param->maxwidth=k[1];
        }else if (k[0]=="height") {
            param->height=k[1];
        }else if (k[0]=="max-height") {
            param->maxheight=k[1];
        }else if (k[0]=="border-radius") {
            param->border_radius=k[1];
        }else if (k[0]=="border-thickness") {
            param->border_thickness=k[1];
        }else if (k[0]=="shadow-offset-x") {
            param->shadow_offset_x=k[1];
        }else if (k[0]=="shadow-offset-y") {
            param->shadow_offset_y=k[1];
        }else if (k[0]=="position-x") {
            posx=k[1];
        }else if (k[0]=="position-y") {
            posy=k[1];
        }
    }
    param->button=Colors[0];
    param->border_normal=Colors[1];
    param->border_hover=Colors[2];
    param->border_active=Colors[3];
    param->shadow_color=Colors[4];
};

Button::~Button() {

}
