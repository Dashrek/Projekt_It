#include "klasy.hpp"
class move {

    int move_id;//numer ruchu
    bool direction;//kierunek ruchu- true zgodnie ze wskazówkami zegara
    int x;//współrzędne węzła x;
    int y;//współrzędne węzła:y
public:
    move(bool dir, int xx, int yy) ;
    string s_str();

};
class Game {
    shared_ptr<Page> Strona;
public:
    Game();
    ~Game();
    void flush();
    void createGame();
    void validate();
};
void Pagedefault(ButtonFactory * Baza, Page * Strona_glowna);
void PageNewGameSolo(ButtonFactory * Baza, Page * Strona_glowna);
void add_last_textfield(Page * Strona_glowna);
void zarejestruj(ButtonFactory * Baza,  Page *Strona_glowna);
void PageZaloguj(ButtonFactory *Baza, Page *Strona_glowna);