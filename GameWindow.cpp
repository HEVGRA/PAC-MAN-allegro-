#include "GameWindow.h"
#include "global.h"
#include <iostream>
using namespace std;

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))

void
GameWindow::create_next_game(){
    create_game_item();
    human->reset_status();
    for(auto gg : ghostSet){
        gg->reset_status();
    }
    for(int i=0;i<3;i++) menu->set_prop_cnt(0,i);
}

void
GameWindow::create_game_item(){
    stop_timer();
    menu->Draw_pass(levelnow, levelmax);
    if(levelnow == levelmax+1){
        al_stop_sample_instance(normal_backgroundmusic);
        al_play_sample_instance(win_backgroundmusic);
    }
    else if(levelnow == 0){
        al_stop_sample_instance(normal_backgroundmusic);
        al_play_sample_instance(lose_backgroundmusic);
    }
    char buffer[50];
    // coin init
    if(levelnow == 1){
        for(int i=0; i < 4; i++)
        {
            ALLEGRO_BITMAP *img;
            sprintf(buffer, "./Coin/coin_(%d).png", i+1);
            img = al_load_bitmap(buffer);

            if(img)
                coin_moveImg.push_back(img);
        }
    }

    if(!coinSet.empty()) coinSet.clear();
    int testing = 0;
    if(!testing)
    for(auto g : level->ReturnPath()){
        if( !(g>=208 && g<=211 || g>=228 && g<=231 || g>=248 && g<=251 || g>=268 && g<=271||g==240||g==259||g==61||g==78||g==381||g==398||g==310||g==309)){
            Coin* c = new Coin(g, coin_moveImg);
            coinSet.push_back(c);
        }
    }
    else{
        Coin* c = new Coin(435, coin_moveImg);
        coinSet.push_back(c);
    }


    // fruit init
    fruitSet.clear();
    int fruitarrx[] = {1, 18, 1, 18};
    int fruitarry[] = {3, 3, 19, 19};
    for(int i=0;i<4;i++){
        Fruit* f = new Fruit(CHERRY,fruitarrx[i]*grid_width,fruitarry[i]*grid_width);
        fruitSet.push_back(f);
    }
}

void
GameWindow::stop_timer(){
    al_start_timer(pass_timer);
    al_stop_timer(timer);
    al_stop_timer(ghost_goout_timer);
    al_stop_timer(cherry_timer);
    al_stop_timer(guava_timer);
    al_stop_timer(straw_timer);
    al_stop_timer(create_fruit_timer);
}

void
GameWindow::start_timer(){
    al_stop_timer(pass_timer);
    al_start_timer(timer);
    al_start_timer(ghost_goout_timer);
    al_start_timer(create_fruit_timer);
}

void
GameWindow::game_init()
{
    char buffer[50];
    levelnow = 1;
    // level init
    level = new LEVEL(levelnow);
    // human init
    human = new me(level->ReturnPath());
    // ghost init
    if(!ghostSet.empty()) ghostSet.clear();
    for(int i=0;i<4;i++){
        ghost* g = new ghost(level->ReturnPath(),i,level->ReturnCrossroad());
        ghostSet.push_back(g);
    }
    menu = new Menu();
    create_game_item();
}

bool
GameWindow::mouse_hover(int startx, int starty, int width, int height)
{
    if(mouse_x >= startx && mouse_x <= startx + width)
        if(mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}

void
GameWindow::game_play()
{
    int msg;

    srand(time(NULL));

    msg = -1;
    game_reset();
    game_init();

    while(msg != GAME_EXIT &&  msg != GAME_BACKTOMENU)
    {
        msg = game_run();
    }

    show_err_msg(msg);
}

void
GameWindow::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else if(msg == MENU_START)
        game_play();
    else if(msg == GAME_BACKTOMENU)
        menu_play();
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}


GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    ghost_goout_timer = al_create_timer(1.0);
    timer = al_create_timer(1.0 / FPS);
    cherry_timer = al_create_timer(7.0);
    guava_timer = al_create_timer(5.0);
    straw_timer = al_create_timer(5.0);
    create_fruit_timer = al_create_timer(10.0);
    pass_timer = al_create_timer(2);


    if(timer == NULL)
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    // font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",12,0); // load small font
    // Medium_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",24,0); //load medium font
    // Large_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",36,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(ghost_goout_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(cherry_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(guava_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(straw_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(create_fruit_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(pass_timer));

    // game_init();
    menu_init();
}

int
GameWindow::game_run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event();
    }
    return error;
}

int
GameWindow::game_update()
{
    unsigned int i, j;


    return GAME_CONTINUE;
}

void
GameWindow::game_reset()
{
    printf("game_reset\n");
    // reset game and begin

    // monsterSet.clear();


    // selectedTower = -1;
    // lastClicked = -1;
    // Coin_Inc_Count = 0;
    // Monster_Pro_Count = 0;
    // mute = false;
    // redraw = false;
    // menu->Reset();

    // stop sample instance
    // al_stop_sample_instance(backgroundSound);
    // al_stop_sample_instance(startSound);

    // stop timer
    al_stop_timer(timer);
    al_stop_timer(ghost_goout_timer);
    al_stop_timer(cherry_timer);
    al_stop_timer(guava_timer);
    al_stop_timer(straw_timer);
    al_stop_timer(create_fruit_timer);
    al_stop_timer(pass_timer);
    // al_stop_timer(monster_pro);
}

void
GameWindow::game_destroy()
{
    game_reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    al_destroy_timer(timer);
    al_destroy_timer(ghost_goout_timer);
    al_destroy_timer(cherry_timer);
    al_destroy_timer(guava_timer);
    al_destroy_timer(straw_timer);
    al_destroy_timer(create_fruit_timer);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);

    al_destroy_sample(sample);
    for(int i=0;i<5;i++){
        al_destroy_sample_instance(get_coin[i]);
    }
    al_destroy_sample_instance(get_fruit);
    al_destroy_sample_instance(kill_ghost);
    al_destroy_sample_instance(dead);
    al_destroy_sample_instance(normal_backgroundmusic);
    al_destroy_sample_instance(win_backgroundmusic);
    al_destroy_sample_instance(lose_backgroundmusic);

    delete level;
    delete menu;
}

