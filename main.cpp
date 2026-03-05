#include "menu.hpp"

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
    Pagedefault(Baza,Strona_glowna);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_set_window_title(display, "Rubik 2D");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    bool running = true;
    bool redraw = false;
    bool hover=false;
    bool click=false;
    bool resize=false;
    Strona_glowna->start=true;
    bool clicked=false;
    int mouse_x,mouse_y;
    while (running) {
        ALLEGRO_EVENT ev,last_ev_axes;
        al_wait_for_event(queue, &ev);

        switch (ev.type) {

            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;

            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                resize=true;
                break;

            case ALLEGRO_EVENT_MOUSE_AXES:
                hover=true;
                last_ev_axes=ev;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                click=true;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                clicked=true;
                break;
            case ALLEGRO_EVENT_KEY_CHAR:
                if(Strona_glowna->tekstowy_klucz>0){
                    Strona_glowna->buttons[Strona_glowna->tekstowy_klucz]->add(ev);
                }
        }

        if (redraw && al_is_event_queue_empty(queue)) {

            al_set_target_backbuffer(display);

            if(hover){
                mouse_x=last_ev_axes.mouse.x;
                mouse_y=last_ev_axes.mouse.y;
                bool a1=false;
                if (Strona_glowna->findButtonHover(mouse_x, mouse_y, a1)) {
                    //al_clear_to_color(al_map_rgb(30,30,40));
                    //Strona_glowna->buildButtons(display);
                    //al_flip_display();
                }
/*                if (a1){
                    al_clear_to_color(al_map_rgb(30,30,40));
                    Strona_glowna->buildButtons(display);
                    al_flip_display();
                }*/
                hover=false;
            }
            if(click){
                Strona_glowna->findButtonActive(mouse_x,mouse_y,true);
                click=false;
            }
            if(clicked){
                Strona_glowna->findButtonActive(mouse_x,mouse_y,false);
                clicked=false;
            }
            if (resize || Strona_glowna->start) {
                al_acknowledge_resize(display);
                screen_width = al_get_display_width(display);
                screen_height = al_get_display_height(display);
                ALLEGRO_TRANSFORM proj;
                al_identity_transform(&proj);
                al_orthographic_transform(&proj, 0, 0, -1, screen_width , screen_height, 1);
                al_use_projection_transform(&proj);
                al_clear_to_color(al_map_rgb(30, 30, 40));
                Baza->ReCreateRectangle();
                Strona_glowna->ReloadFont();
                Strona_glowna->createBackbuffer();
                //Strona_glowna->buildButtons(display);
                //Strona_glowna->createBitmap();
                Strona_glowna->changeRanges();
                resize=false;
                Strona_glowna->start=false;
                al_flip_display();
            }
            Strona_glowna->thicCycle();
            Strona_glowna->flushCycle();


            redraw = false;
        }
    }

    delete Strona_glowna;
    delete Baza;
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}