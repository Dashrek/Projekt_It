#include "klasy.hpp"
#include <format>
#include <fstream>
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
class Client {
private:
    SocketType clientSocket = INVALID_SOCKET;
    std::atomic<bool> connected{false};
    std::thread receiveThread;
    std::mutex queueMutex;
    std::queue<std::string> responseQueue;
    string ranking;
    int userId=0;
    // Wątek tła do odbierania danych

public:
    int get_Id();
    void set_Id(int id);
    Client()=default;
    ~Client();
    void receiveLoop();
    bool connectToServer(const string& ip, int port);
    void disconnect();
    void logout();
    void add_to_ranking(string word);
    string get_ranking();
    void flush_ranking();
    // Metody komunikacyjne
    void sendRequest(const string& message);
    void requestLogin(const string& login, const string& pass);
    void sendSolution(vector<moved> moves, int mover);

    // Zarządzanie odpowiedziami w pętli gry
    bool hasResponses();
    std::string getNextResponse();
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
    void Start(Page* Strona_glowna, ButtonFactory *Baza, string word, string to_word, int length, int steps, bool timer, string time, bool clientStatus, Client * client);
    void flush();
    bool validate();

};


void PageRanking(ButtonFactory * Baza, Page * Strona_glowna, Game * Gra, Client * client);
void update(Client * client,Page * Strona_glowna, ButtonFactory * Baza, Game *Gra);
std::map<std::string, std::string> loadConfig(const std::string& filename);
size_t utf8_len(const std::string& s);
void Pagedefault(ButtonFactory * Baza, Page * Strona_glowna, Game * Gra, Client * client);
void wygaszacz(string Name,ButtonFactory * Baza,  Page *Strona_glowna, Game *Gra, Client * client);
void wygaszacz1(string Name,ButtonFactory * Baza,  Page *Strona_glowna, Game *Gra, Client * client);
void PageNewGameSolo(ButtonFactory * Baza, Page * Strona_glowna, Game * Gra,Client *client);
void add_last_textfield(Page * Strona_glowna);
void zarejestruj(ButtonFactory * Baza,  Page *Strona_glowna, Game * Gra,Client *client);
void wyrejestruj(ButtonFactory * Baza,  Page *Strona_glowna, Game * Gra,Client *client);
void PageZaloguj(ButtonFactory *Baza, Page *Strona_glowna, Game *Gra, Client *client);