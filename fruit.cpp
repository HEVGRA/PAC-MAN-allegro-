#include "Fruit.h"
using namespace std;

const int draw_frequency = 5;

Fruit::Fruit(int ty, int xx, int yy)
{
    circle = new Circle;
    circle->x = xx;
    circle->y = yy;

    // (9.5) * grid_width;
    // (11) * grid_height;
    type = ty;
    Load_Move();
}

Fruit::~Fruit()
{
    al_destroy_bitmap(myimg);
    delete circle;
}

void
Fruit::Load_Move()
{
    char buffer[50];
    sprintf(buffer, "./Fruit/fruit_(%d).png", type+1);
    myimg = al_load_bitmap(buffer);
}

void
Fruit::Draw()
{
    al_draw_scaled_bitmap(myimg,
            0, 0,                                // source origin
            al_get_bitmap_width(myimg),     // source width
            al_get_bitmap_height(myimg),    // source height
            circle->x, circle->y,                                // target origin
            grid_height, grid_height, 
            0                                    // flags
    );
}