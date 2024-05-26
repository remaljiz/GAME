/*
1.所有物件的值：包括地圖、螢幕、角色、牆壁......，定義起始座標、每張圖片的大小、物件移動的位移量
、地圖座標和位移物件在螢幕上的限制座標範圍

2.所有物件會用到的sdl指標：SDL_window(創建視窗)、SDL_Renderer(將圖片複製到renderer, renderer再顯示到螢幕上)
、SDL_Texture(所有圖片的texture, 將texture複製到renderer)

3.sdl的座標系統原點是(0, 0), x軸從左到右遞增，y軸從上到下遞增。*每張圖片的座標是圖片的左上角
*/

#ifndef BASIC_H
#define BASIC_H

#include <stdio.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>

//地圖總長寬
#define MAP_WIDTH 1000
#define MAP_HEIGHT 1000

//螢幕長寬
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

//物件大小(長、寬都是40) *任何物件的位移量 = 40
#define ITEM_SIZE 40

//地圖座標在螢幕上的最大座標限制(x、y都是400)
#define MAP_LIMIT 400

//移動物件座標在螢幕上的最大座標限制(x、y都是560)
#define ITEM_LIMIT 560

//角色在螢幕上的初始位置座標(螢幕左下角) = (0, 560)
#define CHARACTER_X 0
#define CHARACTER_Y ITEM_LIMIT

//地圖在螢幕上的初始座標(切齊螢幕左下角) = (0, 400)
#define MAPPOINT_X 0
#define MAPPOINT_Y MAP_LIMIT

//老鼠的總數(暫定 後面可以改成依據關卡難度增加 老鼠數量也會增加)
#define TOTAL_MOUSE 5
//每隔300毫秒老鼠移動一次
#define MOUSE_MOVE_INTERVAL 300

//總hp(血量)5顆心
#define TOTAL_HP 5

//字母數量
#define TOTAL_LETTERS 26

//黑白老鼠的數量(根據關卡不同可以修改數量)
extern int whiteMouse;
extern int blackMouse;

//wordle格子數
extern int wordle_row, wordle_column;

//每一關的關卡失敗判斷 **在每一關的開頭都要重置
extern bool lose;
//每一關的關卡成功判斷(通過wordle)**在每一關的開頭都要重置
extern bool win;

//sdl 相關的指標
//創建視窗使用的指標)
SDL_Window *window;

//將圖片顯示到螢幕上所需要的指標
SDL_Renderer *renderer;

//所有圖片的texture所需的指標
SDL_Texture *doorTexture;
SDL_Texture *doorOpenTexture;
SDL_Texture *doorCloseTexture;
SDL_Texture *pressToEnterTexture;
SDL_Texture *characterTexture;
SDL_Texture *wallTexture;
SDL_Texture *computerTexture;
SDL_Texture *floorTexture;
SDL_Texture *whiteMouseTexture;
SDL_Texture *blackMouseTexture;
SDL_Texture *crashMessageTexture;
SDL_Texture *hp_fullTexture;
SDL_Texture *hp_emptyTexture;
SDL_Texture *hp_breakTexture;
SDL_Texture *bag1Texture;
SDL_Texture *bag2Texture;
SDL_Texture *bag3Texture;
SDL_Texture *bagInnerTexture;
SDL_Texture *oopsTexture;
SDL_Texture *HPrecoverTexture;
SDL_Texture *gotSomethingTexture;
SDL_Texture *attackedTexture;
//字體
TTF_Font *font;

//地圖的陣列，儲存地圖面積除以物件面積的值，也就是地圖上面能放多少物件
extern int map[MAP_HEIGHT / ITEM_SIZE][MAP_WIDTH / ITEM_SIZE];

//輸入方向鍵的方向
typedef enum dir{character_up, character_left, character_down, character_right}dir;
//老鼠隨機跑的方向
typedef enum mouse_dir{mouse_up, mouse_left, mouse_down, mouse_right}mouse_dir;
//老鼠顏色
typedef enum mouse_color{white, black}mouse_color;
//血量增加或減少的判斷
typedef enum hp_event{hp_recovery, hp_reduction}hp_event;
//wordle的字母判A~Z
typedef enum wordle_letter{letter_A, letter_B, letter_C, letter_D, letter_E, letter_F, letter_G, letter_H, letter_I, letter_J, letter_K, letter_L, letter_M, letter_N, letter_O, letter_P, letter_Q, letter_R, letter_S, letter_T, letter_U, letter_V, letter_W, letter_X, letter_Y, letter_Z}wordle_letter;

//角色的座標和位移量(後續的移動會調用)
extern struct character{
    int x;
    int y;
    int offset_x;
    int offset_y;
}character;

//地圖左上角的座標和位移量(後續的移動會調用)
extern struct mapPoint{
    int x;
    int y;
    int offset_x;
    int offset_y;
}mapPoint;

//老鼠的座標和位移量(隨機出現在地圖的空白上) //五隻老鼠(黑白隨機)
extern struct mouse{
    mouse_dir mouse_dir;
    mouse_color color;
    int x;
    int y;
    int offset_x;
    int offset_y;
}mouse[TOTAL_MOUSE];

//hp的狀態(0<= full、empty的數量 <= 5)
extern struct hp{
    int full;
    int empty;
    //判斷有無死亡(血量歸零)
    bool deadOrNot;
}hp;

//wordle圖片從A~Z
extern struct wordle{
    //這個位置題目要求的字母
    wordle_letter target;
    SDL_Texture *wordle_letterTexture;
}letters[TOTAL_LETTERS];


#endif