int
GameWindow::process_event()
{
    int i;
    int instruction = GAME_CONTINUE;



    // offset for pause window
    int offsetX = field_width/2 - 200;
    int offsetY = field_height/2 - 200;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {

        if(event.timer.source == timer) {
            redraw = true;
            //check if coins are all eated.
            if(coinSet.empty()){
                levelnow++;
                create_next_game();
                return GAME_NEXTLEVEL;
            }

            // ghost human move
            human->Move();
            for(auto g : ghostSet) g->Move(human, human->get_ghostescape(), human->get_timestop());

            // detect overlap
            for(auto g : ghostSet){
                if(human->Deteced_overlap(g->get_x(), g->get_y(), D_GHOST) && !human->get_ghostescape()){
                    al_play_sample_instance(dead);
                    human->change_life(-1);
                    if(human -> get_Life() <= 0){
                        levelnow = 0;
                        create_next_game();
                        return GAME_NEXTLEVEL;
                    }
                    for(auto gg : ghostSet){
                        gg->reset_status();
                    }
                    human->reset_status();
                    for(int i=0;i<3;i++) menu->set_prop_cnt(0,i);
                    al_stop_timer(cherry_timer);
                    al_stop_timer(guava_timer);
                    al_stop_timer(straw_timer);

                    break;
                }
                else if(human->Deteced_overlap(g->get_x(), g->get_y(), D_GHOST)){
                    al_play_sample_instance(kill_ghost);
                    g->reset_status();
                    human->change_score(10);
                }
            }
            int it = 0;
            for(auto c : coinSet){
                if(human->Deteced_overlap(c->get_x(), c->get_y(), D_COIN)){
                    for(int i=0;i<5;i++){
                        if(!al_get_sample_instance_playing(get_coin[i])){
                            al_play_sample_instance(get_coin[i]);
                            break;
                        }
                    }

                    // delete c;
                    coinSet.erase(coinSet.begin()+it);
                    human->change_score(1);
                    break;
                }
                it++;
            }
            it = 0;
            for(auto f : fruitSet){
                if(human->Deteced_overlap(f->get_x(), f->get_y(), D_FRUIT)){
                    // delete c;
                    al_play_sample_instance(get_fruit);
                    human->transform(f->get_type());
                    if(f->get_type() != CHERRY) {
                        uniform_int_distribution<int> dis(1,2);
                        nextft = dis(gen);
                        al_start_timer(create_fruit_timer);
                    }
                    switch (f->get_type())
                    {
                    case CHERRY:
                        al_stop_timer(cherry_timer);
                        al_start_timer(cherry_timer);
                        for(auto g : ghostSet)  g->set_speed(1);
                        menu->change_prop_cnt(7-menu->get_prop_cnt(CHERRY),CHERRY);
                        break;
                    case GUAVA:
                        al_stop_timer(guava_timer);
                        al_start_timer(guava_timer);
                        menu->change_prop_cnt(5,GUAVA);
                        break;
                    case STRAW:
                        al_stop_timer(straw_timer);
                        al_start_timer(straw_timer);
                        menu->change_prop_cnt(5,STRAW);
                        break;
                    }
                    fruitSet.erase(fruitSet.begin()+it);
                    break;
                }
                it++;
            }
        }

        else if(event.timer.source == ghost_goout_timer){
            for(auto g : ghostSet) g->outtimecntinc();
            if(al_get_timer_started(cherry_timer)){
                if(menu->get_prop_cnt(CHERRY) > 0)
                    menu->change_prop_cnt(-1,CHERRY);
            }
            if(al_get_timer_started(guava_timer)){
                if(menu->get_prop_cnt(GUAVA) > 0)
                    menu->change_prop_cnt(-1,GUAVA);
            }
            if(al_get_timer_started(straw_timer)){
                if(menu->get_prop_cnt(STRAW) > 0)
                    menu->change_prop_cnt(-1,STRAW);
            }
        }

        else if(event.timer.source == cherry_timer){
            al_stop_timer(cherry_timer);
            fruitnow[0] = false;
            human->change_ghostescape(false);
            for(auto g : ghostSet) g->set_speed(2);
        }
        else if(event.timer.source == guava_timer){
            fruitnow[1] = false;
            al_stop_timer(guava_timer);
            human->change_speed(-2);
        }
        else if(event.timer.source == straw_timer){
            fruitnow[2] = false;
            al_stop_timer(straw_timer);
            human->change_timestop(false);
        }
        else if(event.timer.source == create_fruit_timer){
            al_stop_timer(create_fruit_timer);
            cur_fruit = new Fruit(nextft, (9.5) * grid_width, (15) * grid_height);
            fruitSet.push_back(cur_fruit);
            human->change_timestop(false);
        }

        else if(event.timer.source == pass_timer){
            if(levelnow == levelmax+1) return GAME_BACKTOMENU;
            if(levelnow == 0) return GAME_BACKTOMENU;
            start_timer();
        }

    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
                human->Rememberdirection(UP);
                break;
            case ALLEGRO_KEY_DOWN:
                human->Rememberdirection(DOWN);
                break;
            case ALLEGRO_KEY_LEFT:
                human->Rememberdirection(LEFT);
                break;
            case ALLEGRO_KEY_RIGHT:
                human->Rememberdirection(RIGHT);
                break;
            case ALLEGRO_KEY_P:
                if(al_get_timer_started(timer)){
                    if(al_get_timer_started(cherry_timer)) fruitnow[0] = true;
                    if(al_get_timer_started(guava_timer)) fruitnow[1] = true;
                    if(al_get_timer_started(straw_timer)) fruitnow[2] = true;
                    al_stop_timer(timer);
                    al_stop_timer(ghost_goout_timer);
                    al_stop_timer(cherry_timer);
                    al_stop_timer(guava_timer);
                    al_stop_timer(straw_timer);
                    al_stop_timer(create_fruit_timer);
                    al_flush_event_queue(event_queue);
                }
                else{
                    al_start_timer(timer);
                    al_resume_timer(ghost_goout_timer);
                    if(fruitnow[0]) al_resume_timer(cherry_timer);
                    if(fruitnow[1]) al_resume_timer(guava_timer);
                    if(fruitnow[2]) al_resume_timer(straw_timer);
                    al_resume_timer(create_fruit_timer);
                }
                break;
        }
    }
    if(redraw) {
        draw_running_map();
        redraw = false;
    }
    return instruction;
}

