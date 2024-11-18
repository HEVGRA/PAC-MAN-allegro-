
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Object.h"
#include "Circle.h"
#include "global.h"
#include "me.h"
using namespace std;

class Menu
{
public:
    Menu();
    virtual ~Menu();
    void Draw(int,int,int,int, bool*);
    void Draw_pass(int ,int);
    int get_prop_cnt(int ge){ return prop_cnt[ge];}
    void change_prop_cnt(int cn, int ty){ prop_cnt[ty]+=cn;}
    void set_prop_cnt(int cn, int ty){ prop_cnt[ty] = cn;}

private:
    vector<ALLEGRO_BITMAP*> allfruit;
    ALLEGRO_BITMAP* game_background = NULL;
    ALLEGRO_BITMAP* right_background = NULL;
    ALLEGRO_BITMAP* level_1 = NULL;
    ALLEGRO_BITMAP* next_level = NULL;
    ALLEGRO_BITMAP* you_win = NULL;
    ALLEGRO_BITMAP* you_lose = NULL;
    ALLEGRO_BITMAP* smarf = NULL;
    ALLEGRO_BITMAP* score = NULL;
    ALLEGRO_FONT *menuFont;
    ALLEGRO_FONT *propFont;
    ALLEGRO_FONT *passFont;
    int prop_cnt[3] = {0,0,0};
};

