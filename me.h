#ifndef ME_H_INCLUDED
#define ME_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Object.h"
#include "Circle.h"
#include "global.h"
using namespace std;

class me: public Object {
public:
    me(std::vector<int> path);
    virtual ~me();

    // Draw image per frame
    // override virtual function "Object::Draw"
    void Draw();
    // Load bitmaps of animation image into container "moveImg"
    void Load_Move();

    // Update monster position per frame
    // And detect if it reaches end point but not destroyed
    bool Move();
    int get_x(){return circle->x; };
    int get_y(){return circle->y; };
    // functions that return informations of monster
    int get_Dir() { return direction; }
    int get_Score() { return score; }
    int get_Life() { return Life;}
    bool get_ghostescape(){ return ghostescape; }
    bool get_timestop(){return timestop;}
    int get_grid(){return get_x()/24 + 20 * (get_y()/24);}

    void reset_status();
    void transform(int ty);

    void change_ghostescape(bool gh){ ghostescape = gh; }
    void change_timestop(bool ti){ timestop = ti; }
    void change_speed(int sp){ speed += sp;}
    void change_score(int sc){ if(score+sc >= 200 && score<200) change_life(1); score += sc; }
    void change_life(int li){ Life += li;}
    void change_direction();
    void Rememberdirection(int);
    bool Deteced_overlap(int, int, int);


private:
    int direction_count[4];
    int Life = 3;
    int speed = 2;
    int score = 0;

    bool ghostescape = false;
    bool timestop = false;

    // direction and index for "path"
    unsigned int step;
    int direction = RIGHT;
    int remdirection = RIGHT;
    //start point
    int start_x, start_y;
    // end point
    int end_x, end_y;
    // animation counter
    int counter = 0;
    // animation image of current direction
    int sprite_pos = 0;

    // set of animation images
    std::vector<ALLEGRO_BITMAP*> moveImg;
    // path on map
    std::vector<int> path;

};


#endif 