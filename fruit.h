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
class Fruit: public Object {
public:
    Fruit(int, int, int);
    virtual ~Fruit();
    void Load_Move();
    int get_type(){return type;}
    int get_x(){return circle->x; };
    int get_y(){return circle->y; };
    void Draw();

private:
    int type;
    ALLEGRO_BITMAP* myimg = NULL;
};
