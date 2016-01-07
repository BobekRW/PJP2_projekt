#include<iostream>
#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\keyboard.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>


#define ScreenWeidth 800
#define ScreenHeigth 600

using namespace std;


int main(){

	if (!al_init()){
		al_show_native_message_box(NULL, "Error", "Allegro 5.0.10", "Can not initialize allegro", NULL, NULL);
		return -1;
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	ALLEGRO_DISPLAY *display = al_create_display(ScreenWeidth, ScreenHeigth);
	al_set_window_position(display, 200, 50);
	al_set_window_title(display, "Hungry Run");

	const float FPS = 90.0;
	bool done = false;
	bool starter = true;
	bool instruction = false;
	bool game = false;
	bool jump = false;
	int choiceY = 330;
	float pejzazX = 0;
	float pejzaz2X = pejzazX + 800;
	float playerX = 10;
	float playerY = 545;
	float moveSpeed = 5;
	int score = 0;
	const float gravity = 1;
	float jumpSpeed = 15;
	float velx,vely;
	velx = vely = 0;
	float position = 0;
	position = pejzazX;
	int temp = 0;
	

	if (!display){
		al_show_native_message_box(NULL, "Error", "Display", "Can not initialize display", NULL, NULL);
		return -1;
	}


	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();

	ALLEGRO_FONT *font = al_load_ttf_font("megaman.ttf", 20, 0);
	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_BITMAP *start = al_load_bitmap("EkranStartowy.png");
	ALLEGRO_BITMAP *instrukcja = al_load_bitmap("Instrukcja.png");
	ALLEGRO_BITMAP *pejzaz = al_load_bitmap("Pejzaz.png");
	ALLEGRO_BITMAP *pejzaz2 = al_load_bitmap("Pejzaz2.png");
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	if (!font){
		al_show_native_message_box(NULL, "ERROR", "FONT", "Can not initialize font", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}


	al_start_timer(timer);

	while (!done){
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		if (events.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (events.keyboard.keycode){
			case ALLEGRO_KEY_ESCAPE:
				if (starter){
					done = true;
				}
				if (instruction){
					instruction = false;
					starter = true;
				}
				if (game){
					game = false;
					starter = true;
				}
				break;
			case ALLEGRO_KEY_DOWN:
				if (starter){
					if (choiceY >= 330 && choiceY < 440)
						choiceY += 55;
				}
				break;
			case ALLEGRO_KEY_UP:
				if (starter){
					if (choiceY > 330 && choiceY <= 440)
						choiceY -= 55;
				}
				break;
			}
		}

		else if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			done = true;

		if (events.type == ALLEGRO_EVENT_TIMER){
			al_get_keyboard_state(&keyState);
			if (al_key_down(&keyState, ALLEGRO_KEY_UP) && jump){
				velx = moveSpeed;
				vely = -jumpSpeed;
				jump = false;
			}
			else if (al_key_down(&keyState, ALLEGRO_KEY_ENTER)){
				if (choiceY == 440){
					starter = false;
					done = true;
				}
				if (choiceY == 385){
					instruction = true;
					starter = false;

				}
				if (choiceY == 330){
					starter = false;
					game = true;
				}
			}
			else
				velx = 0;
			if (!jump)
				vely += gravity;
			else
			vely = 0;
			playerX += velx;
			playerY += vely;

			jump = (playerY>= 545);
		}

		if (starter){
			al_draw_bitmap(start, 0, 0, NULL);
			al_draw_rectangle(200, choiceY, 580, choiceY + 50, al_map_rgb(200, 130, 65), 3.0);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		if (instruction){
			al_draw_bitmap(instrukcja, 0, 0, NULL);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		if (game){
			pejzazX -= moveSpeed;
			pejzaz2X -= moveSpeed;
			if (pejzazX <= -800){
				pejzazX = 0;
				pejzaz2X = pejzazX + 800;
			}

			position += moveSpeed;
			score = (int) (position + moveSpeed)/100;
			al_draw_bitmap(pejzaz, pejzazX, 0, NULL);
			al_draw_bitmap(pejzaz2, pejzaz2X, 0, NULL);
			al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenHeigth, ScreenWeidth/50, ALLEGRO_ALIGN_LEFT, "SCORE:%d", score);
			al_draw_filled_rectangle(playerX, playerY, playerX+20, playerY+20, al_map_rgb(245, 100, 145));
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}


	al_destroy_display(display);
	al_destroy_bitmap(start);
	al_destroy_bitmap(instrukcja);
	al_destroy_bitmap(pejzaz);
	al_destroy_bitmap(pejzaz2);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}