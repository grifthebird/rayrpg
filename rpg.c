#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int window_width = 800;
int window_height = 450;

int enter_flag = 0;
int left_flag = 0;
int right_flag = 0;
int up_flag = 0;
int down_flag = 0;

typedef struct info { // object holding all relevant info for functions
    int screen_type; // current screen in game
    int class; // class player chose
    int map[400]; // 20x20 map for the game, (+- 1) traverses columns, (+- 20) traverses rows
    int map_pos;
    int roll_state;
    int roll_flag;
    int moves;
    int generated;
    int map_start;
    int map_end;
    int atk;
    int def;
    int defending;
    int spd;
    int vit_max;
    int vit;
    int enemy_vit;
    int enemy_vit_max;
    int option_select;
    int turn;
    int special_cooldown;
    Camera2D map_cam;
    Rectangle cam_target;
    Texture2D my_texture;
} info;

void init_info(info * rpg_info) { // intialize info for game
    Image image = LoadImage("images/Bird.jpg");
    rpg_info->screen_type = 0;
    rpg_info->class = 2; 
    rpg_info->map_pos = -1;
    rpg_info->roll_state = 0;
    rpg_info->roll_flag = 1;
    rpg_info->moves = 0;
    rpg_info->atk = 1;
    rpg_info->def = 1;
    rpg_info->defending = 0;
    rpg_info->spd = 1;
    rpg_info->vit_max = 10;
    rpg_info->vit = 3;
    rpg_info->enemy_vit = GetRandomValue(10, 50);
    rpg_info->enemy_vit_max = rpg_info->enemy_vit;
    rpg_info->option_select = 1;
    rpg_info->turn = 1; // GetRandomValue(0, 1);
    rpg_info->special_cooldown = 0;
    rpg_info->generated = 0;
    rpg_info->my_texture = LoadTextureFromImage(image);
    UnloadImage(image);
    
    // initialize map camera
    rpg_info->cam_target = (Rectangle) { 0, 0, 0, 0 };
    rpg_info->map_cam.target = (Vector2) { rpg_info->cam_target.x, rpg_info->cam_target.y }; 
    rpg_info->map_cam.offset = (Vector2) { (window_width / 2.0f), (window_height / 2.0f) };
    rpg_info->map_cam.rotation = (float) GetRandomValue(-5, 5);
    rpg_info->map_cam.zoom = 1.0f;
}

void title_screen(info * rpg_info) { // draw title screen
    // draw title screen
    // draw bg
    ClearBackground(DARKGRAY);
    
    // my logo
    //DrawRectangle(9, (window_height - 16) - 35 , 38, 36, BLACK);
    DrawRectangleGradientEx((Rectangle) { 8, (window_height - 16) - 36, 40, 38 }, 
    (Color) { 124, 236, 190, 255 },
    (Color) { 0, 144, 109, 255 },
    (Color) { 108, 175, 230, 255 },
    (Color) { 66, 24, 126, 255 });
    DrawTexture(rpg_info->my_texture, 10, (window_height - 16) - 34, WHITE);
    
    // raylib logo
    DrawRectangle(window_width - 80, window_height - 80, 64, 64, BLACK);
    DrawRectangle(window_width - 76, window_height - 76, 56, 56, RAYWHITE);
    DrawText("raylib", window_width - 54, window_height - 34, 12, BLACK);
    DrawText("powered by:", window_width - 80, window_height - 92, 10, RAYWHITE);
    
    // enter prompt
    DrawText("Press enter to start.", (window_width / 2) - (12 * 5), window_height - 28, 12, RAYWHITE);
    
    // rayrpg title
    DrawText("r", (window_width / 2) - 75, (window_height / 2), 40, RAYWHITE);
    DrawText("a", (window_width / 2) - 45, (window_height / 2), 40, RAYWHITE);
    DrawText("y", (window_width / 2) - 15, (window_height / 2), 40, RAYWHITE);
    DrawText("r", (window_width / 2) + 15, (window_height / 2), 40, RED);
    DrawText("p", (window_width / 2) + 45, (window_height / 2), 40, GREEN);
    DrawText("g", (window_width / 2) + 75, (window_height / 2), 40, DARKBLUE);

    if (!IsKeyDown(KEY_ENTER)) {
        enter_flag = 0;
    }
    else if (IsKeyDown(KEY_ENTER)) {
        if (!enter_flag) {
            enter_flag = 1;
            rpg_info->screen_type = 1; // advance to next screen
        }
    }
}

