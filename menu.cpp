#include "menu.hpp"
void Pagedefault(ButtonFactory * Baza, Page * Strona_glowna){//Funkcja rysująca menu główne
    Strona_glowna->makeEmpty();
    Strona_glowna->addElement<Button>(*Baza,"Złoto",
                             vector<string>{"position-x:50vw",
                              "position-y:20vh",
                              "font-size:3vw",
                                 "font-maxwidth:340px",
                              "font-name:./fonts/orbitron-black.ttf",
                              "font:#FEBD27FF",
                              "font-shadow:#00000050"},vector<string>{"width:30vw",
                                                        "min-width:20px",
                                                        "max-width:350px",
                                                        "height:15vh",
                                                        "min-height:20px",
                                                        "max-height:200px",
                                                        "border-radius:2px",
                                                        "border-thickness:0.5vw",
                                                        "shadow-offset-x:2px",
                                                        "shadow-offset-y:2px"},
                             vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                              f_HTML("#000000"),
                              f_HTML("#FEF177"),
                              f_HTML("#F25420"),
                              f_HTML("#000000")},"Nowa gra Solo");//Przycisk nowej gry solo
    cout << Strona_glowna->buttons[Strona_glowna->getKlucz()-1]->name<< "Dupa\n";
    Strona_glowna->buttons[Strona_glowna->getKlucz()-1]->checkevent=[page=Strona_glowna,factory=Baza]{PageNewGameSolo(factory,page);};
    //Przycisk nowej gry online
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Nowa Gra Online","50vw", "50vh");
    //Przycisk Nowej Gry Rankingowej
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Gra Rankingowa","50vw", "80vh");
    //Przycisk do zalogowania na nowe konto
    Strona_glowna->addElement<TriangleButton>(*Baza,"Srebro",vector<string>{"position-x:16vw",
                                                                            "position-y:16vh",
                                                                            "font-size:3vw",
                                                                            "font-maxwidth:170px",
                                                                            "font-minwidth:12vw",
                                                                            "font-name:./fonts/orbitron-black.ttf",
                                                                            "font:#FEBD27FF",
                                                                            "font-shadow:#00000050"},vector<string>{"width:30vw","min-width:20px",
                                                                                                                    "max-width:350px",
                                                                                                                    "height:30vh",
                                                                                                                    "min-height:20px",
                                                                                                                    "max-height:350px",
                                                                                                                    "border-radius:2px",
                                                                                                                    "border-thickness:0.5vw",
                                                                                                                    "shadow-offset-x:2px",
                                                                                                                    "shadow-offset-y:2px"},
                                              vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                    f_HTML("#000000"),
                                                                    f_HTML("#FEF177"),
                                                                    f_HTML("#F25420"),
                                                                    f_HTML("#000000")},"Zaloguj:U");
    //Przycisk do zarejestrowania konta
    Strona_glowna->addElement<TriangleButton>(*Baza,"Brąz",vector<string>{"position-x:18vw",
                                                                            "position-y:18vh",
                                                                            "font-size:3vw",
                                                                            "font-maxwidth:170px",
                                                                            "font-minwidth:12vw",
                                                                            "font-name:./fonts/orbitron-black.ttf",
                                                                            "font:#FEBD27FF",
                                                                            "font-shadow:#00000050"},vector<string>{"width:30vw",
                                                                                                                    "min-width:20px",
                                                                                                                    "max-width:350px",
                                                                                                                    "height:30vh",
                                                                                                                    "min-height:20px",
                                                                                                                    "max-height:350px",
                                                                                                                    "border-radius:2px",
                                                                                                                    "border-thickness:0.5vw",
                                                                                                                    "shadow-offset-x:2px",
                                                                                                                    "shadow-offset-y:2px"},
                                              vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                    f_HTML("#000000"),
                                                                    f_HTML("#FEF177"),
                                                                    f_HTML("#F25420"),
                                                                    f_HTML("#000000")},"Zarejestruj:D");
    //Timer
    Strona_glowna->addElement<Timer>(*Baza,"Molibden", vector<string>{"position-x:90vw",
                                                                      "position-y:10vh",
                                                                      "font-size:2.5vw",
                                                                      "font-maxwidth:170px",
                                                                      "font-minwidth:8vw",
                                                                      "font-name:./fonts/orbitron-black.ttf",
                                                                      "font:#FEBD27FF",
                                                                      "font-shadow:#00000050",
                                                                      "time-format:hh;mm;ss",
                                                                      "crementation:+",
                                                                      "real-time:+"},vector<string>{"width:120px",
                                                                                                    "min-width:10vw",
                                                                                                    "max-width:192px",
                                                                                                    "height:10vh",
                                                                                                    "min-height:20px",
                                                                                                    "max-height:108px",
                                                                                                    "border-radius:2px",
                                                                                                    "border-thickness:0.5vw",
                                                                                                    "shadow-offset-x:2px",
                                                                                                    "shadow-offset-y:2px"},vector<ALLEGRO_COLOR>{f_HTML("#C8B5B5"),
                                                                                                                                                 f_HTML("#000000"),
                                                                                                                                                 f_HTML("#FEF177"),
                                                                                                                                                 f_HTML("#F25420"),f_HTML("#000000")}, "00;00;00");
    Strona_glowna->addCycle(Strona_glowna->getKlucz()-1);
    Strona_glowna->buttons[Strona_glowna->getKlucz()-1]->checkTimeEvent=[Strona_glowna, ten=Strona_glowna->getKlucz()-1]{Strona_glowna->addActive(ten);};
}

void PageNewGameSolo(ButtonFactory *Baza, Page *Strona_glowna) {
    Strona_glowna->makeEmpty();
    Strona_glowna->addElement<Atom>(*Baza, "Orange", vector<string>{"position-x:42.5vw",
                                                                        "position-y:20vh",
                                                                        "font-size:7vh",
                                                                        "font-name:./fonts/orbitron-black.ttf",
                                                                        "font:#7a2160",
                                                                        "font-shadow:#290b20",
                                                                        "font-maxwidth:15vw"}, vector<string>{"width:17vw","min-width:17vw","max-width:17vw",
                                                                                                                "height:10vh","min-height:10vh","max-height:10vh",
                                                                                                                "border-radius:1px","border-thickness:0.2vw","shadow-offset-x:3px","shadow-offset-y:3px"},
                                                                                                                vector<ALLEGRO_COLOR>{f_HTML("#f2bf41"),f_HTML("#7a6021"),f_HTML("#000000")}, "Podaj słowo:");
    //Strona_glowna->addElement<TextField>(Baza,"")
}
