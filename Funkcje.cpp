//
// Created by karol on 23.01.2026.
//


#include "klasy.hpp"
int actual_value(const string &value) {//funkcja zmieniająca wartości typowo html na piksele
    if (value=="") return 0;
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
        extractPosition(font_h);
        generateFont();

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
void AllegroGaussFilter(ALLEGRO_BITMAP* Source, ALLEGRO_BITMAP* Target, int w, int h){
    int t[3][3]={{1,2,1},{2,4,2},{1,2,1}};
    ALLEGRO_LOCKED_REGION *reg = al_lock_bitmap(Target, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
    for(int j=1;j<h-1;j++){
        uint8_t *ptr = (uint8_t *)reg->data + j * reg->pitch;
        ptr+=4;
        for(int i=1; i<w-1; i++){
            int r=0;int g=0; int b=0;int a=0;
            for (int k=-1;k<2;k++){
                for(int l=-1;l<2;l++){
                    ALLEGRO_COLOR bl= al_get_pixel(Source,i+k,j+l);
                    r+=bl.r*t[i][j];
                    g+=bl.g*t[i][j];
                    b+=bl.b*t[i][j];
                    a=(bl.a>a ? bl.a : a);
                }
            }
            r/=12;g/=12;b/=12;
            *((uint32_t *)ptr) = (r | (g << 8) | (b << 16) | (a << 24));
            ptr+=4;
        }
    }
    al_unlock_bitmap(Target);
}
void Button::generateFont() {
    ALLEGRO_FONT* font_1=al_load_ttf_font(font.c_str(),actual_value(fontsize),0);
    int w = al_get_text_width(font_1, name.c_str());
    int h = al_get_font_line_height(font_1);
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
    int w_offset=actual_value(param->shadow_offset_x);
    w_offset+=(w_offset==0 ? 0 : (w_offset<0 ? -1 : 1));
    int h_offset=actual_value(param->shadow_offset_y);
    h_offset+=(h_offset==0 ? 0 : (h_offset<0 ? -1 : 1));
    ALLEGRO_BITMAP* bi=nullptr;
    if(w_offset!=0 || h_offset!=0) {
        bi = al_create_bitmap(w + abs(w_offset), h + abs(h_offset));
        al_set_target_bitmap(bi);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        al_draw_text(font_1, font_shadow_color,(w_offset>0 ? w_offset : 0 ),(h_offset>0 ? h_offset :0),ALLEGRO_ALIGN_LEFT,name.c_str());
    }
    ShadowFont=al_create_bitmap(w+abs(w_offset) ,h+ abs(h_offset));
    AllegroGaussFilter(bi,ShadowFont,w+abs(w_offset),h+abs(h_offset));
    al_destroy_bitmap(bi);
    al_set_target_bitmap(ShadowFont);
    al_draw_text(font_1, font_color,(w_offset>0 ? 0 : -w_offset ),(h_offset>0 ? 0 : -h_offset),ALLEGRO_ALIGN_LEFT,name.c_str());
    al_destroy_font(font_1);
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
    createRectangle(p);
}
void ButtonFactory::createRectangle(shared_ptr<ButtonParameters> p) {

    int w= actual_value(p->width);
    int p_w= actual_value(p->minwidth);
    int m_w= actual_value(p->maxwidth);
    int h= actual_value(p->height);
    int p_h= actual_value(p->minheight);
    int m_h= actual_value(p->maxheight);
    w=(w<p_w && p_w<=m_w ? p_w :(w>m_w ? m_w : w) );
    h=(h<p_h && p_h<=m_h ? p_h :(h>m_h ? m_h : h) );
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
    int thic= actual_value(p->border_thickness);
    int w_offset=actual_value(p->shadow_offset_x);
    w_offset+=(w_offset==0 ? 0 : (w_offset<0 ? -1 : 1));
    int h_offset=actual_value(p->shadow_offset_y);
    h_offset+=(h_offset==0 ? 0 : (h_offset<0 ? -1 : 1));
    vector<ALLEGRO_BITMAP*>dar;
    dar.push_back(p->images->normal);
    dar.push_back(p->images->hover);
    dar.push_back(p->images->pressed);
    ALLEGRO_BITMAP* bi=nullptr;
    for(int i=0; i<3;i++) {
        if (dar[i]){
            al_destroy_bitmap(dar[i]);
        }
    }
    for(int i=0; i<3;i++){
        dar[i]=al_create_bitmap(w + abs(w_offset)+2*thic, h + abs(h_offset)+2*thic);
        al_set_target_bitmap(dar[i]);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    }
    if(w_offset!=0 || h_offset!=0) {
        bi = al_create_bitmap(w + abs(w_offset)+2*thic, h + abs(h_offset)+2*thic);
        al_set_target_bitmap(bi);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        int b_r=actual_value(p->border_radius);
        al_draw_filled_rounded_rectangle((w_offset>0 ? w_offset:0),(h_offset>0 ? h_offset:0),
                                         w+2*thic+(w_offset>0 ? w_offset:0),h+2*thic+(h_offset>0 ? h_offset:0),
                                         b_r+(b_r>0 ? thic : 0),b_r+(b_r>0 ? thic : 0),p->shadow_color);
        for(int i=0; i<3;i++){
            AllegroGaussFilter(bi,dar[i],w+abs(w_offset)+2*thic,h+abs(h_offset)+2*thic);
        }
        al_destroy_bitmap(bi);
        vector<ALLEGRO_COLOR>mar;
        mar.push_back(p->border_normal);
        mar.push_back(p->border_hover);
        mar.push_back(p->border_active);
        for(int i=0;i<3;i++){
            al_set_target_bitmap(dar[i]);
            al_draw_filled_rounded_rectangle((w_offset>0 ? 0 : -w_offset),(h_offset>0 ? 0: -h_offset),
                                             w+2*thic+(w_offset>0 ? 0 : -w_offset),h+2*thic+(h_offset>0 ? 0 : -h_offset),
                                             b_r+(b_r>0 ? thic : 0),b_r+(b_r>0 ? thic : 0),mar[i]);
            al_draw_rounded_rectangle((w_offset>0 ? 0 : -w_offset),(h_offset>0 ? 0: -h_offset),
                                       w+2*thic+(w_offset>0 ? 0 : -w_offset),h+2*thic+(h_offset>0 ? 0 : -h_offset),
                                       b_r+(b_r>0 ? thic : 0),b_r+(b_r>0 ? thic : 0), f_HTML("#000000FF"),1.0f);
            al_draw_filled_rounded_rectangle((w_offset>0 ? 0 : -w_offset)+thic,(h_offset>0 ? 0: -h_offset)+thic,
                                             w+thic+(w_offset>0 ? 0 : -w_offset),h+thic+(h_offset>0 ? 0 : -h_offset),
                                             b_r,b_r,p->button);
            al_draw_rounded_rectangle((w_offset>0 ? 0 : -w_offset)+thic,(h_offset>0 ? 0: -h_offset)+thic,
                                             w+thic+(w_offset>0 ? 0 : -w_offset),h+thic+(h_offset>0 ? 0 : -h_offset),
                                             b_r,b_r,f_HTML("#000000FF"),1.0f);
        }
    }
}