void class_screen(info * rpg_info) { // draw class screen
    char pos[128];
    sprintf(pos, "mouse: (%d, %d)", GetMouseX(), GetMouseY());
    ClearBackground(DARKGRAY);
    switch (rpg_info->class) {
        case (0): { // red select
            ClearBackground((Color){ 100, 15, 25, 255 });
            DrawText("The Warrior: High attack, Low vitality, Average speed & defense", (window_width / 2) - 188, window_height - 62, 12, RAYWHITE);
            // draw options
            DrawRectangle((window_width / 4) - 76, ((window_height - 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle((window_width / 4) - 75, (window_height - 200) / 2, 150, 200, DARKBLUE);
            DrawRectangle(((window_width - 150) / 2) - 1, ((window_height- 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle((window_width - 150) / 2, (window_height- 200) / 2, 150, 200, RED);
            DrawRectangle((((3 * window_width) / 4) - 75) - 1, ((window_height - 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle(((3 * window_width) / 4) - 75, (window_height - 200) / 2, 150, 200, GREEN);
            break;
        }
        case (1): { // green select
            ClearBackground((Color){ 0, 100, 20, 255 });
            DrawText("The Tank: Very high defense, High vitality, Average attack, low speed", (window_width / 2) - 210, window_height - 62, 12, RAYWHITE);
            // draw options
            DrawRectangle((window_width / 4) - 76, ((window_height - 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle((window_width / 4) - 75, (window_height - 200) / 2, 150, 200, RED);
            DrawRectangle(((window_width - 150) / 2) - 1, ((window_height- 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle((window_width - 150) / 2, (window_height- 200) / 2, 150, 200, GREEN);
            DrawRectangle((((3 * window_width) / 4) - 75) - 1, ((window_height - 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle(((3 * window_width) / 4) - 75, (window_height - 200) / 2, 150, 200, DARKBLUE);
            break;
        }
        case (2): { // blue select
            ClearBackground((Color){ 0, 30, 105, 255 });
            DrawText("The Mage: High vitality, Low attack, Average speed and defense", (window_width / 2) - 191, window_height - 62, 12, RAYWHITE);
            // draw options
            DrawRectangle((window_width / 4) - 76, ((window_height - 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle((window_width / 4) - 75, (window_height - 200) / 2, 150, 200, GREEN);
            DrawRectangle(((window_width - 150) / 2) - 1, ((window_height- 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle((window_width - 150) / 2, (window_height- 200) / 2, 150, 200, DARKBLUE);
            DrawRectangle((((3 * window_width) / 4) - 75) - 1, ((window_height - 200) / 2) - 1, 152, 202, BLACK);
            DrawRectangle(((3 * window_width) / 4) - 75, (window_height - 200) / 2, 150, 200, RED);
            break;
        }
        default: { // none select
            break;
        }
    }
    
    DrawText(pos, 20, 20, 12, RAYWHITE);
    
    if (rpg_info->class == -1) {
        rpg_info->class = 1;
    }
    
    if (IsKeyDown(KEY_BACKSPACE)) {
        rpg_info->screen_type = 0;
    }
    
    if (!IsKeyDown(KEY_ENTER)) {
        enter_flag = 0;
    }
    if (IsKeyDown(KEY_ENTER)) {
        if (!enter_flag) {
            enter_flag = 1;
            rpg_info->screen_type = 2;
            switch (rpg_info->class) {
                case (0): {
                    rpg_info->atk = 7;
                    rpg_info->def = 5;
                    rpg_info->spd = 5;
                    rpg_info->vit = 35;
                    rpg_info->vit_max = 35;
                    break;
                }
                case (1): {
                    rpg_info->atk = 5;
                    rpg_info->def = 9;
                    rpg_info->spd = 3;
                    rpg_info->vit = 70;
                    rpg_info->vit_max = 70;
                    break;
                }
                case (2): {
                    rpg_info->atk = 3;
                    rpg_info->def = 5;
                    rpg_info->spd = 5;
                    rpg_info->vit = 70;
                    rpg_info->vit_max = 70;
                    break;
                }
            }
        }
    }
    
    if (!IsKeyDown(KEY_LEFT)) {
        left_flag = 0;
    } 
    else if (IsKeyDown(KEY_LEFT)) {
        if (!left_flag) {
            left_flag = 1;
            if (rpg_info->class == 0) {
                rpg_info->class = 2;
            }
            else {
                rpg_info->class--;
            }
        }
    }
    
    if (!IsKeyDown(KEY_RIGHT)) {
        right_flag = 0;
    } 
    else if (IsKeyDown(KEY_RIGHT)) {
        if (!right_flag) {
            right_flag = 1;
            if (rpg_info->class == 2) {
                rpg_info->class = 0;
            }
            else {
                rpg_info->class++;
            }
        }
    }
}

void draw_player(info * rpg_info) { // draw player on map
    // set player color
    Color player_color;
    switch (rpg_info->class) {
        case (0): {
            player_color = RED;
            break;
        }
        case (1): {
            player_color = GREEN;
            break;
        }
        case (2): {
            player_color = DARKBLUE;
            break;
        }
    }
    
    // call to find start, add later
    if (rpg_info->map_pos == -1) {
        for (int i = 0; i < 400; i++) {
            if (rpg_info->map[i] == 2) {
                rpg_info->map_pos = i;
                rpg_info->cam_target.x = (70 * (i % 20)) + (window_width / 2);
                rpg_info->cam_target.y = (70 * (i / 20)) + (window_height / 2);
            }
        }
    }
    
    DrawRectangle(((window_width / 2) + (70 * (rpg_info->map_pos % 20))) + 20, ((window_height / 2) + (70 * (rpg_info->map_pos / 20))) +20, 20, 20, player_color);
}

void hook(info * rpg_info, int start) {
    int length = GetRandomValue(2, 20);
    int direction = GetRandomValue(0, 3);
    rpg_info->map[start] = 1;
    for (int i = 0; i < length / 2; i++) {
        switch (direction) {
            case (0): { // right
                if (rpg_info->map[start + i] == 2 || rpg_info->map[start + i] == 3) {
                    break;
                }
                rpg_info->map[start + i] = 1;
                break;
            }
            case (1): { // down
                if (rpg_info->map[start + (20 * i)] == 2 || rpg_info->map[start + (20 * i)] == 3) {
                    break;
                }
                rpg_info->map[start + (20 * i)] = 1;
                break;
            }
            case (2): { // left
                if (rpg_info->map[start - i] == 2 || rpg_info->map[start - i] == 3) {
                    break;
                }
                rpg_info->map[start - i] = 1;
                break;
            }
            case (3): { // up
                if (rpg_info->map[start - (20 * i)] == 2 || rpg_info->map[start - (20 * i)] == 3) {
                    break;
                }
                rpg_info->map[start - (20 * i)] = 1;
                break;
            }
        }
    }
    
    for (int i = 0; i < length / 2; i++) {
        switch (direction) {
            case (0): { // down
                if (rpg_info->map[start + (20 * i)] == 2 || rpg_info->map[start + (20 * i)] == 3) {
                    break;
                }
                rpg_info->map[start + (20 * i)] = 1;
                break;
            }
            case (1): { // left
                if (rpg_info->map[start - i] == 2 || rpg_info->map[start - i] == 3) {
                    break;
                }
                rpg_info->map[start + i] = 1;
                break;
            }
            case (2): { // up
                if (rpg_info->map[start - (20 * i)] == 2 || rpg_info->map[start - (20 * i)] == 3) {
                    break;
                }
                rpg_info->map[start - (20 * i)] = 1;
                break;
            }
            case (3): { // right
                if (rpg_info->map[start + i] == 2 || rpg_info->map[start + i] == 3) {
                    break;
                }
                rpg_info->map[start - i] = 1;
                break;
            }
        }
    }
}

void fork(info * rpg_info, int start) {
    int direction = GetRandomValue(0, 1);
    int branch = GetRandomValue(0, 1);
    rpg_info->map[start] = 1;
    if (direction) { // horizontal
        rpg_info->map[start + 1] = 1;
        rpg_info->map[start - 1] = 1;
        if (branch) {
            hook(rpg_info, start + 1);
        }
        else {
            hook(rpg_info, start - 1);
        }
    }
    else { // vertical
        rpg_info->map[start + 20] = 1;
        rpg_info->map[start - 20] = 1;
        if (branch) {
            hook(rpg_info, start + 20);
        }
        else {
            hook(rpg_info, start - 20);
        }
    }
}
    
void generate_map(info * rpg_info) { // test map generation function
    int current_tile;
    if (!rpg_info->generated) {
        // clear array
        for (int i = 0; i < 400; i++) {
            rpg_info->map[i] = 0;
        }
        
        // path generation code
        // start and end
        rpg_info->map_start = GetRandomValue(0, 400);
        rpg_info->map[rpg_info->map_start] = 2;
        rpg_info->map_end = GetRandomValue(0, 400);
        rpg_info->map[rpg_info->map_end] = 3;
        current_tile = rpg_info->map_start;
        rpg_info->generated = 1;
        
        // draw simple path from start to end
        while ((current_tile % 20) != (rpg_info->map_end % 20)) { // draw horizontal
            if ((current_tile % 20) >= (rpg_info->map_end % 20)) {
                current_tile -= 1;
                if (rpg_info->map[current_tile] == 3) {
                    break;
                }
                if (GetRandomValue(0, 10) > 8) {
                    rpg_info->map[current_tile] = 4;
                }
                /*
                else if (GetRandomValue(0, 20) < 5) {
                    fork(rpg_info, current_tile);
                }
                */
                else {
                    rpg_info->map[current_tile] = 1;
                }
            }
            else if ((current_tile % 20) <= (rpg_info->map_end % 20)) {
                current_tile += 1;
                if (rpg_info->map[current_tile] == 3) {
                    break;
                }
                if (GetRandomValue(0, 10) > 8) {
                    rpg_info->map[current_tile] = 4;
                }
                /*
                else if (GetRandomValue(0, 20) < 5) {
                    fork(rpg_info, current_tile);
                }
                */
                else {
                    rpg_info->map[current_tile] = 1;
                }
            }
        }
        
        while ((current_tile / 20) != (rpg_info->map_end / 20)) { // draw vertical
            if ((current_tile / 20) >= (rpg_info->map_end / 20)) {
                if (rpg_info->map[current_tile] == 2) {
                    current_tile -= 20;
                }
                if (GetRandomValue(0, 10) > 8) {
                    rpg_info->map[current_tile] = 4;
                }
                else {
                    rpg_info->map[current_tile] = 1;
                }
                current_tile -= 20;
            }
            else if ((current_tile / 20) <= (rpg_info->map_end / 20)) {
                if (rpg_info->map[current_tile] == 2) {
                    current_tile += 20;
                }
                if (GetRandomValue(0, 10) > 8) {
                    rpg_info->map[current_tile] = 4;
                }
                else {
                    rpg_info->map[current_tile] = 1;
                }
                current_tile += 20;
            }
        }
    }
    
    // dummy map
    /*
    rpg_info->map[0] = 2; // start tile;
    
    // starting corridor
    for (int i = 1; i < 12; i++) {
        rpg_info->map[i] = 1;
    }
    
    // long hallway into dead-end
    for (int i = 0; i < 10; i++) {
        rpg_info->map[12 + (20 * i)] = 1;
    }
    
    // optional dead-end (right side)
    for (int i = 0; i < 4; i++) {
        rpg_info->map[72 + i] = 1;
    }
    rpg_info->map[55] = 1;
    
    // main path (left junction)
    for (int i = 0; i < 5; i++) {
        rpg_info->map[132 - i] = 1;
    }
    rpg_info->map[148] = 1; // dead-end
    
    for (int i = 0; i < 4; i++) { // main path
        rpg_info->map[130 - (20 * i)] = 1;
    }
    
    for (int i = 0; i < 6; i++) { // main path
        rpg_info->map[50 - i] = 1;
    }
    
    for (int i = 0; i < 3; i++) { // main path
        rpg_info->map[44 + (20 * i)] = 1;
    }
    
    for (int i = 0; i < 5; i++) { // main path
        rpg_info->map[84 + i] = 1;
    }
    
    for (int i = 0; i < 5; i++) { // main path with dead-end
        rpg_info->map[84 + i] = 1;
    }
    
    for (int i = 0; i < 6; i++) { // main path, and end of map
        rpg_info->map[86 + (20 * i)] = 1;
    }
    
    rpg_info->map[186] = 3; // end tile
    */
}

void draw_map(info * rpg_info) { // function to draw the map & more
    generate_map(rpg_info);
    char number[8];
    for (int i = 0; i < 400; i++) {
        if ( 1 || (abs(rpg_info->map_pos - i) <= 42 && abs(rpg_info->map_pos - i) >= 38) ||
            (abs(rpg_info->map_pos - i) <= 22 && abs(rpg_info->map_pos - i) >= 18) ||
            abs(rpg_info->map_pos - i) <= 2) { // adding "fog of war"
            switch (rpg_info->map[i]) {
                case (0): { // no tile
                    break;
                }                
                case (1): { // regular tile
                    sprintf(number, "%d\n", i);
                    DrawRectangle(((window_width / 2) + (70 * (i % 20))) - 1, ((window_height / 2) + (70 * (i / 20))) - 1, 62, 62, BLACK);
                    DrawRectangle((window_width / 2) + (70 * (i % 20)), (window_height / 2) + (70 * (i / 20)), 60, 60, (Color) { 220, 180, 0, 255 });
                    DrawText(number, ((window_width / 2) + (70 * (i % 20))) - 1 + 2, ((window_height / 2) + (70 * (i / 20))) - 1 + 2, 8, BLACK);
                    break;
                }
                case (2): { // start tile
                    sprintf(number, "%d\n", i);
                    DrawRectangle(((window_width / 2) + (70 * (i % 20))) - 1, ((window_height / 2) + (70 * (i / 20))) - 1, 62, 62, BLACK);
                    DrawRectangle((window_width / 2) + (70 * (i % 20)), (window_height / 2) + (70 * (i / 20)), 60, 60, (Color) { 232, 195, 162, 255 });
                    DrawText(number, ((window_width / 2) + (70 * (i % 20))) - 1 + 2, ((window_height / 2) + (70 * (i / 20))) - 1 + 2, 8, BLACK);
                    break;
                }
                case (3): { // end tile
                    sprintf(number, "%d\n", i);
                    DrawRectangle(((window_width / 2) + (70 * (i % 20))) - 1, ((window_height / 2) + (70 * (i / 20))) - 1, 62, 62, RAYWHITE);
                    DrawRectangle((window_width / 2) + (70 * (i % 20)), (window_height / 2) + (70 * (i / 20)), 60, 60, BLACK);
                    DrawText(number, ((window_width / 2) + (70 * (i % 20))) - 1 + 2, ((window_height / 2) + (70 * (i / 20))) - 1 + 2, 8, RAYWHITE);
                    break;
                }
                case (4): {
                    sprintf(number, "%d\n", i);
                    DrawRectangle(((window_width / 2) + (70 * (i % 20))) - 1, ((window_height / 2) + (70 * (i / 20))) - 1, 62, 62, BLACK);
                    DrawRectangle((window_width / 2) + (70 * (i % 20)), (window_height / 2) + (70 * (i / 20)), 60, 60, LIGHTGRAY);
                    DrawText(number, ((window_width / 2) + (70 * (i % 20))) - 1 + 2, ((window_height / 2) + (70 * (i / 20))) - 1 + 2, 8, BLACK);
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
}

void move_mode(info * rpg_info) { // dice rolls
    char output[64];
    if (rpg_info->roll_state == 0) { // pre-roll, await enter key
        if (!IsKeyDown(KEY_ENTER)) {
            enter_flag = 0;
        }
        if (IsKeyDown(KEY_ENTER)) {
            if (!enter_flag) {
                enter_flag = 1;
                rpg_info->roll_state = 1;
            }
        }
    }
    else if (rpg_info->roll_state == 1) { // roll once, let player move
        if (!rpg_info->roll_flag) {
            sprintf(output, "moves: %d", rpg_info->moves);
            DrawRectangle(0, window_height - 35, 80, 40, BLACK);
            DrawRectangle(1, window_height - 34, 78, 38, (Color) { 232, 195, 162, 255 });
            DrawText(output, 10, window_height - 26, 16, BLACK);
        }
        else {
            rpg_info->roll_flag = 0;
            rpg_info->moves += GetRandomValue(1, rpg_info->spd);
        }
    }
    else if (rpg_info->roll_state == 2) { // do event on space
        switch (rpg_info->map[rpg_info->map_pos]) {
            case (1): { // gold (normal) tile, do nothing
                rpg_info->roll_state = 0;
                break;
            }
            case (2): { // tan (start) tile, do nothing
                rpg_info->roll_state = 0;
                break;
            }
            case (3): { // black (end) tile, finish game
                rpg_info->roll_state = 0;
                rpg_info->generated = 0;
                rpg_info->map_pos = -1;
                rpg_info->map_cam.rotation = (float) GetRandomValue(-5, 5);
                break;
            }
            case (4): { // light gray (battle) tile, start battle
                rpg_info->roll_state = 0;
                rpg_info->screen_type = 3;
            }
        }
    }
}

void move_map(info * rpg_info) { // map movement logic

    if (rpg_info->roll_state == 0) {
        return;
    }
    
    if (rpg_info->map[rpg_info->map_pos] == 3) {
        rpg_info->moves = 0;
    }
    
    if (rpg_info->moves == 0 && rpg_info->roll_state == 1) {
        DrawRectangle(0, window_height - 35, 80, 40, BLACK);
        DrawRectangle(1, window_height - 34, 78, 38, (Color) { 232, 195, 162, 255 });
        DrawText("End", 14, window_height - 26, 16, BLACK);
        if (!IsKeyDown(KEY_ENTER)) {
            enter_flag = 0;
        }
        if (IsKeyDown(KEY_ENTER)) {
            if (!enter_flag) {
                enter_flag = 1;
                rpg_info->roll_state = 2;
                rpg_info->roll_flag = 1;
                rpg_info->moves = 0;
            }
        }
        return;
    }
    
    if (!IsKeyDown(KEY_LEFT)) {
        left_flag = 0;
    } 
    else if (IsKeyDown(KEY_LEFT)) {
        if (!left_flag) {
            left_flag = 1;
            if (((rpg_info->map_pos % 20) == 0) || (rpg_info->map[rpg_info->map_pos - 1] == 0)) {
            }
            else {
                rpg_info->map_pos--;
                rpg_info->moves--;
            }
        }
    }
    
    if (!IsKeyDown(KEY_RIGHT)) {
        right_flag = 0;
    } 
    else if (IsKeyDown(KEY_RIGHT)) {
        if (!right_flag) {
            right_flag = 1;
            if (((rpg_info->map_pos % 20) == 19) || (rpg_info->map[rpg_info->map_pos + 1] == 0)) {
            }
            else {
                rpg_info->map_pos++;
                rpg_info->moves--;
            }
        }
    }
    
    if (!IsKeyDown(KEY_UP)) {
        up_flag = 0;
    } 
    else if (IsKeyDown(KEY_UP)) {
        if (!up_flag) {
            up_flag = 1;
            if ((rpg_info->map_pos < 20) || (rpg_info->map[rpg_info->map_pos - 20] == 0)) {
            }
            else {
                rpg_info->map_pos -= 20;
                rpg_info->moves--;
            }
        }
    }
    
    if (!IsKeyDown(KEY_DOWN)) {
        down_flag = 0;
    } 
    else if (IsKeyDown(KEY_DOWN)) {
        if (!down_flag) {
            down_flag = 1;
            if ((rpg_info->map_pos > 380) || (rpg_info->map[rpg_info->map_pos + 20] == 0)) {
            }
            else {
                rpg_info->map_pos += 20;
                rpg_info->moves--;
            }
        }
    }
}

void map_screen(info * rpg_info) { // map screen
    BeginMode2D(rpg_info->map_cam);
        ClearBackground(DARKGRAY);
        draw_map(rpg_info);
        draw_player(rpg_info);
        // camera zoom
        if (IsKeyDown(KEY_Z)) {
            rpg_info->map_cam.zoom -= 0.0002f;
            if (rpg_info->map_cam.zoom < 0.5f) {
                rpg_info->map_cam.zoom = 0.5f;
            }
        }
        
        if (IsKeyDown(KEY_X)) {
            rpg_info->map_cam.zoom += 0.0002f;
            if (rpg_info->map_cam.zoom > 3.0f) {
                rpg_info->map_cam.zoom = 3.0f;
            }
        }
        
        // camera movement
        if (IsKeyDown(KEY_A)) {
            rpg_info->cam_target.x -= 0.15f;
            rpg_info->map_cam.target = (Vector2) { rpg_info->cam_target.x, rpg_info->cam_target.y };
        }
        else if (IsKeyDown(KEY_D)) {
            rpg_info->cam_target.x += 0.15f;
            rpg_info->map_cam.target = (Vector2) { rpg_info->cam_target.x, rpg_info->cam_target.y };
        }
        else if (IsKeyDown(KEY_W)) {
            rpg_info->cam_target.y -= 0.15f;
            rpg_info->map_cam.target = (Vector2) { rpg_info->cam_target.x, rpg_info->cam_target.y };
        }
        else if (IsKeyDown(KEY_S)) {
            rpg_info->cam_target.y += 0.15f;
            rpg_info->map_cam.target = (Vector2) { rpg_info->cam_target.x, rpg_info->cam_target.y };
        }
    EndMode2D();
    move_mode(rpg_info);
    move_map(rpg_info);
}

void draw_battle(info * rpg_info) { // draw battle assets
    char pos[128];
    char stats[128];
    char hp[128];
    char hp_enemy[128];
    sprintf(pos, "mouse: (%d, %d)", GetMouseX(), GetMouseY());
    DrawText(pos, 20, 20, 12, BLACK);
    switch (rpg_info->class) { // draw player in bot left corner
        case (0): { // warrior
            DrawRectangle(79, (window_height) - 180 - 1, 152, 200, BLACK);
            DrawRectangle(80, (window_height) - 180, 150, 200, RED);
            break;
        }
        case (1): { // tank
            DrawRectangle(79, (window_height) - 180 - 1, 152, 200, BLACK);
            DrawRectangle(80, (window_height) - 180, 150, 200, GREEN);
            break;
        }
        case (2): { // mage
            DrawRectangle(79, (window_height) - 180 - 1, 152, 200, BLACK);
            DrawRectangle(80, (window_height) - 180, 150, 200, DARKBLUE);
            break;
        }
    }
    
    // draw enemy
    DrawRectangle(540, 60, 120, 160, RAYWHITE);
    DrawRectangle(541, 61, 118, 158, BLACK);
    
    // player actions
    DrawRectangle(((window_width - 35) / 2) - 70, window_height - 40, 70, 100, BLACK);
    DrawRectangle(((window_width - 35) / 2) - 69, window_height - 39, 68, 100, RED);
    DrawText("Attack", ((window_width - 35) / 2) - 65, window_height - 28, 18, BLACK);
    DrawRectangle((window_width - 35) / 2, window_height - 40, 70, 100, BLACK);
    DrawRectangle(((window_width - 35) / 2) + 1, window_height - 39, 68, 100, GREEN);
    DrawText("Defend", ((window_width - 35) / 2) + 5, window_height - 28, 18, BLACK);
    DrawRectangle(((window_width - 35) / 2) + 70, window_height - 40, 70, 100, BLACK);
    DrawRectangle(((window_width - 35) / 2) + 71, window_height - 39, 68, 100, DARKBLUE);
    DrawText("Special", ((window_width - 35) / 2) + 75, window_height - 28, 18, BLACK);
    
    if (rpg_info->turn && rpg_info->option_select == 0) {
        DrawRectangle(((window_width - 35) / 2) - 70, window_height - 40, 70, 100, BLACK);
        DrawRectangle(((window_width - 35) / 2) - 69, window_height - 39, 68, 100, RAYWHITE);
        DrawText("Attack", ((window_width - 35) / 2) - 65, window_height - 28, 18, BLACK);
    }
    else if (rpg_info->turn && rpg_info->option_select == 1) {
        DrawRectangle((window_width - 35) / 2, window_height - 40, 70, 100, BLACK);
        DrawRectangle(((window_width - 35) / 2) + 1, window_height - 39, 68, 100, RAYWHITE);
        DrawText("Defend", ((window_width - 35) / 2) + 5, window_height - 28, 18, BLACK);
    }
    else if (rpg_info->turn && rpg_info->option_select == 2) {
        DrawRectangle(((window_width - 35) / 2) + 70, window_height - 40, 70, 100, BLACK);
        DrawRectangle(((window_width - 35) / 2) + 71, window_height - 39, 68, 100, RAYWHITE);
        DrawText("Special", ((window_width - 35) / 2) + 75, window_height - 28, 18, BLACK);
    }
    
    // player stat hub
    DrawRectangle(579, 359, 500, 500, BLACK);
    DrawRectangle(580, 360, 500, 500, DARKGRAY);
    sprintf(stats, "Atk: %d, Def: %d, Spd: %d\n", rpg_info->atk, rpg_info->def, rpg_info->spd);
    DrawText(stats, 620, 380, 15, BLACK);
    DrawRectangle(600, 410, 180, 30, BLACK);
    DrawRectangle(601, 411, ((float) rpg_info->vit / (float) rpg_info->vit_max) * 178, 28, RED);
    sprintf(hp, "%d/%d", rpg_info->vit, rpg_info->vit_max);
    DrawText(hp, 670, 415, 18, RAYWHITE);
    
    // enemy stats
    DrawRectangle(0, 0, 220, 90, BLACK);
    DrawRectangle(0, 0, 219, 89, DARKGRAY);
    DrawRectangle(20, 30, 180, 30, BLACK);
    DrawRectangle(21, 31, ((float) rpg_info->enemy_vit / (float) rpg_info->enemy_vit_max) * 178, 28, RED);
    sprintf(hp_enemy, "%d/%d", rpg_info->enemy_vit, rpg_info->enemy_vit_max);
    DrawText(hp_enemy, 90, 35, 18, RAYWHITE);
    //DrawText(pos, 20, 20, 12, BLACK);
}

void turns(info * rpg_info) { // turn-based battle gameplay
    int enemy_action;
    if (rpg_info->turn) { // player's turn
        if (!IsKeyDown(KEY_LEFT)) {
        left_flag = 0;
        } 
        else if (IsKeyDown(KEY_LEFT)) {
            if (!left_flag) {
                left_flag = 1;
                if (rpg_info->option_select == 0) {
                    rpg_info->option_select = 2;
                }
                else {
                    rpg_info->option_select--;
                }
            }
        }
        
        if (!IsKeyDown(KEY_RIGHT)) {
        right_flag = 0;
        } 
        else if (IsKeyDown(KEY_RIGHT)) {
            if (!right_flag) {
                right_flag = 1;
                if (rpg_info->option_select == 2) {
                    rpg_info->option_select = 0;
                }
                else {
                    rpg_info->option_select++;
                }
            }
        }
        
        if (!IsKeyDown(KEY_ENTER)) {
        enter_flag = 0;
        } 
        else if (IsKeyDown(KEY_ENTER)) {
            if (!enter_flag) {
                enter_flag = 1;
                switch (rpg_info->option_select) {
                    case (0): { // attack
                        rpg_info->enemy_vit -= rpg_info->atk;
                        rpg_info->turn = 0;
                        rpg_info->defending = 0;
                        rpg_info->special_cooldown--;
                        break;
                    }
                    case (1): { // defend
                        rpg_info->defending = 1;
                        rpg_info->turn = 0;
                        rpg_info->special_cooldown--;
                        break;
                    }
                    case (2): { // special
                        if (rpg_info->special_cooldown > 0) {
                            break;
                        }
                        rpg_info->special_cooldown = 2;
                        switch (rpg_info->class) {
                            case (0): { // warrior
                                rpg_info->enemy_vit -= (4 * rpg_info->atk) + GetRandomValue(1, 5);
                                break;
                            }
                            case (1): { // tank
                                rpg_info->vit_max = (int) (1.2 * rpg_info->vit_max);
                                rpg_info->vit = (int) (1.2 * rpg_info->vit);
                                rpg_info->defending = 1;
                                break;
                            }
                            case (2): { // mage
                                rpg_info->vit += GetRandomValue((int) (rpg_info->vit_max * 0.5), (int) (rpg_info->vit_max * 0.7));
                                if (rpg_info->vit > rpg_info->vit_max) {
                                    rpg_info->vit = rpg_info->vit_max;
                                }
                                break;
                            }
                        }
                        rpg_info->turn = 0;
                        rpg_info->defending = 0;
                        break;
                    }
                }
            }
        }
    }
    else { // enemy's turn
        enemy_action = GetRandomValue(0, 2);
        switch (enemy_action) {
            case (0): { // attack
                rpg_info->turn = 1;
                if (rpg_info->defending) {
                    rpg_info->vit -= (int) (GetRandomValue(1, (int) (rpg_info->enemy_vit_max / 2)) * (((float) (10 - rpg_info->def)) / 10));
                }
                else {
                    rpg_info->vit -= GetRandomValue(1, (int) (rpg_info->enemy_vit_max / 2));
                }
                break;
            }
            case (1): { // heal
                rpg_info->enemy_vit += GetRandomValue(0, 5);
                if (rpg_info->enemy_vit > rpg_info->enemy_vit_max) {
                    rpg_info->enemy_vit = rpg_info->enemy_vit_max;
                }
                rpg_info->turn = 1;
                break;
            }
            default: {
            }
        }
    }
}

void battle_screen(info * rpg_info) { // battle screen
    if (rpg_info->enemy_vit <= 0) {
        if (rpg_info->screen_type == 3) {
            rpg_info->screen_type = 2;
        }
        rpg_info->enemy_vit_max = GetRandomValue(10, 50);
        rpg_info->enemy_vit = rpg_info->enemy_vit_max;
    }
    else if (rpg_info->vit <= 0) {
        init_info(rpg_info);
    }
    
    ClearBackground(LIGHTGRAY);
    draw_battle(rpg_info);
    turns(rpg_info);
}

int main() { // main
    InitWindow(window_width, window_height, "rpg in progress...");
       
    struct info rpg_info;
    init_info(&rpg_info);
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        switch (rpg_info.screen_type) {
            case (0): {
                title_screen(&rpg_info);
                break;
            }
            case (1): {
                class_screen(&rpg_info);
                break;
            }
            case (2): {
                map_screen(&rpg_info);
                break;
            }
            case (3): {
                battle_screen(&rpg_info);
                break;
            }
        }
        EndDrawing();
    }
    
    UnloadTexture(rpg_info.my_texture);
    CloseWindow();
    return 0;
}