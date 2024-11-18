#include "Menu.h"

Menu::Menu()
{
    char buffer[50];
    sprintf(buffer, "./Menu/smarf_(1).png");
    smarf= al_load_bitmap(buffer);

    sprintf(buffer, "./Menu/score_(1).png");
    score= al_load_bitmap(buffer);

    for(int i=0;i<3;i++){
        sprintf(buffer, "./Menu/fruit_(%d).png",i+1);
        allfruit.push_back(al_load_bitmap(buffer));
    }

    sprintf(buffer, "./Menu/gamebackground.png");
    game_background = al_load_bitmap(buffer);

    sprintf(buffer, "./Menu/rightbackground.png");
    right_background = al_load_bitmap(buffer);

    sprintf(buffer, "./Menu/level_1.png");
    level_1 = al_load_bitmap(buffer);

    sprintf(buffer, "./Menu/next_level.png");
    next_level = al_load_bitmap(buffer);

    sprintf(buffer, "./Menu/you_win.png");
    you_win = al_load_bitmap(buffer);

    sprintf(buffer, "./Menu/you_lose.png");
    you_lose = al_load_bitmap(buffer);

    menuFont = al_load_ttf_font("./Menu/pirulen.ttf", 35, 0);
    propFont = al_load_ttf_font("./Menu/pirulen.ttf", 20, 0);
    passFont = al_load_ttf_font("./Menu/pirulen.ttf", 50, 0);
}

Menu::~Menu()
{
    al_destroy_bitmap(smarf);
    al_destroy_bitmap(score);
    for(int i=0;i<3;i++)
        al_destroy_bitmap(allfruit[i]);
    al_destroy_font(menuFont);

    allfruit.clear();
}


void
Menu::Draw(int sco, int life, int nextfruit, int lev, bool* bo)
{
    char buffer[50];

    al_draw_bitmap(game_background,0,0,0);
    al_draw_bitmap(right_background,480,0,0);

    //level
    sprintf(buffer, "LEVEL%d", lev);
    al_draw_text(propFont, al_map_rgb(255, 255, 255), 590 , 75, 0, buffer);

    // debug
    // sprintf(buffer, "%d %d %d %d %d %d %d", bo[0], bo[1], bo[2], bo[3], bo[4], bo[5], bo[6]);
    // al_draw_text(propFont, al_map_rgb(255, 255, 255), 550 , 95, 0, buffer);

    //score
    sprintf(buffer, "%d", sco);
    al_draw_text(menuFont, al_map_rgb(255, 255, 255), 620 , 185, 0, buffer);

    //prop
    al_draw_scaled_bitmap(allfruit[nextfruit],
            0, 0,                                // source origin
            al_get_bitmap_width(allfruit[nextfruit]),     // source width
            al_get_bitmap_height(allfruit[nextfruit]),    // source height
            540, 340,                                // target origin
            grid_height*1.2, grid_height*1.7,                                // target dimensions
            0                                    // flags
    );
    for(int i=0;i<3;i++){
        sprintf(buffer, "%d", prop_cnt[i]);
        al_draw_text(propFont, al_map_rgb(255, 255, 255), 650 , 310 + 35 * i, 0, buffer);
    }

    //life
    for(int i=0;i<life; i++){
        al_draw_scaled_bitmap(smarf,
            0, 0,                                // source origin
            al_get_bitmap_width(smarf),     // source width
            al_get_bitmap_height(smarf),    // source height
            540 + 30*i, 500,                                // target origin
            grid_height*1.2, grid_height*1.2,                                // target dimensions
            0                                    // flags
        );
    }

}

void
Menu::Draw_pass(int lev, int levmax)
{
    char buffer[50];
    if(lev == 1){
        al_draw_bitmap(level_1,0,0,0);
    }
    else if(lev == 0){
        al_draw_bitmap(you_lose,0,0,0);
    }
    else if(lev == levmax+1){
        al_draw_bitmap(you_win,0,0,0);
    }
    else if(lev != levmax+1){
        al_draw_bitmap(next_level,0,0,0);
    }

    al_flip_display();
}
