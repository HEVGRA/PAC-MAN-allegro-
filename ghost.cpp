#include "ghost.h"


using namespace std;

const int axis_x[] = {-1, 0, 1, 0};
const int axis_y[] = {0, -1, 0, 1};

// set counter frequency of drawing moving animation
const int draw_frequency = 5;

ghost::ghost(vector<int> path, int t, set<int> cross)
{
    this->path = path;
    this->crossroad = cross;
    this->step = 0;

    // default direction is right
    type = t;
    outtime = t*4;

    circle = new Circle;

    if(type == 0){
        circle->x = (9) * grid_width;
        circle->y = (11) * grid_height;
    }
    if(type == 1){
        circle->x = (9 + 1) * grid_width;
        circle->y = (11) * grid_height;
    }
    if(type == 2){
        circle->x = (9) * grid_width;
        circle->y = (11 + 1) * grid_height;
    }
    if(type == 3){
        circle->x = (9 + 1) * grid_width;
        circle->y = (11 + 1) * grid_height;
    }

    circle->r = grid_width/2;

    start_x = circle->x;
    start_y = circle->y;

    Load_Move();
}

ghost::~ghost()
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
ghost::Load_Move()
{
    char buffer[50];

    for(int i=0; i < 5; i++)
    {
        ALLEGRO_BITMAP *img;
        sprintf(buffer, "./Ghost/ghost_(%d).png", 5*type+i+1);
        img = al_load_bitmap(buffer);

        if(img)
            moveImg.push_back(img);
    }
    ALLEGRO_BITMAP *img;
    sprintf(buffer, "./Ghost/ghost_(%d).png", 21);
    img = al_load_bitmap(buffer);

    if(img)
        moveImg.push_back(img);
}

void
ghost::Draw()
{
    int w, h;

    // get height and width of sprite bitmap
    if(isescape){
        al_draw_scaled_bitmap(moveImg[5],
                0, 0,                                // source origin
                al_get_bitmap_width(moveImg[5]),     // source width
                al_get_bitmap_height(moveImg[5]),    // source height
                circle->x, circle->y,                                // target origin
                grid_height, grid_height,                                // target dimensions
                0                                    // flags
        );
    }
    else{
        al_draw_scaled_bitmap(moveImg[direction],
                0, 0,                                // source origin
                al_get_bitmap_width(moveImg[direction]),     // source width
                al_get_bitmap_height(moveImg[direction]),    // source height
                circle->x, circle->y,                                // target origin
                grid_height, grid_height,                                // target dimensions
                0                                    // flags
        );
    }
}

void
ghost::Move(me* m, bool escape, bool timestop)
{
    int gx = circle->x;
    int gy = circle->x;
    int mx = m->get_x();
    int my = m->get_y();
    int xx = circle->x/24;
    int yy = circle->y/24;
    int thegrid = yy*20 + xx;
    int xmod = circle->x%24;
    int ymod = circle->y%24;
    isescape = escape;
    if(outtime <= outtimecnt){
        if(!isout){
            direction = UP;
            if(circle->x == (9 + type%2)*24 && circle->y == 8*24){
                isout = 1;
                direction = (type%2)*2;
            }
        }
        else{
            if(xmod == 0 && ymod == 0){
                if(circle->x < 6 && circle->y == 288 && direction == LEFT){
                    circle->x = 456;
                }
                else if(circle->x > 450 && circle->y == 288 && direction == RIGHT){
                    circle->x = 0;
                }
                else if(find(path.begin(), path.end(), thegrid-1) == path.end() && direction == LEFT){
                    if(find(path.begin(), path.end(), thegrid-20) == path.end()) direction = DOWN;
                    else direction = UP;
                }
                else if(find(path.begin(), path.end(), thegrid-20) == path.end() && direction == UP){
                    if(find(path.begin(), path.end(), thegrid+1) == path.end()) direction = LEFT;
                    else direction = RIGHT;
                }
                else if(find(path.begin(), path.end(), thegrid+1) == path.end() && direction == RIGHT){
                    if(find(path.begin(), path.end(), thegrid-20) == path.end()) direction = DOWN;
                    else direction = UP;
                }
                else if(find(path.begin(), path.end(), thegrid+20) == path.end() && direction == DOWN){
                    if(find(path.begin(), path.end(), thegrid+1) == path.end()) direction = LEFT;
                    else direction = RIGHT;
                }
                else if(crossroad.count(thegrid) > 0){
                    if(!escape){
                        vector<int> rand;
                        int moreadd = 5;
                        int divider = 20;
                        if(find(path.begin(), path.end(), thegrid-1) != path.end() && direction != RIGHT){
                            rand.push_back(LEFT);
                            if(gx - mx > 0){
                                int cnt = (gx - mx) % divider + moreadd;
                                for(int ii=0;ii<cnt;ii++) rand.push_back(LEFT);
                            }
                        }
                        if(find(path.begin(), path.end(), thegrid-20) != path.end() && direction != DOWN){
                            rand.push_back(UP);
                            if(gy - my > 0){
                                int cnt = (gy - my) % divider + moreadd;
                                for(int ii=0;ii<cnt;ii++) rand.push_back(UP);
                            }
                        }
                        if(find(path.begin(), path.end(), thegrid+1) != path.end() && direction != LEFT){
                            rand.push_back(RIGHT);
                            if(gx - mx < 0){
                                int cnt = -(gx - mx) % divider + moreadd;
                                for(int ii=0;ii<cnt;ii++) rand.push_back(RIGHT);
                            }
                        }
                        if(find(path.begin(), path.end(), thegrid+20) != path.end() && direction != UP){
                            rand.push_back(DOWN);
                            if(gy - my < 0){
                                int cnt = -(gy - my) % divider + moreadd;
                                for(int ii=0;ii<cnt;ii++) rand.push_back(DOWN);
                            }
                        }
                    
                        uniform_int_distribution<int> dis(0,rand.size()-1);
                        direction = rand[dis(gen)];
                    }
                    else{
                        vector<int> rand;
                        if(find(path.begin(), path.end(), thegrid-1) != path.end() && direction != RIGHT) rand.push_back(LEFT);
                        if(find(path.begin(), path.end(), thegrid-20) != path.end() && direction != DOWN) rand.push_back(UP);
                        if(find(path.begin(), path.end(), thegrid+1) != path.end() && direction != LEFT) rand.push_back(RIGHT);
                        if(find(path.begin(), path.end(), thegrid+20) != path.end() && direction != UP) rand.push_back(DOWN);
                        std::uniform_int_distribution<int> dis(0,rand.size()-1);
                        direction = rand[dis(gen)];
                    }
                }
            }   
        }
        if(!timestop){
            if(circle->x % 2 != 0)  circle->x += 1 * axis_x[direction];
            else circle->x += speed * axis_x[direction];
            if(circle->y % 2 != 0)  circle->y += 1 * axis_y[direction];
            else circle->y += speed * axis_y[direction];
        }
    }
}



