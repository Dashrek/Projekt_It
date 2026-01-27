#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include<cstdio>
int main()
{
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    // ===== KLUCZOWE FLAGI OKNA =====
    al_set_new_display_flags(
            ALLEGRO_RESIZABLE |
            ALLEGRO_WINDOWED
    );
    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    if (!display)
        return -1;
    al_set_window_title(display, "Allegro 5 - Resize / Minimize / Maximize");
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    bool running = true;
    int w,h;
    while (running)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);
        switch (ev.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                running = false;
                break;
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                // WAŻNE: MUSI BYĆ
                al_acknowledge_resize(display);
                w = al_get_display_width(display);
                h = al_get_display_height(display);
                printf("Resize: %dx%d\n", w, h);
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    running = false;
                break;
        }
        // ===== RENDER =====
        al_clear_to_color(al_map_rgb(30, 30, 40));
        al_draw_rectangle(10, 10,
                          static_cast<float>(al_get_display_width(display)) - 10,
                          static_cast<float>(al_get_display_height(display)) - 10.f,
                          al_map_rgb(200, 200, 255), 2);
        al_flip_display();
    }
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    return 0;
}