void
GameWindow::draw_running_map()
{
    unsigned int i, j;

    al_clear_to_color(al_map_rgb(100, 100, 100));

    for(i = 0; i < field_height/grid_height; i++)
    {
        for(j = 0; j < field_width/grid_width; j++)
        {
            char buffer[100];
            sprintf(buffer, "%d", i*field_width/grid_height + j);
            if(level->isRoad(i*field_width/grid_height + j)) {
                al_draw_filled_rectangle(j*grid_width, i*grid_height, j*grid_height+grid_height, i*grid_height+grid_height, al_map_rgb(76, 23, 8));
            }
        }
    }

    // menu->Draw();



    bool boo[7] = {NULL};
    boo[0] = al_get_timer_started(ghost_goout_timer);
    boo[1] = al_get_timer_started(timer);
    boo[2] = al_get_timer_started(cherry_timer);
    boo[3] = al_get_timer_started(guava_timer);
    boo[4] = al_get_timer_started(straw_timer);
    boo[5] = al_get_timer_started(create_fruit_timer);
    boo[6] = al_get_timer_started(pass_timer);
    menu->Draw(human->get_Score(),human->get_Life(),nextft,levelnow, boo);
    for(auto c : coinSet) c->Draw();
    for(auto f : fruitSet) f->Draw();
    for(auto g : ghostSet) g->Draw();
    human->Draw();

    // vision control
    if(levelnow == 2){
        al_draw_filled_rectangle(0, 0, field_width, human->get_y() - vision_size, al_map_rgb(0, 0, 0));
        al_draw_filled_rectangle(0, 0, human->get_x() - vision_size, field_height, al_map_rgb(0, 0, 0));
        al_draw_filled_rectangle(0, min(human->get_y() + vision_size + grid_height, field_height), field_width, field_height, al_map_rgb(0, 0, 0));
        al_draw_filled_rectangle(min(human->get_x() + vision_size + grid_width,field_width), 0, field_width, field_height, al_map_rgb(0, 0, 0));
    }

    al_flip_display();
}


