#include <iostream>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
struct ButtonImage{
    std::string width;
    std::string height;
    ALLEGRO_BITMAP* normal;
    ALLEGRO_BITMAP* hover;
    ALLEGRO_BITMAP* pressed;
};
class Button{
    public:
        Button();
        ~Button();
};