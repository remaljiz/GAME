/*
遊戲執行、鍵盤輸入
//gcc -o main.exe main.c initial.c collision.c move.c generate.c game.c -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

*/

#include "basic.h"
#include "initial.h"
#include "collision.h"
#include "move.h"
#include "generate.h"
#include "game.h"

//宣告所有extern的變數
struct character character = {.x = CHARACTER_X, .y = CHARACTER_Y, .offset_x = 0, .offset_y = 0};
int map[MAP_HEIGHT / ITEM_SIZE][MAP_WIDTH / ITEM_SIZE];
struct mapPoint mapPoint = {.x = MAPPOINT_X, .y = MAPPOINT_Y, .offset_x = 0, .offset_y = 0};
struct mouse mouse[TOTAL_MOUSE];
struct hp hp = {.full = TOTAL_HP, .empty = 0, .deadOrNot = false};
struct wordle letters[TOTAL_LETTERS];
int whiteMouse;
int blackMouse;
//wordle
int wordle_row, wordle_column;

//lose、win
bool lose;
bool win;

int main(int argc, char *argv[]){
    
    //初始化:創建螢幕和renderer、所有圖片
    initBasic();
    //字體
    initFont();
    //初始化wordle的圖 **測試
    initWordlePicture();
    //初始化start圖片
    initStartPicture();
    //初始化level 1圖片
    initPicture1();
    //初始化背包圖片
    initBagPicture();
    //初始化抽獎圖片
    initluckyDrawPicture();

    //start
    start();
    //level_1
    level_1();
    //整個遊戲結束釋放記憶體
    closeSDL();

}
