#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <list>
#include <time.h>
#include <random>
#include "Menu.h"
#include "fruit.h"
#include "me.h"
#include "ghost.h"
#include "Coin.h"
#include "Level.h"
using namespace std;


#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7
#define MENU_START 8
#define MENU_ABOUT 9
#define GAME_NEXTLEVEL 10
#define GAME_BACKTOMENU 11


// clock rate
const float FPS = 60;

// total number of level
const int LevelNum = 4;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;

class GameWindow
{
public:
    // constructor
    GameWindow();
    // each process of scene
    void game_init();
    void game_reset();
    void game_play();

    void menu_play();
    void menu_init();
    int game_menu_run();
    int process_event_menu();
    void draw_menu();


    int game_run();
    int game_update();

    void show_err_msg(int msg);
    void game_destroy();

    void create_game_item();
    void create_next_game();
    void stop_timer();
    void start_timer();
    // each drawing scene function
    void draw_running_map();

    // process of updated event
    int process_event();
    // detect if mouse hovers over a rectangle
    bool mouse_hover(int, int, int, int);

public:
    bool initial = true;

private:
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *background = NULL;

    ALLEGRO_BITMAP *menu_background = NULL;
    ALLEGRO_BITMAP *about_background = NULL;

    ALLEGRO_BITMAP *menu_btn[3] = {NULL};
    ALLEGRO_BITMAP *menu_btn_s[3] = {NULL};

    ALLEGRO_DISPLAY* display = NULL;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *ghost_goout_timer = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *cherry_timer = NULL;
    ALLEGRO_TIMER *guava_timer = NULL;
    ALLEGRO_TIMER *straw_timer = NULL;
    ALLEGRO_TIMER *create_fruit_timer = NULL;
    ALLEGRO_TIMER *pass_timer = NULL;

    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE *get_coin[5] = {NULL};
    ALLEGRO_SAMPLE_INSTANCE *get_fruit = NULL;
    ALLEGRO_SAMPLE_INSTANCE *kill_ghost = NULL;
    ALLEGRO_SAMPLE_INSTANCE *dead = NULL;
    ALLEGRO_SAMPLE_INSTANCE *normal_backgroundmusic = NULL;
    ALLEGRO_SAMPLE_INSTANCE *win_backgroundmusic = NULL;
    ALLEGRO_SAMPLE_INSTANCE *lose_backgroundmusic = NULL;

    LEVEL *level = NULL;
    Menu *menu = NULL;

    Fruit *cur_fruit;

    me* human;
    vector<ghost*> ghostSet;
    vector<Coin* > coinSet;
    vector<Fruit* > fruitSet;
    vector<ALLEGRO_BITMAP*> coin_moveImg;

    random_device rd;
    default_random_engine gen = std::default_random_engine(rd());

    int vision_size = 150;
    int Coin_Inc_Count = 0;
    int mouse_x, mouse_y;
    int levelnow = 1;
    int levelmax = 2;

    int onWhichBtn = 0;
    int isAboutPage = 0;
    int nextft = 1;
    bool redraw = false;
    bool mute = false;

    bool fruitnow[3] = {false};
};

