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
    Camera2D map_cam;
    Rectangle cam_target;
    Texture2D my_texture;
} info;

void generate_map(info * rpg_info) { // test map generation function, maybe delete later
    // dummy map
    // clear array
    for (int i = 0; i < 400; i++) {
        rpg_info->map[i] = 0;
    }
    
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
}

void draw_map(info * rpg_info) { // function to draw the map & more
    generate_map(rpg_info);
    char number[8];
    for (int i = 0; i < 400; i++) {
        if ((abs(rpg_info->map_pos - i) <= 42 && abs(rpg_info->map_pos - i) >= 38) ||
            (abs(rpg_info->map_pos - i) <= 22 && abs(rpg_info->map_pos - i) >= 18) ||
            abs(rpg_info->map_pos - i) <= 2) {
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
                default: {
                    break;
                }
            }
        }
    }
}

void draw_player(info * rpg_info) {
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
        rpg_info->map_pos = 0;
    }
    
    DrawRectangle(((window_width / 2) + (70 * (rpg_info->map_pos % 20))) + 20, ((window_height / 2) + (70 * (rpg_info->map_pos / 20))) +20, 20, 20, player_color);
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

void move_mode(info * rpg_info) {
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
            rpg_info->moves += GetRandomValue(50, 100);
        }
    }
}

void move_map(info * rpg_info) {
    if (rpg_info->moves == 0) {
        DrawRectangle(0, window_height - 35, 80, 40, BLACK);
        DrawRectangle(1, window_height - 34, 78, 38, (Color) { 232, 195, 162, 255 });
        DrawText("End", 14, window_height - 26, 16, BLACK);
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

void map_screen(info * rpg_info) {
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

int main() {
    InitWindow(window_width, window_height, "rpg in progress...");
    
    Image image = LoadImage("images/Bird.jpg");
    
    struct info rpg_info;
    rpg_info.screen_type = 0;
    rpg_info.class = -1; 
    rpg_info.map_pos = -1;
    rpg_info.roll_state = 0;
    rpg_info.roll_flag = 1;
    rpg_info.moves = 0;
    rpg_info.my_texture = LoadTextureFromImage(image);
    UnloadImage(image);
    
    // initialize map camera
    rpg_info.cam_target = (Rectangle) { 200, 200, 0, 0 };
    rpg_info.map_cam.target = (Vector2) { rpg_info.cam_target.x, rpg_info.cam_target.y }; 
    rpg_info.map_cam.offset = (Vector2) { (window_width / 2.0f), (window_height / 2.0f) };
    rpg_info.map_cam.rotation = (float) GetRandomValue(-5, 5);
    rpg_info.map_cam.zoom = 1.0f;
        
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
        }
        EndDrawing();
    }
    
    UnloadTexture(rpg_info.my_texture);
    CloseWindow();
    return 0;
}