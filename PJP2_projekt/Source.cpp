#include<iostream>
#include<cmath>
#include<allegro5\allegro.h>
#include<allegro5\keyboard.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>


#define ScreenWeidth 800
#define ScreemHeigth 600
#define grawitacja 9.81
#define time  0.5

using namespace std;

int main(){
	const int FPS = 60;
	if (!al_init()){
		al_show_native_message_box(NULL, NULL, NULL, "Wystapil blad podczas initacji okienka", NULL, NULL);
		return -1;
	}

	al_set_new_display_flags(ALLEGRO_WINDOWED);
	ALLEGRO_DISPLAY *display = al_create_display(ScreenWeidth, ScreemHeigth);
	al_set_window_position(display, 200, 100);
	al_set_window_title(display, "Nauka");

	if (!display){
		al_show_native_message_box(NULL, "Simple Title", "ERROR", "Okno nie zostalo utworzone", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	
	ALLEGRO_BITMAP *background1 = al_load_bitmap("pejzaz.bmp");
	ALLEGRO_BITMAP *backgroud2 = al_load_bitmap("pejzaz2.bmp");
	ALLEGRO_BITMAP *background3 = al_load_bitmap("pejzaz3.bmp");
	ALLEGRO_TIMER	*timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *event_queque = al_create_event_queue();
	ALLEGRO_FONT *font = al_load_ttf_font("RAVIE.ttf", 24, 0);
	al_register_event_source(event_queque, al_get_keyboard_event_source());
	al_register_event_source(event_queque, al_get_timer_event_source(timer));
	al_register_event_source(event_queque, al_get_display_event_source(display));

	if (!font){
		al_show_native_message_box(NULL, "CZCIONKA", "ERROR", "Nie mozna za³adowaæ czcionki", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_start_timer(timer);


	bool done = false;
	int score = 0;
	int jump=1;
	int backGround1X = 0;
	int backGround1Y = 0;
	int backGround2X = backGround1X+800;
	int backGround2Y = backGround1Y;
	int backGround3X = backGround1X + 1600;
	int backGround3Y = backGround1Y;
	int playerX = backGround1X+75;
	int playerY = 520;
	int position = playerX;
	int moveSpeed = 5;
	int counter = 0;
	al_flip_display();

	while (!done){


		ALLEGRO_EVENT events;
		al_wait_for_event(event_queque, &events);
		if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done = true;
		}
		if (events.type = ALLEGRO_EVENT_TIMER){
			switch (events.keyboard.keycode){
			case ALLEGRO_KEY_UP:
				if (playerY == 450){
					jump = 0;
				}
				if (playerY == 520){
					jump = 1;
				}
				if (playerY <= 520 && jump == 1){
					playerY -= moveSpeed;
					playerX += 2;
					backGround1X -= moveSpeed;
					backGround2X -= moveSpeed;
					backGround3X -= moveSpeed;
				}
				else if (playerY >= 450 && jump == 0){
					playerY += moveSpeed;
					playerX += 2;
					backGround1X -= moveSpeed;
					backGround2X -= moveSpeed;
					backGround3X -= moveSpeed;
				}
				break;

			case ALLEGRO_KEY_ESCAPE:
				int question;
				question = al_show_native_message_box(display, "Question", "Exit", "Do you want exit this program", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
				if (question == 1){
					done = true;

				}
				else{

				}
				break;
			}
		}

		backGround1X -= moveSpeed;
		backGround2X -= moveSpeed;
		backGround3X -= moveSpeed;
		position += 2;
		score = (int) position / (moveSpeed+1);
		cout << score << endl;

		if (backGround1X <= -1600){
			backGround1X = 0;
			backGround2X = backGround1X + 800;
			backGround3X = backGround1X + 1600;
		}
		al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
		al_draw_bitmap(background1, backGround1X, backGround1Y, NULL);
		al_draw_bitmap(backgroud2, backGround2X, backGround2Y, NULL);
		al_draw_bitmap(background3, backGround3X, backGround3Y, NULL);
		al_draw_textf(font, al_map_rgb(255,255, 255), ScreemHeigth, ScreenWeidth/50, ALLEGRO_ALIGN_LEFT, "SCORE:%i",score); 
		al_draw_filled_rectangle(playerX, playerY, playerX+20,playerY+20, al_map_rgb(255, 20, 13));
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
	}
	


	al_destroy_bitmap(background1);
	al_destroy_bitmap(backgroud2);
	al_destroy_bitmap(background3);
	al_destroy_event_queue(event_queque);
	al_destroy_display(display);
	return 0;
}