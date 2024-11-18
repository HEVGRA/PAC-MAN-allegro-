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
class Coin: public Object {
public:
    Coin(int grid, vector<ALLEGRO_BITMAP*> co);
    virtual ~Coin();
    int get_x(){return circle->x; };
    int get_y(){return circle->y; };
    void Draw();

private:
    int counter;
    int sprite_pos;
    vector<ALLEGRO_BITMAP*> moveImg;

};
