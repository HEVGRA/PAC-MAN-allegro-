#include "Coin.h"
using namespace std;

const int draw_frequency = 5;

Coin::Coin(int thegrid, vector<ALLEGRO_BITMAP*> coinmo)
{
    circle = new Circle;
    circle->x = (thegrid%20) * grid_width;
    circle->y = (thegrid/20) * grid_height;
    sprite_pos = 0;
    counter = 0;
    moveImg = coinmo;
    // Load_Move();
}

Coin::~Coin()
{
    for(unsigned int i=0; i<moveImg.size(); i++)
    {
        ALLEGRO_BITMAP *img = moveImg[i];
        moveImg.erase(moveImg.begin() + i);
        i--;
        al_destroy_bitmap(img);
    }
    moveImg.clear();

    delete circle;
}

void
Coin::Draw()
{
    counter = (counter + 1) % draw_frequency;
    if(counter == 0)
        sprite_pos = (sprite_pos + 1) % 4;

    al_draw_scaled_bitmap(moveImg[sprite_pos],
            0, 0,                                // source origin
            al_get_bitmap_width(moveImg[sprite_pos]),     // source width
            al_get_bitmap_height(moveImg[sprite_pos]),    // source height
            circle->x+6, circle->y+3,                                // target origin
            grid_height-12, grid_height-3,                            // target dimensions
            0                                    // flags
    );
}