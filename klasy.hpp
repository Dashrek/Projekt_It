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
#include <thread>
using namespace std;
// Struktura przechowująca styl i wymiary (obsługuje px, vh, vw)

inline int screen_width, screen_height;
typedef struct {
    ALLEGRO_BITMAP* normal;//wyliczona mapa normal
    ALLEGRO_BITMAP* hover;// wyliczona mapa przy najechaniu
    ALLEGRO_BITMAP* pressed;// wyliczona mapa przy naciśnięciu
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
    ALLEGRO_COLOR shadow_color;//kolor cienia
    int typ;
    shared_ptr<ButtonImage> images;
} ButtonParameters;
int actual_value(const string &value);
ALLEGRO_COLOR f_HTML(string html_color);
vector<string> split_manual(string s,const string delimiter);
void AllegroImageDeleter(ButtonImage* bi);
void AllegroGaussFilter(ALLEGRO_BITMAP* Source, ALLEGRO_BITMAP* Target, int w, int h);
bool BakeFontToMemoryBitmap(ALLEGRO_BITMAP* dest,ALLEGRO_FONT* font,const string& text,ALLEGRO_COLOR color,int x = 0,int y = 0);
enum Typ{Przycisk,Pierwiastek, PoleTekstowe, TriangleU, TriangleD};
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
class Atom{
protected:
    string name;
    shared_ptr<ButtonParameters> param;
    string posx, posy;
    string fontsize,font,fontmaxwidth,fontminwidth;
    ALLEGRO_COLOR font_color,font_shadow_color;
    void virtual extractPositionH(const vector<string>& res);
    void virtual generateFontH();
    void virtual buildH();

    Atom(ButtonFactory& Factory, string styleID, const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="",int typ=Pierwiastek);
    ALLEGRO_FONT *Font=nullptr;
public:
    Atom(ButtonFactory& Factory, string styleID, const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="");
    Atom(const Atom& Inny, ButtonFactory& factory, const string nazwa, const string pos_x, const string pos_y);
    void build();
    void extractPosition(const vector<string>& res);
    void generateFont();
    void virtual ReaddRange(){};
    virtual std::unique_ptr<Atom> clone(ButtonFactory& factory,
                                        const std::string& nazwa,
                                        const std::string& pos_x,
                                        const std::string& pos_y) const
    {
        return std::make_unique<Atom>(*this, factory, nazwa, pos_x, pos_y);
    }
    virtual void hover(){};
    virtual void pressed(){};
    virtual void normal(){};
    virtual void draw(ALLEGRO_COLOR color){};
    virtual bool check(int x, int y){return false;};
    virtual ~Atom();



};
class Button : public Atom{
    //aspekty wizualne


public:
    Button(ButtonFactory& factory, string styleID,const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="");
    Button(const Button& Inny, ButtonFactory& factory, string nazwa, string pos_x, string pos_y);
    void hover() override;
    void pressed() override;
    void normal() override;
    void take_event();
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
    function <void()> checkevent;
    unique_ptr<trojkat> t;
    bool tryb[2];
    void buildH() override;

    Button(ButtonFactory& Factory, string styleID, const vector<string>& font_h, const vector<string>& res, const vector<ALLEGRO_COLOR>& col, string nam,int typ);
};
class TriangleButton : public Button{
    //aspekty wizualne
public:
    TriangleButton(ButtonFactory& factory, string styleID,const vector<string>& font_h={}, const vector<string>& res={}, const vector<ALLEGRO_COLOR>& col={}, string nam="");
    TriangleButton(const TriangleButton& Inny, ButtonFactory& factory, string nazwa, string pos_x, string pos_y);
    void take_event();
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
    int aktywny_przycisk;
    ALLEGRO_BITMAP *przyciski;
    int aktualny_klucz;
public:
    const int getKlucz() const{return aktualny_klucz;};
    void createBitmap();
    void changeRanges();
    bool findButton(int x, int y);
    bool hover(int x, int y);
    map<int,unique_ptr<Atom>> buttons;
    Page();
    ~Page();
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