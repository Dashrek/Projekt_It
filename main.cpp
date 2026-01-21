#include <iostream>
#include <allegro5/allegro.h>
using namespace std;
int main(int argc,char* argv[]) {
    ALLEGRO_DISPLAY * display = nullptr;

    if( !al_init() ) {
        cout << "Failed to initialize allegro!" << endl;
        return - 1;
    }

    display = al_create_display( 640, 480 );
    if( !display ) {
        cout << "Failed to create display!"<< endl;
        return - 1;
    }

    al_clear_to_color( al_map_rgb( 25, 60, 120 ) );
    al_flip_display();
    al_rest( 3.0 );

    al_destroy_display( display );

    return 0;
}
