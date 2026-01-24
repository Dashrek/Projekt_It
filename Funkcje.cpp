//
// Created by karol on 23.01.2026.
//
#include <allegro5/allegro_ttf.h>

#include "klasy.hpp"
int actual_value(const string &value) {
    if (value.find("vh")) {
        return static_cast<int>(static_cast<float>(stoi(split_manual(value, "vh")[0])) / 100.0f * static_cast<float>(screen_height));
    }if (value.find("vw")) {
        return static_cast<int>(static_cast<float>(stoi(split_manual(value, "vw")[0])) / 100.0f * static_cast<float>(screen_width));
    }if (value.find("px")) {
        return stoi(split_manual(value,"px")[0]);
    }
    return -1;
}
ALLEGRO_COLOR f_HTML(string html_color) {//funkcja konwertująca kolor html na kolor w wersji zwykłej + alpha
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
    return al_map_rgba(0,0,0,255);
};
vector<string> split_manual(string s,const string delimiter) {//własny split
    size_t pos = 0;
    vector<string> res;
    while ((pos = s.find(delimiter)) != string::npos) {
        res.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    res.push_back(s);
    return res;
}
Button::Button(ButtonFactory& factory, string styleID, const vector<string>& font_h,const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam)
: name(nam) {//tworzenie przyciku- wymaga dodanie fabryki przycisków

        param = factory.getOrCreate(styleID, res, col);//style id to nazwa jak class w css
        extractPosition(res);

}
void Button::extractPosition(const vector<string>& res) {
    for (auto i : res) {
        vector<string> k = split_manual(i, ":");
        if (k[0] == "position-x") posx = k[1];//pozycja przycisku w X
        if (k[0] == "position-y") posy = k[1];
        if (k[0]=="font-size") fontsize=k[1];//rozmiar czionki
        if (k[0]=="font-name") font=k[1];//path czcionki
        if (k[0]=="font") font_color=f_HTML(k[1]); //kolor fontu w html
        if (k[0]=="font-shadow") font_shadow_color=f_HTML(k[1]); //kolor cienia fontu w html
    }
}

void Button::generateFont() {
    ALLEGRO_FONT* font_1=al_load_ttf_font(font.c_str(),actual_value(fontsize),0);
    int w = al_get_text_width(font_1, name.c_str());
    int h = al_get_font_line_height(font_1);
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
    ALLEGRO_BITMAP* bi=al_create_bitmap(w,h,4);

}

Button::~Button() {

}
void AllegroImageDeleter(ButtonImage* bi) {
    if (bi) {
        if (bi->normal)  al_destroy_bitmap(bi->normal);
        if (bi->hover)   al_destroy_bitmap(bi->hover);
        if (bi->pressed) al_destroy_bitmap(bi->pressed);
        delete bi; // Na końcu usuwamy samą strukturę
    }
}
shared_ptr<ButtonParameters> ButtonFactory::getOrCreate(string id, const vector<string> &res, const vector<ALLEGRO_COLOR> &col) {
    //Dla starego id
    if (styles.count(id)) {
        if (!res.empty() || !col.empty()) {
            updateParams(styles[id], res, col);
        }
        return styles[id];
    }
    //Dla nowego id
    auto p = make_shared<ButtonParameters>();
    p->images = shared_ptr<ButtonImage>(new ButtonImage(), AllegroImageDeleter);

    updateParams(p, res, col);
    styles[id] = p;
    return p;
}
void ButtonFactory::updateParams(shared_ptr<ButtonParameters> p, const vector<string>& res, const vector<ALLEGRO_COLOR>& col) {
    for (const auto& i : res){
        vector<string> k= split_manual(i,":");
        if (k[0]=="width"){
            p->width=k[1];
        }else if (k[0]=="min-width"){
            p->minwidth=k[1];
        }else if (k[0]=="max-width") {
            p->maxwidth=k[1];
        }else if (k[0]=="height") {
            p->height=k[1];
        }else if (k[0]=="max-height") {
            p->maxheight=k[1];
        }else if (k[0]=="border-radius") {
            p->border_radius=k[1];
        }else if (k[0]=="border-thickness") {
            p->border_thickness=k[1];
        }else if (k[0]=="shadow-offset-x") {
            p->shadow_offset_x=k[1];
        }else if (k[0]=="shadow-offset-y") {
            p->shadow_offset_y=k[1];
        }
    }
    // Aktualizacja kolorów tylko jeśli wektor nie jest pusty
    if (col.size() >= 5) {
        p->button = col[0];
        p->border_normal = col[1];
        p->border_hover = col[2];
        p->border_active = col[3];
        p->shadow_color = col[4];
    }
}