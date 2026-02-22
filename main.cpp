#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cstdio>
#include "klasy.hpp"
int main()
{
    screen_width=800;
    screen_height=600;
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_install_mouse();
    al_init_font_addon();
    al_init_ttf_addon();


    // ===== KLUCZOWE FLAGI OKNA =====
    al_set_new_display_flags(
        ALLEGRO_RESIZABLE |
        ALLEGRO_WINDOWED
    );

    ALLEGRO_DISPLAY* display = al_create_display(screen_width, screen_height);//obraz
    if (!display)
        return -1;

    al_set_target_backbuffer(display);
    al_clear_to_color(al_map_rgb(30, 30, 40));
    /*al_draw_rectangle(10, 10,
                      static_cast<float>(al_get_display_width(display)) - 10,
                      static_cast<float>(al_get_display_height(display)) - 10.f,
                      al_map_rgb(200, 200, 255), 2);*/
    al_flip_display();

    auto Baza=new ButtonFactory();
    auto Strona_glowna=new Page();
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
                              f_HTML("#000000")},"Nowa gra Solo");
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Nowa Gra Online","50vw", "50vh");
    /*Strona_glowna->addButton(*Baza,"Złoto","Nowa Gra Online",
                             {"position-x:50vw",
                              "position-y:50vh",
                              "font-size:3vw",
                                 "font-maxwidth:340px",
                              "font-name:./fonts/orbitron-black.ttf",
                              "font:#FEBD27FF",
                              "font-shadow:#00000050"});*/
    Strona_glowna->addButton(*Baza,*Strona_glowna->buttons[Strona_glowna->getKlucz()-1],"Gra Rankingowa","50vw", "80vh");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_set_window_title(display, "Allegro 5 - Resize / Minimize / Maximize");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    bool running = true;
    bool redraw = false;
    bool hover=false;
    bool resize=false;
    bool start=true;
    int mouse_x,mouse_y;
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        switch (ev.type) {

            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;

            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(display);
                screen_width = al_get_display_width(display);
                screen_height = al_get_display_height(display);
                resize=true;
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                hover=true;
                mouse_x=ev.mouse.x;
                mouse_y=ev.mouse.y;
                break;
        }

        if (redraw && al_is_event_queue_empty(queue)) {

            al_set_target_backbuffer(display);
            if (resize || start) {
                al_clear_to_color(al_map_rgb(30, 30, 40));
                Strona_glowna->buildButtons(display);
                Strona_glowna->createBitmap();
                resize=false;
                start=false;
                al_flip_display();
            }
            if(hover){
                if (Strona_glowna->hover(mouse_x, mouse_y)) {
                    al_clear_to_color(al_map_rgb(30,30,40));

                    Strona_glowna->buildButtons(display);
                    hover=false;
                    al_flip_display();
                }
            }

            redraw = false;
        }
    }

    delete Strona_glowna;
    delete Baza;
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}