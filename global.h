#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#include <allegro5/allegro_font.h>

#define font_size 12
#define grid_width 24
#define grid_height 24

#define window_width 800
#define window_height 600
#define field_width 480
#define field_height 600

#define btn_width 200
#define btn_height 100

#define Num_TowerType 5
#define Num_MonsterType 4
#define NumOfGrid (field_width / grid_width) * (field_height / grid_height)


extern int TowerRadius[];
extern char TowerClass[][20];
extern int TowerWidth[];
extern int TowerHeight[];


enum {LEFT=0, UP, RIGHT, DOWN};
enum {D_GHOST=0, D_COIN, D_FRUIT};
enum {CHERRY=0, GUAVA, STRAW};

#endif // GLOBAL_H_INCLUDED
