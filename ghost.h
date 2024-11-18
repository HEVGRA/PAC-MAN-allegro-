#ifndef GHOST_H_INCLUDED
#define GHOST_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include "Object.h"
#include "Circle.h"
#include "global.h"
#include "me.h"
using namespace std;

// enum {LEFT=0, UP, RIGHT, DOWN};

class ghost: public Object {
public:
    ghost(std::vector<int> path, int t, set<int> cross);
    virtual ~ghost();

    void Draw();

    void Load_Move();

    void Move(me* m, bool escape, bool timestop);
    
    int get_x(){return circle->x; };
    int get_y(){return circle->y; };
    int get_speed(){return speed;}
    int getDir() { return direction; }
    void reset_status(){ circle->x = start_x; circle->y = start_y; isout = 0; outtimecnt = 0; direction = 4; isescape = 0;speed = 2;}
    
    void change_speed(int sp){ speed += sp;}
    void set_speed(int sp){ speed = sp;}
    void outtimecntinc(){ outtimecnt ++;}

private:
    int direction_count[4];
    int speed = 2;
    int type = 0;
    int isout = 0;
    int outtime = 0;
    int outtimecnt = 0;
    bool isescape = 0;
    // direction and index for "path"
    random_device rd;
    default_random_engine gen = std::default_random_engine(rd());
    unsigned int step;
    int direction = 4;
    //start point
    int start_x, start_y;


    // set of animation images
    vector<ALLEGRO_BITMAP*> moveImg;
    // path on map
    vector<int> path;
    set<int> crossroad;

};


#endif 