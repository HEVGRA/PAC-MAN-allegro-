#include "me.h"
#include <algorithm>
using namespace std;

const int axis_x[] = {-1, 0, 1, 0};
const int axis_y[] = {0, -1, 0, 1};

// set counter frequency of drawing moving animation
const int draw_frequency = 5;

me::me(std::vector<int> path)
{
    this->path = path;
    this->step = 0;

    circle = new Circle;
    circle->x = (9.5) * grid_width;
    circle->y = (19) * grid_height;
    circle->r = grid_width/2;

    start_x = circle->x;
    start_y = circle->y;

    Load_Move();
}

me::~me()
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
me::Load_Move()
{
    char buffer[50];

    for(int i=0; i < 16; i++)
    {
        ALLEGRO_BITMAP *img;
        sprintf(buffer, "./Smarf/smarf_(%d).png", i+1);
        img = al_load_bitmap(buffer);
        
        if(img)
            moveImg.push_back(img);
    }
}

void
me::Draw()
{
    int offset = direction*4;

    counter = (counter + 1) % draw_frequency;
    if(counter == 0)
        sprite_pos = (sprite_pos + 1) % 4;

    al_draw_scaled_bitmap(moveImg[offset + sprite_pos],
            0, 0,                                // source origin
            al_get_bitmap_width(moveImg[offset + sprite_pos]),     // source width
            al_get_bitmap_height(moveImg[offset + sprite_pos]),    // source height
            circle->x, circle->y,                                // target origin
            grid_height, grid_height,                                // target dimensions
            0                                    // flags
    );

    //al_draw_filled_circle(circle->x, circle->y, circle->r, al_map_rgba(196, 79, 79, 200));
}

void
me::Rememberdirection(int dir){
    remdirection = dir;
}

void
me::change_direction(){
    int xx = circle->x/24;
    int yy = circle->y/24;
    int thegrid = yy*20 + xx;
    int xmod = circle->x%24;
    int ymod = circle->y%24;

    switch(remdirection) {
        case LEFT:
            if(find(path.begin(), path.end(), thegrid-1) != path.end() && ymod == 0){
                direction = remdirection;
            }
            break;
        case UP:
            if(find(path.begin(), path.end(), thegrid-20) != path.end() && xmod == 0){
                direction = remdirection;
            }
            break;
        case RIGHT:
            if(find(path.begin(), path.end(), thegrid+1) != path.end() && ymod == 0){
                direction = remdirection;
            }
            break;
        case DOWN:
            if(find(path.begin(), path.end(), thegrid+20) != path.end() && xmod == 0){
                direction = remdirection;
            }
            break;
    }
}

bool
me::Move()
{
    change_direction();
    int xx = circle->x/24;
    int yy = circle->y/24;
    int thegrid = yy*20 + xx;
    int xmod = circle->x%24;
    int ymod = circle->y%24;
    
    int grid_dir_arrx[] = {1,0,1,0};
    int grid_dir_arry[] = {0,20,0,20};
    switch(direction) {
        case LEFT:
            if(find(path.begin(), path.end(), thegrid-1) != path.end() || xmod != 0){
                // if(circle->x % 2 != 0)  circle->x += 1 * axis_x[direction];
                circle->x += speed * axis_x[direction];
                circle->y += speed * axis_y[direction];
            }
            break;
        case UP:
            if(find(path.begin(), path.end(), thegrid-20) != path.end() || ymod != 0){
                circle->x += speed * axis_x[direction];
                circle->y += speed * axis_y[direction];
            }
            break;
        case RIGHT:
            if(find(path.begin(), path.end(), thegrid+1) != path.end() || xmod != 0){
                circle->x += speed * axis_x[direction];
                circle->y += speed * axis_y[direction];
            }
            break;
        case DOWN:
            if(find(path.begin(), path.end(), thegrid+20) != path.end() || ymod != 0){
                circle->x += speed * axis_x[direction];
                circle->y += speed * axis_y[direction];
            }
            break;
    }
    // // detect if walk too fast and get in a wall
    // int nextgrid = thegrid + grid_dir_arrx[direction]*axis_x[direction] + grid_dir_arry[direction]*axis_y[direction];
    // if(find(path.begin(), path.end(), nextgrid) == path.end()){
    //     if((circle->x-((nextgrid%20)*24)) * (circle->x+23-((nextgrid%20)*24)) < 0 && (circle->y-((nextgrid/20)*23)) * (circle->y+23-((nextgrid/20)*23)) < 0){
    //         circle->x = (thegrid%20)*24;
    //         circle->y = (thegrid/20)*24;
    //     }
    //     else if((circle->x-((nextgrid%20)*24+23)) * (circle->x+23-((nextgrid%20)*24+23)) < 0 && (circle->y-((nextgrid/20)*24+23)) * (circle->y+23-((nextgrid/20)*24+23)) < 0){
    //         circle->x = (thegrid%20)*24;
    //         circle->y = (thegrid/20)*24;
    //     }
    // }
    // //

    if(circle->x < 6 && circle->y == 288 && direction == LEFT){
        circle->x = 456;
    }
    else if(circle->x > 450 && circle->y == 288 && direction == RIGHT){
        circle->x = 0;
    }

    return false;
}

bool 
me::Deteced_overlap(int x, int y, int type){
    if(type == D_GHOST){
        if( abs(x - circle->x) <= 21 && abs(y - circle->y) <= 21) return true;
    }
    else if(type == D_COIN){
        if( abs(x - circle->x) <= 8 && abs(y - circle->y) <= 5) return true;
    }
    else if(type == D_FRUIT){
        if( abs(x - circle->x) <= 21 && abs(y - circle->y) <= 21) return true;
    }
    return false;
}

void
me::transform(int ty){
    if(ty == CHERRY){
        change_ghostescape(true);
    }
    else if(ty == GUAVA){
        change_speed(2);
    }
    else if(ty == STRAW){
        change_timestop(true);
    }
}

void
me::reset_status(){
    circle->x = start_x;
    circle->y = start_y;
    direction = RIGHT; 
    speed = 2;
    timestop = false;
    ghostescape = false;
    direction = RIGHT;
    remdirection = RIGHT;
    sprite_pos = 0;
    counter = 0;
}
