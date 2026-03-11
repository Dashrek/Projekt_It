#include "klasy.hpp"
#include <format>
class moved {

    int move_id;//numer ruchu
    bool direction;//kierunek ruchu- true zgodnie ze wskazówkami zegara
    int x;//współrzędne węzła x;
    int y;//współrzędne węzła y;
public:
    moved(bool dir, int xx, int yy, int id) ;
    string s_str();

};
struct AtomData {
    int id;
    Atom* pointer;
};
class Game {

    vector<vector<AtomData>> przyciski;
    vector<vector<AtomData>> tablica;
    vector<moved>moves;
    int w_i,h_i,move_count;
    bool no_validate;
    string actual_word;
public:
    Game();
    ~Game();
    void Start(Page* Strona_glowna, ButtonFactory *Baza, string word, string to_word, int length, int steps, bool timer, string time);
    void flush();
    bool validate();

};


class Client {
private:
    SocketType clientSocket = INVALID_SOCKET;
    std::atomic<bool> connected{false};
    std::thread receiveThread;
    std::mutex queueMutex;
    std::queue<std::string> responseQueue;

    void receiveLoop(); // Wątek tła do odbierania danych

public:
    Client();
    ~Client();

    bool connectToServer(const std::string& ip, int port);
    void disconnect();

    // Metody komunikacyjne
    void sendRequest(const std::string& message);
    void requestLogin(const std::string& login, const std::string& pass);
    void sendSolution(const std::vector<std::string>& moves);

    // Zarządzanie odpowiedziami w pętli gry
    bool hasResponses();
    std::string getNextResponse();
};

size_t utf8_len(const std::string& s);
void Pagedefault(ButtonFactory * Baza, Page * Strona_glowna, Game * Gra);
void PageNewGameSolo(ButtonFactory * Baza, Page * Strona_glowna, Game * Gra);
void add_last_textfield(Page * Strona_glowna);
void zarejestruj(ButtonFactory * Baza,  Page *Strona_glowna, Game * Gra);
void PageZaloguj(ButtonFactory *Baza, Page *Strona_glowna, Game *Gra);