// MENU setting
void
GameWindow::menu_init()
{
    menu_background = al_load_bitmap("./BeginningCover/menu_Background.png");
    about_background = al_load_bitmap("./BeginningCover/About_Background.png");
    menu_btn[0] = al_load_bitmap("./BeginningCover/Start.png");
    menu_btn_s[0] = al_load_bitmap("./BeginningCover/Start_shadow.png");
    menu_btn[1] = al_load_bitmap("./BeginningCover/About.png");
    menu_btn_s[1] = al_load_bitmap("./BeginningCover/About_shadow.png");
    menu_btn[2] = al_load_bitmap("./BeginningCover/Leave.png");
    menu_btn_s[2] = al_load_bitmap("./BeginningCover/Leave_shadow.png");

    icon = al_load_bitmap("./Smarf/smarf_(1).png");
    background = al_load_bitmap("./BeginningCover/menu_Background.png");

    al_set_display_icon(display, icon);
    al_set_window_title(display, "PAC-MAN");


    al_reserve_samples(11);

    sample = al_load_sample("./Music/get_coin.mp3");

    for(int i=0;i<5;i++){
        get_coin[i] = al_create_sample_instance(sample);
        al_set_sample_instance_playmode(get_coin[i], ALLEGRO_PLAYMODE_ONCE);
        al_attach_sample_instance_to_mixer(get_coin[i], al_get_default_mixer());
    }


    sample = al_load_sample("./Music/get_fruit.mp3");
    get_fruit = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(get_fruit, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(get_fruit, al_get_default_mixer());

    sample = al_load_sample("./Music/kill_ghost.mp3");
    kill_ghost = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(kill_ghost, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(kill_ghost, al_get_default_mixer());

    sample = al_load_sample("./Music/dead.mp3");
    dead = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(dead, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(dead, al_get_default_mixer());

    sample = al_load_sample("./Music/bgm.mp3");
    normal_backgroundmusic = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(normal_backgroundmusic, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(normal_backgroundmusic, al_get_default_mixer());

    sample = al_load_sample("./Music/win_bgm.mp3");
    win_backgroundmusic = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(win_backgroundmusic, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(win_backgroundmusic, al_get_default_mixer());

    sample = al_load_sample("./Music/lose_bgm.mp3");
    lose_backgroundmusic = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(lose_backgroundmusic, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(lose_backgroundmusic, al_get_default_mixer());
    al_set_sample_instance_gain(lose_backgroundmusic, 10);
}

void
GameWindow::menu_play()
{
    int msg;

    srand(time(NULL));

    msg = -1;

    while(msg != GAME_EXIT && msg != MENU_START)
    {
        msg = game_menu_run();
    }

    show_err_msg(msg);
}

int
GameWindow::game_menu_run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event_menu();
    }
    return error;
}

int
GameWindow::process_event_menu()
{
    if(!(al_get_sample_instance_playing(normal_backgroundmusic))){
        al_stop_sample_instance(lose_backgroundmusic);
        al_stop_sample_instance(win_backgroundmusic);
        al_play_sample_instance(normal_backgroundmusic);
    }

    int i;
    int instruction = GAME_CONTINUE;

    // offset for pause window
    int offsetX = field_width/2 - 200;
    int offsetY = field_height/2 - 200;

    al_wait_for_event(event_queue, &event);

    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }

    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if(event.mouse.button == 1) {
            if(onWhichBtn!=0){
                if(onWhichBtn == 1){
                    return MENU_START;
                }
                else if(onWhichBtn == 2){
                    isAboutPage = 1;
                }
                else if(onWhichBtn == 3){
                    return GAME_EXIT;
                }
                else if(onWhichBtn == 4){
                    isAboutPage = 0;
                }
            }
        }
    }

    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(isAboutPage == 0){
            if(mouse_hover(100, 375, btn_width, btn_height)){
                onWhichBtn = 1;
            }
            else if(mouse_hover(325, 375, btn_width, btn_height)){
                onWhichBtn = 2;
            }
            else if(mouse_hover(550, 375, btn_width, btn_height)){
                onWhichBtn = 3;
            }
            else{
                onWhichBtn = 0;
            }
        }
        else{
            if(mouse_hover(300, 500, btn_width, btn_height)){
                onWhichBtn = 4;
            }
            else onWhichBtn = 0;
        }
    }

    draw_menu();

    return instruction;
}

void
GameWindow::draw_menu()
{
    cout<<"draw_menu\n";
    unsigned int i, j;

    al_clear_to_color(al_map_rgb(100, 100, 100));
    if(isAboutPage){
        al_draw_bitmap(about_background, 0, 0, 0);
        if(onWhichBtn == 4){
            al_draw_bitmap(menu_btn_s[2], 300, 500, 0);
        }
        else{
            al_draw_bitmap(menu_btn[2], 300, 500, 0);
        }
    }
    else {
        al_draw_bitmap(menu_background, 0, 0, 0);
        if(onWhichBtn == 0){
            for(int i=0;i<3;i++){
                al_draw_bitmap(menu_btn[i], 100+i*225, 375, 0);
            }
        }
        else{
            for(int i=0;i<3;i++){
                if(i+1==onWhichBtn) al_draw_bitmap(menu_btn_s[i], 100+i*225, 375, 0);
                else al_draw_bitmap(menu_btn[i], 100+i*225, 375, 0);
            }
        }
    }

    al_flip_display();
}


