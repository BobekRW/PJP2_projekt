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

bool collision(float x, float y, float ex, float ey, int height, int width){
	if (x + width<ex || x>width + ex || y + height<ey || y>height + ey){
		return false;
	}

	return true;
}

int main(){

	if (!al_init()){
		al_show_native_message_box(NULL, "Error", "Allegro 5.0.10", "Can not initialize allegro", NULL, NULL);
		return -1;
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	ALLEGRO_DISPLAY *display = al_create_display(ScreenWeidth, ScreenHeigth);
	al_set_window_position(display, 200, 50);
	al_set_window_title(display, "Hungry Run");

	const float FPS = 60.0;
	bool done = false;
	bool starter = true;
	bool instruction = false;
	bool game = false;
	bool jump = false;
	bool motion = false;
	int choiceY = 330;
	int pejzazX = 0;
	int pejzaz2X = pejzazX + 800;
	int playerX = 10;
	int playerY = 480;
	int moveSpeed = 5;
	int score = 0;
	int highscore = 0;
	const int gravity = 1;
	int jumpSpeed = 12;
	int velx,vely;
	float position = 0;
	int obstacle1X = 800;
	int obstacle1Y = 505;
	int obstacle2X = 1200;
	int obstacle2Y = 505;
	int obstacle3X = 1500;
	int obstacle3Y = 505;
	velx = vely = 0;
	position = pejzazX;
	const int maxFrame = 4;
	int curFrame = 0;
	int frameCount = 0;
	int frameDelay = 7;

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
	ALLEGRO_BITMAP *przeszkoda = al_load_bitmap("przeszkoda.png");
	ALLEGRO_BITMAP *przekszoda2 = al_load_bitmap("przeszkoda.png");
	ALLEGRO_BITMAP *przeszkoda3 = al_load_bitmap("przeszkoda.png");
	ALLEGRO_BITMAP *skok=al_load_bitmap("PostacSkok.png");
	ALLEGRO_BITMAP *animation[maxFrame];
	animation[0] = al_load_bitmap("Postac.png");
	animation[1] = al_load_bitmap("Postac2.png");
	animation[2] = al_load_bitmap("Postac3.png");
	animation[3] = al_load_bitmap("Postac4.png");
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

	for (int i = 0; i < maxFrame; i++)
		al_convert_mask_to_alpha(animation[i], al_map_rgb(222, 122, 180));

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
				vely = -jumpSpeed;
				playerY = 481;
				jump = false;
				motion = true;
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
					motion = true;
				}
			}
			else
				velx = 0;
				
			if (!jump){
				vely += gravity;
			}
			else
				vely = 0;
			

			playerX += velx;
			playerY += vely;

			jump = (playerY>= 480);

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
			obstacle1X -= moveSpeed;
			obstacle2X -= moveSpeed;
			obstacle3X -= moveSpeed;

			if (pejzazX <= -800){
				pejzazX = 0;
				pejzaz2X = pejzazX + 800;
			}

			if (obstacle1X <= -700){
				obstacle1X = 800;
				obstacle2X = 1200;
				obstacle3X = 1500;
			}


			if (score <= 30){
				moveSpeed = 5;
			}
			else if(score >30  && score<=80){
				moveSpeed = 8;
			}
			else if(score>150 && score<=300){
				moveSpeed = 10;
			}
			else{
				moveSpeed = 12;
			}

			if (collision(playerX, playerY, obstacle1X, obstacle1Y, 32, 32)){
				al_rest(0.5);
				al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenHeigth / 1.5, ScreenWeidth / 2.5, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_rest(1.0);
				score = 0;
				position = 0;
				moveSpeed = 5;
				playerX = 10;
				playerY = 481;
				pejzazX = 0;
				pejzaz2X = pejzazX + 800;
				obstacle1X = 800;
				obstacle2X = 1200;
				obstacle3X = 1500;
				game = false;
				starter = true;
			}

			if (collision(playerX, playerY, obstacle2X, obstacle2Y, 32, 32)){
				al_rest(0.5);
				al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenHeigth / 1.5, ScreenWeidth / 2.5, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_rest(1.0);
				score = 0;
				position = 0;
				moveSpeed = 5;
				playerX = 10;
				playerY = 481;
				pejzazX = 0;
				pejzaz2X = pejzazX + 800;
				obstacle1X = 800;
				obstacle2X = 1200;
				obstacle3X = 1500;
				game = false;
				starter = true;
			}

			if (collision(playerX, playerY, obstacle3X, obstacle3Y, 32, 32)){
				al_rest(0.5);
				al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenHeigth / 1.5, ScreenWeidth / 2.5, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_rest(1.0);
				score = 0;
				position = 0;
				moveSpeed = 5;
				playerX = 10;
				playerY = 481;
				pejzazX = 0;
				pejzaz2X = pejzazX + 800;
				obstacle1X = 800;
				obstacle2X = 1200;
				obstacle3X = 1500;
				game = false;
				starter = true;
			}

			position += moveSpeed;
			score = (int) (position + moveSpeed)/100;

			if (highscore < score)
				highscore = score;

			al_draw_bitmap(pejzaz, pejzazX, 0, NULL);
			al_draw_bitmap(pejzaz2, pejzaz2X, 0, NULL);
			al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenHeigth, ScreenWeidth/50, ALLEGRO_ALIGN_LEFT, "SCORE:%d", score);
			al_draw_textf(font, al_map_rgb(255, 255, 255), ScreenHeigth/2.5, ScreenWeidth / 50, ALLEGRO_ALIGN_RIGHT, "HIGHSCORE:%d", highscore);
			if (++frameCount >= frameDelay){
				if (++curFrame >= maxFrame)
					curFrame = 0;

				frameCount = 0;
			}

			if (motion && jump)
				al_draw_bitmap(animation[curFrame], playerX, playerY, NULL);
			if (!jump)
				al_draw_bitmap(skok, playerX, playerY, NULL);
			al_draw_bitmap(przeszkoda, obstacle1X, obstacle1Y, 0);
			al_draw_bitmap(przekszoda2, obstacle2X, obstacle2Y, 0);
			al_draw_bitmap(przeszkoda3, obstacle3X, obstacle3Y, 0);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

		}
	}

	for (int i = 0; i < maxFrame; i++)
		al_destroy_bitmap(animation[i]);

	al_destroy_display(display);
	al_destroy_bitmap(start);
	al_destroy_bitmap(instrukcja);
	al_destroy_bitmap(pejzaz);
	al_destroy_bitmap(pejzaz2);
	al_destroy_bitmap(przeszkoda);
	al_destroy_bitmap(przekszoda2);
	al_destroy_bitmap(przeszkoda3);
	al_destroy_bitmap(skok);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}