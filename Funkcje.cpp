//
// Created by karol on 23.01.2026.
//


#include "klasy.hpp"
void AllegroGaussFilter(ALLEGRO_BITMAP* Source, ALLEGRO_BITMAP* Target, int w, int h) {
    // Wymuszamy format 32-bitowy, aby przesunięcia bitowe (<<) działały przewidywalnie
    ALLEGRO_LOCKED_REGION *src_reg = al_lock_bitmap(Source, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_READONLY);
    ALLEGRO_LOCKED_REGION *dst_reg = al_lock_bitmap(Target, ALLEGRO_PIXEL_FORMAT_ABGR_8888_LE, ALLEGRO_LOCK_WRITEONLY);

    int t[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
    int div = 16; // Suma wag macierzy

    for (int j = 1; j < h - 1; j++) {
        // Obliczamy wskaźnik do początku wiersza w obu bitmapach
        uint32_t *dst_ptr = (uint32_t*)((uint8_t*)dst_reg->data + j * dst_reg->pitch);

        for (int i = 1; i < w - 1; i++) {
            int r = 0, g = 0, b = 0, a = 0;

            for (int ky = -1; ky <= 1; ky++) {
                // Wskaźnik do wiersza źródłowego dla splotu
                uint32_t *src_row = (uint32_t*)((uint8_t*)src_reg->data + (j + ky) * src_reg->pitch);
                for (int kx = -1; kx <= 1; kx++) {
                    uint32_t pixel = src_row[i + kx];
                    int weight = t[kx + 1][ky + 1];

                    // Wyciąganie składowych z formatu ABGR_8888_LE
                    r += (pixel & 0xFF) * weight;
                    g += ((pixel >> 8) & 0xFF) * weight;
                    b += ((pixel >> 16) & 0xFF) * weight;
                    a += ((pixel >> 24) & 0xFF) * weight;
                }
            }

            // Zapisujemy przetworzony piksel (dzielenie przez sumę wag)
            dst_ptr[i] = (r / div) | ((g / div) << 8) | ((b / div) << 16) | ((a / div) << 24);
        }
    }

    al_unlock_bitmap(Source);
    al_unlock_bitmap(Target);
}

int actual_value(const string &value) {
    if (value.empty()) return 0;

    if (value.find("vh") != string::npos) {
        return static_cast<int>(
                std::stof(split_manual(value, "vh")[0]) / 100.0f * screen_height
        );
    }
    else if (value.find("vw") != string::npos) {
        return static_cast<int>(
                std::stof(split_manual(value, "vw")[0]) / 100.0f * screen_width
        );
    }
    else if (value.find("px") != string::npos) {
        return std::stoi(split_manual(value, "px")[0]);
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
Atom::~Atom(){
    if (Font!=nullptr) al_destroy_font(Font);
}
Atom::Atom(ButtonFactory& Factory,
           string styleID,
           const vector<string>& font_h,
           const vector<string>& res,
           const vector<ALLEGRO_COLOR>& col,
           string nam,int typ): name(nam){
    param = Factory.getOrCreate(styleID, typ,res, col);//style id to nazwa jak class w css
    extractPosition(font_h);
    generateFont();
}
Button::Button(ButtonFactory& Factory, string styleID,
               const vector<string>& font_h, const vector<string>& res,
               const vector<ALLEGRO_COLOR>& col, string nam,int typ) :
        Atom(Factory, styleID, font_h, res, col, nam,typ){}
Atom::Atom(ButtonFactory& Factory,
     string styleID,
     const vector<string>& font_h,
     const vector<string>& res,
     const vector<ALLEGRO_COLOR>& col,
     string nam) : Atom(Factory,styleID,font_h,res,col,nam,Pierwiastek){

}
Button::Button(ButtonFactory& factory, string styleID, const vector<string>& font_h,const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam)
: Atom(factory,styleID,font_h,res,col,nam,Przycisk) {//tworzenie przyciku- wymaga dodanie fabryki przycisków
}
TriangleButton::TriangleButton(ButtonFactory& factory, string styleID, const vector<string>& font_h,const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam)
        : Button(factory,styleID,font_h,res,col, split_manual(nam,":")[0],(split_manual(nam,":")[1]=="D" ? TriangleD : TriangleU)) {//tworzenie przyciku- wymaga dodanie fabryki przycisków
}
Atom::Atom(const Atom& Inny, ButtonFactory& factory,const string nazwa, const string pos_x, const string pos_y):
        name(nazwa),
        param(Inny.param),
        posx(pos_x),
        posy(pos_y),
        fontsize(Inny.fontsize),
        font(Inny.font),
        fontmaxwidth(Inny.fontmaxwidth),
        fontminwidth(Inny.fontminwidth),
        font_color(Inny.font_color),
        font_shadow_color(Inny.font_shadow_color)
{
    generateFont();
}
Button::Button(const Button& Inny, ButtonFactory& factory,string nazwa, string pos_x, string pos_y):
    Atom(Inny,factory,nazwa,pos_x,pos_y)
{}
TriangleButton::TriangleButton(const TriangleButton& Inny, ButtonFactory& factory,string nazwa, string pos_x, string pos_y):
        Button(Inny,factory,nazwa,pos_x,pos_y)
    {}
void Atom::extractPosition(const vector<std::string> &res) {
    extractPositionH(res);
}
void Atom::extractPositionH(const vector<string>& res) {
    for (auto i : res) {
        vector<string> k = split_manual(i, ":");
        if (k[0] == "position-x") posx = k[1];//pozycja przycisku w X
        if (k[0] == "position-y") posy = k[1];
        if (k[0]=="font-size") fontsize=k[1];//rozmiar czcionki
        if (k[0]=="font-name") font=k[1];//path czcionki
        if (k[0]=="font") font_color=f_HTML(k[1]); //kolor fontu w html
        if (k[0]=="font-shadow") font_shadow_color=f_HTML(k[1]); //kolor cienia fontu w html
        if (k[0]=="font-maxwidth") fontmaxwidth=k[1];
        if (k[0]=="font-minwidth") fontminwidth=k[1];
    }
}
ButtonFactory::ButtonFactory() {

}
bool BakeFontToMemoryBitmap(
        ALLEGRO_BITMAP* dest,
        ALLEGRO_FONT* font,
        const std::string& text,
        ALLEGRO_COLOR color,
        int x,
        int y
) {
    if (!dest || !font) return false;

    if (!al_get_current_display()) return false;

    int w = al_get_bitmap_width(dest);
    int h = al_get_bitmap_height(dest);
    if (w <= 0 || h <= 0) return false;

    ALLEGRO_BITMAP* prev = al_get_target_bitmap();
    int old_flags = al_get_new_bitmap_flags();
    // VIDEO temp
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    ALLEGRO_BITMAP* video = al_create_bitmap(w, h);
    if (!video) return false;

    al_set_target_bitmap(video);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    al_draw_text(font, color, x, y, 0, text.c_str());

    // copy → MEMORY
    al_set_target_bitmap(dest);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_draw_bitmap(video, 0, 0, 0);

    al_set_target_bitmap(prev);
    al_set_new_bitmap_flags(old_flags);
    al_destroy_bitmap(video);

    return true;
}
void Atom::generateFont() {
    generateFontH();
}
void Atom::generateFontH() {
    int fontsizer=actual_value(fontsize);
    if (fontmaxwidth=="") fontmaxwidth="100vh";
    int w;
    Start:
    if(Font) al_destroy_font(Font);
    Font=al_load_ttf_font(font.c_str(),fontsizer,0);
    if (!Font)
    {cout<<"Nie udało się załadować fontu\n";
        return;}  // nie udało się załadować
    w=al_get_text_width(Font,name.c_str());
    if (w>actual_value(fontmaxwidth) && w<actual_value(fontminwidth)) {
        return;
    }else if (w>actual_value(fontmaxwidth)) {
        fontsizer--;
        goto Start;
    }else if (w<actual_value(fontminwidth)) {
        fontsizer++;
        goto Start;
    }
}
void Button::hover(){
    tryb[0]=true;
    tryb[1]=false;
};
void Button::pressed(){
    tryb[0]=true;
    tryb[1]=true;
}
void Button::normal(){
    tryb[0]=false;
    tryb[1]=false;
}
void Button::take_event() {
    checkevent();
}
void Button::draw(ALLEGRO_COLOR  color){
    int posix,posiy;
    posix= actual_value(posx);
    posiy= actual_value(posy);
    int w= actual_value(param->width);
    int p_w= actual_value(param->minwidth);
    int m_w= actual_value(param->maxwidth);
    int h= actual_value(param->height);
    int p_h= actual_value(param->minheight);
    int m_h= actual_value(param->maxheight);

    w=(w<p_w && p_w<=m_w ? p_w : w);
    w=(w>m_w ? m_w : w);
    h=(h<p_h && p_h<=m_h ? p_h :h);
    h=(h>m_h ? m_h : h);
    al_draw_filled_rectangle(posix-w/2,posiy-h/2,posix+w/2,posiy+h/2,color);
}
void TriangleButton::draw(ALLEGRO_COLOR color){
    int posix,posiy;
    posix= actual_value(posx);
    posiy= actual_value(posy);
    int w= actual_value(param->width);
    int p_w= actual_value(param->minwidth);
    int m_w= actual_value(param->maxwidth);
    int h= actual_value(param->height);
    int p_h= actual_value(param->minheight);
    int m_h= actual_value(param->maxheight);
    w=(w<p_w && p_w<=m_w ? p_w : w);
    w=(w>m_w ? m_w : w);
    h=(h<p_h && p_h<=m_h ? p_h :h);
    h=(h>m_h ? m_h : h);
    bool warunek=param->typ==TriangleU;
    al_draw_filled_triangle((warunek ? posix-w/2 : posix+w/2),
                            (warunek ? posiy-h/2 : posiy+h/2),
                            posix+w/2,
                            posiy-h/2,
                            posix-w/2,
                            posiy+h/2,
                            color);
}
void Atom::build() {
    buildH();
}
void Atom::buildH(){
    int posix,posiy;
    int k,l,off_y,off_x;
    k=al_get_bitmap_width(param->images->normal);
    l=al_get_bitmap_height(param->images->normal);
    off_x=actual_value(param->shadow_offset_x);
    off_y=actual_value(param->shadow_offset_y);
    posix=actual_value(posx)-(k-abs(off_x))/2;
    posix+=(off_x>0 ? 0 : off_x);
    posiy=actual_value(posy)-(l-abs(off_y))/2;
    posiy+=(off_y>0 ? 0 : off_y);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_draw_bitmap( param->images->normal,posix, posiy,0);
    int w=al_get_text_width(Font,name.c_str());
    int h=al_get_font_line_height(Font);
    al_draw_text(Font, font_shadow_color,actual_value(posx)-0.5*w+actual_value(param->shadow_offset_x),actual_value(posy)-0.5*h+actual_value(param->shadow_offset_y),ALLEGRO_ALIGN_LEFT,name.c_str());
    al_draw_text(Font, font_color,actual_value(posx)-0.5*w,actual_value(posy)-0.5*h,ALLEGRO_ALIGN_LEFT,name.c_str());
}
void Button::buildH() {
    int posix,posiy;
    int k,l,off_y,off_x;
    k=al_get_bitmap_width(param->images->normal);
    l=al_get_bitmap_height(param->images->normal);
    off_x=actual_value(param->shadow_offset_x);
    off_y=actual_value(param->shadow_offset_y);
    posix=actual_value(posx)-(k-abs(off_x))/2;
    posix+=(off_x>0 ? 0 : off_x);
    posiy=actual_value(posy)-(l-abs(off_y))/2;
    posiy+=(off_y>0 ? 0 : off_y);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_draw_bitmap((!tryb[0] && !tryb[1] ? param->images->normal:(tryb[0] && !tryb[1] ? param->images->hover : param->images->pressed)),
                   posix, posiy,0);
    int w=al_get_text_width(Font,name.c_str());
    int h=al_get_font_line_height(Font);
    al_draw_text(Font, font_shadow_color,actual_value(posx)-0.5*w+actual_value(param->shadow_offset_x),actual_value(posy)-0.5*h+actual_value(param->shadow_offset_y),ALLEGRO_ALIGN_LEFT,name.c_str());
    al_draw_text(Font, font_color,actual_value(posx)-0.5*w,actual_value(posy)-0.5*h,ALLEGRO_ALIGN_LEFT,name.c_str());
    /*
    posix=actual_value(posx)-(al_get_bitmap_width(ShadowFont)-actual_value(param->shadow_offset_x))/2;
    posix+=(actual_value(param->shadow_offset_x)>0 ? 0 : actual_value(param->shadow_offset_x));
    posiy=actual_value(posy)-(al_get_bitmap_height(ShadowFont)-actual_value(param->shadow_offset_y))/2;
    posiy+=(actual_value(param->shadow_offset_y)>0 ? 0 : actual_value(param->shadow_offset_y));
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_draw_bitmap(ShadowFont, posix, posiy,0);*/
}
void TriangleButton::buildH() {
    int posix,posiy;
    int k,l,off_y,off_x;
    k=al_get_bitmap_width(param->images->normal);
    l=al_get_bitmap_height(param->images->normal);
    off_x=actual_value(param->shadow_offset_x);
    off_y=actual_value(param->shadow_offset_y);
    posix=actual_value(posx)-(k-abs(off_x))/2;
    posix+=(off_x>0 ? 0 : off_x);
    posiy=actual_value(posy)-(l-abs(off_y))/2;
    posiy+=(off_y>0 ? 0 : off_y);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    al_draw_bitmap((!tryb[0] && !tryb[1] ? param->images->normal:(tryb[0] && !tryb[1] ? param->images->hover : param->images->pressed)),
                   posix, posiy,0);
    int w=al_get_text_width(Font,name.c_str());
    int h=al_get_font_line_height(Font);
    int center_x=actual_value(posx);
    int center_y=actual_value(posy);
    int next_x=(center_x-posix+(off_x>0 ? 0 : abs(off_x))-4* actual_value(param->border_thickness))/2;
    int next_y=(center_y-posiy+(off_y>0?0:abs(off_y))-4* actual_value(param->border_thickness))/2;
    al_draw_text(Font, font_shadow_color,center_x+(param->typ==TriangleU ? -next_x : next_x)+(param->typ==TriangleU ? -1:1)*0.5*w+actual_value(param->shadow_offset_x),center_y+(param->typ==TriangleU ? -next_y : next_y)-0.5*h+actual_value(param->shadow_offset_y),(param->typ==TriangleU ? ALLEGRO_ALIGN_LEFT:ALLEGRO_ALIGN_RIGHT),name.c_str());
    al_draw_text(Font, font_color,center_x+(param->typ==TriangleU ? -next_x : next_x)+(param->typ==TriangleU ? -1:1)*0.5*w,center_y+(param->typ==TriangleU ? -next_y : next_y)-0.5*h,(param->typ==TriangleU ? ALLEGRO_ALIGN_LEFT:ALLEGRO_ALIGN_RIGHT),name.c_str());
}
void AllegroImageDeleter(ButtonImage* bi) {
    if (bi) {
        if (bi->normal)  al_destroy_bitmap(bi->normal);
        if (bi->hover)   al_destroy_bitmap(bi->hover);
        if (bi->pressed) al_destroy_bitmap(bi->pressed);
        delete bi; // Na końcu usuwamy samą strukturę
    }
}
shared_ptr<ButtonParameters> ButtonFactory::getOrCreate(string id,int typ, const vector<string> &res, const vector<ALLEGRO_COLOR> &col) {
    //Dla starego id
    if (styles.count(id)) {
        if (!res.empty() || !col.empty()) {
            updateParams(styles[id], res, col,typ);
        }
        return styles[id];
    }
    //Dla nowego id
    auto p = make_shared<ButtonParameters>();
    p->images = shared_ptr<ButtonImage>(new ButtonImage(), AllegroImageDeleter);
    updateParams(p, res, col,typ);
    styles[id] = p;
    return p;
}
void ButtonFactory::updateParams(shared_ptr<ButtonParameters> p, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, int typ) {
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
        }else if (k[0]=="min-height") {
            p->minheight=k[1];
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
        p->typ=typ;
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
void ButtonFactory::ReCreateRectangle() {
    const size_t maxThreads = 4;  // liczba wątków
    std::vector<std::shared_ptr<ButtonParameters>> batch;
    batch.reserve(maxThreads);

    auto it = styles.begin();
    while (it != styles.end()) {
        // Zbieramy partię do 4 przycisków
        batch.clear();
        for (size_t i = 0; i < maxThreads && it != styles.end(); ++i, ++it) {
            batch.push_back(it->second);
        }

        // Tworzymy wątki dla tej partii
        std::vector<std::thread> threads;
        for (auto& param : batch) {
            threads.emplace_back([param, this]() {
                createRectangle(param);
            });
        }

        // Czekamy na zakończenie wszystkich wątków
        for (auto& t : threads) {
            t.join();
        }
    }
}



void ButtonFactory::createRectangle(shared_ptr<ButtonParameters> p) {
    if (p->images->normal) {
        al_destroy_bitmap(p->images->normal);
        p->images->normal= nullptr;
    }
    if (p->images->hover) {
        al_destroy_bitmap(p->images->hover);
        p->images->hover= nullptr;
    }
    if (p->images->hover) {
        al_destroy_bitmap(p->images->pressed);
        p->images->pressed= nullptr;
    }
    int w= actual_value(p->width);
    int p_w= actual_value(p->minwidth);
    int m_w= actual_value(p->maxwidth);
    int h= actual_value(p->height);
    int p_h= actual_value(p->minheight);
    int m_h= actual_value(p->maxheight);
    w=(w<p_w && p_w<=m_w ? p_w : w);
    w=(w>m_w ? m_w : w);
    h=(h<p_h && p_h<=m_h ? p_h :h);
    h=(h>m_h ? m_h : h);
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    al_set_new_bitmap_format(ALLEGRO_PIXEL_FORMAT_RGBA_8888);
    int thic= actual_value(p->border_thickness);
    int w_offset=actual_value(p->shadow_offset_x);
    w_offset+=(w_offset==0 ? 0 : (w_offset<0 ? -1 : 1));
    int h_offset=actual_value(p->shadow_offset_y);
    h_offset+=(h_offset==0 ? 0 : (h_offset<0 ? -1 : 1));
    vector<ALLEGRO_BITMAP*>dar;

    dar.push_back(p->images->normal);
    bool warunek=p->typ==Przycisk;
    bool warunek1=p->typ==Przycisk || p->typ==Pierwiastek;
    bool warunek2=p->typ==Przycisk || p->typ==TriangleD || p->typ==TriangleU;
    bool warunek3= p->typ==TriangleD || p->typ==TriangleU;
    bool warunek4=p->typ==TriangleD;
    bool warunek5= p->typ==TriangleU;
    if (warunek2) {
        dar.push_back(p->images->hover);
        dar.push_back(p->images->pressed);
    }
    ALLEGRO_BITMAP* bi=nullptr;
    for(int i=0; i<dar.size();i++){
        dar[i]=al_create_bitmap(w + abs(w_offset)+2*thic, h + abs(h_offset)+2*thic);
        al_set_target_bitmap(dar[i]);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    }
    if(w_offset!=0 || h_offset!=0) {
        //cout<<w_offset << " " << h_offset;
        bi = al_create_bitmap(w + abs(w_offset)+2*thic, h + abs(h_offset)+2*thic);
        al_set_target_bitmap(bi);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        int b_r=actual_value(p->border_radius);
        if (warunek1) {
            al_draw_filled_rounded_rectangle((w_offset > 0 ? w_offset : 0), (h_offset > 0 ? h_offset : 0),
                                             w + 2 * thic + (w_offset > 0 ? w_offset : 0),
                                             h + 2 * thic + (h_offset > 0 ? h_offset : 0),
                                             b_r + (b_r > 0 ? thic : 0), b_r + (b_r > 0 ? thic : 0), p->shadow_color);
        }
        else if(warunek3){
            al_draw_filled_triangle((warunek5 ? (w_offset > 0 ? w_offset : 0):w + 2 * thic + (w_offset > 0 ? w_offset : 0)),
                                    (warunek5 ? (h_offset > 0 ? h_offset : 0) : h + 2 * thic + (h_offset > 0 ? h_offset : 0)),
                                    w + 2 * thic + (w_offset > 0 ? w_offset : 0),
                                    (h_offset > 0 ? h_offset : 0),
                                    (w_offset > 0 ? w_offset : 0),
                                    h + 2 * thic + (h_offset > 0 ? h_offset : 0),
                                    p->shadow_color);
        }
        for(int i=0; i<dar.size();i++){
            AllegroGaussFilter(bi,dar[i],w+abs(w_offset)+2*thic,h+abs(h_offset)+2*thic);
        }
        al_destroy_bitmap(bi);
        vector<ALLEGRO_COLOR>mar;
        mar.push_back(p->border_normal);
        if(warunek2) {
            mar.push_back(p->border_hover);
            mar.push_back(p->border_active);
        }
        for(int i=0;i<dar.size();i++){
            al_set_target_bitmap(dar[i]);
            if(warunek1) {
                al_draw_filled_rounded_rectangle((w_offset > 0 ? 0 : -w_offset), (h_offset > 0 ? 0 : -h_offset),
                                                 w + 2 * thic + (w_offset > 0 ? 0 : -w_offset),
                                                 h + 2 * thic + (h_offset > 0 ? 0 : -h_offset),
                                                 b_r + (b_r > 0 ? thic : 0), b_r + (b_r > 0 ? thic : 0), mar[i]);
                al_draw_rounded_rectangle((w_offset > 0 ? 0 : -w_offset), (h_offset > 0 ? 0 : -h_offset),
                                          w + 2 * thic + (w_offset > 0 ? 0 : -w_offset),
                                          h + 2 * thic + (h_offset > 0 ? 0 : -h_offset),
                                          b_r + (b_r > 0 ? thic : 0), b_r + (b_r > 0 ? thic : 0), f_HTML("#000000FF"),
                                          1.0f);
                al_draw_filled_rounded_rectangle((w_offset > 0 ? 0 : -w_offset) + thic,
                                                 (h_offset > 0 ? 0 : -h_offset) + thic,
                                                 w + thic + (w_offset > 0 ? 0 : -w_offset),
                                                 h + thic + (h_offset > 0 ? 0 : -h_offset),
                                                 b_r, b_r, p->button);
                al_draw_rounded_rectangle((w_offset > 0 ? 0 : -w_offset) + thic, (h_offset > 0 ? 0 : -h_offset) + thic,
                                          w + thic + (w_offset > 0 ? 0 : -w_offset),
                                          h + thic + (h_offset > 0 ? 0 : -h_offset),
                                          b_r, b_r, f_HTML("#000000FF"), 1.0f);
            }
            if(warunek3){
                al_draw_filled_triangle((warunek5 ? (w_offset > 0 ? 0 : -w_offset) : w + 2 * thic + (w_offset > 0 ? 0 : -w_offset)),
                                        (warunek5 ? (h_offset > 0 ? 0 : -h_offset):h + 2 * thic + (h_offset > 0 ? 0 : -h_offset)),
                                        w + (warunek5 ? 4 * thic:2*thic) +(w_offset > 0 ? 0 : -w_offset),
                                        (h_offset > 0 ? 0 : -h_offset)+(warunek5 ? 0:-2*thic),
                                        (w_offset > 0 ? 0 : -w_offset)+(warunek5 ? 0:-2*thic),
                                        h + (warunek5 ? 4 * thic:2*thic) + (h_offset > 0 ? 0 : -h_offset),mar[i]);
                al_draw_triangle((warunek5 ? (w_offset > 0 ? 0 : -w_offset) : w + 2 * thic + (w_offset > 0 ? 0 : -w_offset)),
                                 (warunek5 ? (h_offset > 0 ? 0 : -h_offset):h + 2 * thic + (h_offset > 0 ? 0 : -h_offset)),
                                 w + (warunek5 ? 4 * thic:2*thic) +(w_offset > 0 ? 0 : -w_offset),
                                 (h_offset > 0 ? 0 : -h_offset)+(warunek5 ? 0:-2*thic),
                                 (w_offset > 0 ? 0 : -w_offset)+(warunek5 ? 0:-2*thic),
                                 h + (warunek5 ? 4 * thic:2*thic) + (h_offset > 0 ? 0 : -h_offset),f_HTML("#000000FF"),1.0f);
                al_draw_filled_triangle((warunek5 ? (w_offset > 0 ? 0 : -w_offset)+thic : w +  thic + (w_offset > 0 ? 0 : -w_offset)),
                                        (warunek5 ? (h_offset > 0 ? 0 : -h_offset)+thic:h +thic + (h_offset > 0 ? 0 : -h_offset)),
                                        w + thic +(w_offset > 0 ? 0 : -w_offset),
                                        (h_offset > 0 ? 0 : -h_offset)+thic,
                                        (w_offset > 0 ? 0 : -w_offset)+thic,
                                        h + thic + (h_offset > 0 ? 0 : -h_offset),p->button);
                al_draw_triangle((warunek5 ? (w_offset > 0 ? 0 : -w_offset)+thic : w +  thic + (w_offset > 0 ? 0 : -w_offset)),
                                        (warunek5 ? (h_offset > 0 ? 0 : -h_offset)+thic:h +thic + (h_offset > 0 ? 0 : -h_offset)),
                                        w + thic +(w_offset > 0 ? 0 : -w_offset),
                                        (h_offset > 0 ? 0 : -h_offset)+thic,
                                        (w_offset > 0 ? 0 : -w_offset)+thic,
                                        h + thic + (h_offset > 0 ? 0 : -h_offset),f_HTML("#000000FF"), 1.0f);
            }
        }

    }
    p->images->normal=dar[0];
    if(warunek2) {
        p->images->hover = dar[1];
        p->images->pressed = dar[2];
    }
}
Page::Page(){
    przyciski=nullptr;
    aktualny_klucz=1;
    aktywny_przycisk= 0;
}
void Page::ReloadFont() {
    const size_t maxThreads = 4;
    std::vector<Atom*> batch;  // wskaźniki do obiektów w unique_ptr
    batch.reserve(maxThreads);

    auto it = buttons.begin();
    while (it != buttons.end()) {
        batch.clear();

        // Tworzymy batch do 4 przycisków
        for (size_t i = 0; i < maxThreads && it != buttons.end(); ++i, ++it) {
            batch.push_back(it->second.get());  // get() daje wskaźnik surowy
        }

        // Uruchamiamy wątki dla tej partii
        std::vector<std::thread> threads;
        for (auto* przycisk : batch) {
            threads.emplace_back([przycisk]() {
                przycisk->generateFont();
            });
        }

        // Czekamy na zakończenie wszystkich wątków
        for (auto& t : threads) {
            t.join();
        }
    }
}

void Page::addButton(ButtonFactory &factory,const Atom & Inny, const string nazwa,const string pos_x, const string pos_y) {
    buttons[aktualny_klucz] = Inny.clone(factory, nazwa, pos_x, pos_y);
    aktualny_klucz=(aktualny_klucz==255 ? 1 : aktualny_klucz+1);
}
void Page::buildButtons(ALLEGRO_DISPLAY *obraz) {
    for (auto const& [klucz, przycisk] : buttons) {
        przycisk->build();
    }
    al_set_target_backbuffer(obraz);
}
bool Page::hover(int x, int y ){
    ALLEGRO_COLOR punkt=al_get_pixel(przyciski,x,y);
    unsigned char r, g, b, a;
    al_unmap_rgba(punkt, &r, &g, &b, &a);
    int ri=(int)r;
    //cout << ri << "\n";
    if (ri!=aktywny_przycisk) {
        if (aktywny_przycisk!=0) {
            buttons[aktywny_przycisk]->normal();
        }
        aktywny_przycisk=ri;
        if (aktywny_przycisk!=0) {
            buttons[aktywny_przycisk]->hover();
        }
        return true;
    }
    return false;
}
/* void Page::createBitmap() {//wersja sekwencyjna
    ALLEGRO_BITMAP * old=al_get_target_bitmap();
    if (przyciski) {
        al_destroy_bitmap(przyciski);
        przyciski=nullptr;
    }
    przyciski=al_create_bitmap(screen_width,screen_height);
    al_set_target_bitmap(przyciski);
    al_clear_to_color((al_map_rgba(0,0,0,255)));
    for (auto const& [klucz, przycisk] : buttons) {
        przycisk->draw(al_map_rgba(klucz,0,0,255));
    }
    al_set_target_bitmap(old);
}*/
void Page::createBitmap() {

    ALLEGRO_BITMAP* old = al_get_target_bitmap();

    if (przyciski) {
        al_destroy_bitmap(przyciski);
        przyciski = nullptr;
    }

    przyciski = al_create_bitmap(screen_width, screen_height);

    const size_t maxThreads = 4;

    // 🔹 zbieramy dane: Atom* + klucz
    struct Task {
        Atom* atom;
        int key;
    };

    std::vector<Task> tasks;
    tasks.reserve(buttons.size());

    for (auto const& [klucz, przycisk] : buttons) {
        tasks.push_back({przycisk.get(), klucz});
    }

    size_t total = tasks.size();
    size_t chunkSize = (total + maxThreads - 1) / maxThreads;

    std::vector<ALLEGRO_BITMAP*> threadBitmaps(maxThreads, nullptr);
    std::vector<std::thread> threads;

    for (size_t t = 0; t < maxThreads; ++t) {

        size_t begin = t * chunkSize;
        size_t end = std::min(begin + chunkSize, total);

        if (begin >= end)
            break;

        threads.emplace_back([=, &threadBitmaps]() {

            // UPEWNIJ SIĘ że to MEMORY_BITMAP
            al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

            ALLEGRO_BITMAP* local =
                    al_create_bitmap(screen_width, screen_height);

            al_set_target_bitmap(local);
            al_clear_to_color(al_map_rgba(0,0,0,0));

            for (size_t i = begin; i < end; ++i) {
                ALLEGRO_COLOR c =
                        al_map_rgba(tasks[i].key, 0, 0, 255);

                tasks[i].atom->draw(c);
            }

            threadBitmaps[t] = local;
        });
    }

    for (auto& th : threads)
        th.join();

    // 🔵 składanie w głównym wątku
    al_set_target_bitmap(przyciski);
    al_clear_to_color(al_map_rgba(0,0,0,255));

    for (auto* bmp : threadBitmaps) {
        if (bmp) {
            al_draw_bitmap(bmp, 0, 0, 0);
            al_destroy_bitmap(bmp);
        }
    }

    al_set_target_bitmap(old);
}
Page::~Page()
{
    al_destroy_bitmap(przyciski);
    przyciski= nullptr;
}

void trojkat::addMoreLines(punkt C, punkt D) {
    addMoreLinesH(C,D);
}

trojkat::trojkat(punkt A, punkt B, punkt C, punkt D) {
    //trójkąt składa się z linii ax+b oraz osi x i osi y
    //jeżeli typ
    AB={A,B,true};//ax+b,
    BC={B,C, false};s
    addMoreLines(C,D);
}

