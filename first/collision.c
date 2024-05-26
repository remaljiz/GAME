/*
1.偵測物件之間碰撞的關係：角色、地圖、怪物的移動是否合理，角色和怪物碰撞後的反應
2.判斷移動是否合理，回傳bool值到move.c中(物件座標改變與否的判斷)
*/

#include "basic.h"
#include "initial.h"
#include "collision.h"
#include "move.h"
#include "generate.h"
#include "game.h"


//判斷角色的下一步是否能走
bool characterMoveOrNot(void){
    int next_x = character.x + character.offset_x;
    int next_y = character.y + character.offset_y;
    //如果角色的下一步在地圖上能夠行走(下一格是0, 空地)
    if(map[(next_y + mapPoint.y) / ITEM_SIZE][(next_x + mapPoint.x) / ITEM_SIZE] == 0){
        //如果下一步沒有超過整張地圖的範圍
        if(next_y + mapPoint.y < MAP_HEIGHT && next_x + mapPoint.x < MAP_WIDTH)
            return true;
    }
    return false;
}

//根據角色移動(characterMoveOrNot)，如果下一步不是空地(false)，判斷是否為電腦(或是其他互動選項)  回傳編號(ex.電腦 == 2) **測試
int nextStepIsComputerOrNot(){
    int next_x = character.x + character.offset_x;
    int next_y = character.y + character.offset_y;
    //2 computer
    if(map[(next_y + mapPoint.y) / ITEM_SIZE][(next_x + mapPoint.x) / ITEM_SIZE] == 2)
        return 2;
    //3 locked door
    else if(map[(next_y + mapPoint.y) / ITEM_SIZE][(next_x + mapPoint.x) / ITEM_SIZE] == 3)
        return 3;
    //不是互動物件
    return -1;
    
}

//根據角色移動，判斷地圖是否移動
bool mapMoveOrNot(void){
    //如果角色移動
    if(characterMoveOrNot()){
        //角色的下一步(x, y)
        int next_x = character.x + character.offset_x;
        int next_y = character.y + character.offset_y;
        //如果角色下一步在螢幕的兩邊中線上，或在螢幕的邊界上 **在原點(0, 560)的邊界移動，要讓地圖不動，(character.x == 0 && character.y + mapPoint.y != MAP_LIMIT + ITEM_LIMIT) || (character.y == ITEM_LIMIT && character.y + mapPoint.y != MAP_LIMIT + ITEM_LIMIT)
        if(next_x + ITEM_SIZE/2 == SCREEN_WIDTH /2 || next_y + ITEM_SIZE/2 == SCREEN_HEIGHT/2 || next_x == ITEM_LIMIT || (next_x == 0 && next_y + mapPoint.y != MAP_LIMIT + ITEM_LIMIT) || (next_y == ITEM_LIMIT && next_y + mapPoint.y != MAP_LIMIT + ITEM_LIMIT) || next_y == 0){
            //如果地圖座標+位移量在地圖座標範圍內
            if(mapPoint.x + mapPoint.offset_x <= MAP_LIMIT && mapPoint.x + mapPoint.offset_x >= 0 && mapPoint.y + mapPoint.offset_y <= MAP_LIMIT && mapPoint.y + mapPoint.offset_y >= 0){
                return true;
            }
        }      
    }
    return false;
}

//判斷老鼠的下一步是否能走
bool mouseMoveOrNot(struct mouse *eachMouse){
    int next_x, next_y;
    next_x = eachMouse -> x + eachMouse -> offset_x;
    next_y = eachMouse -> y + eachMouse -> offset_y;
    //判斷下一步是不是空地
    if(map[next_y][next_x] == 0)
         //如果老鼠座標+位移量在地圖座標範圍內
        if(next_x < MAP_WIDTH / ITEM_SIZE && next_y < MAP_HEIGHT / ITEM_SIZE )
            return true;
    
    return false;
}

//角色和怪物的碰撞(判斷每一隻老鼠(移動後)的座標，和角色(移動後)的座標，兩者有無重疊)
bool characterMeetsMouseOrNot(struct mouse *eachmouse){
    //因為老鼠的座標是地圖儲存圖片的座標 要 * ITEM_SIZE 再 - mapPoint才是螢幕上的座標
    int mouse_x = eachmouse -> x * ITEM_SIZE - mapPoint.x;
    int mouse_y = eachmouse -> y * ITEM_SIZE - mapPoint.y;
    //如果老鼠在螢幕上的座標等於角色的座標 有碰撞
    if(character.x == mouse_x && character.y  == mouse_y)
        return true;

    return false;
}

//滑鼠是否點擊app圖示
int clickAppOrNot(int x, int y){
    
    for(int i = 0; i<4; i++){
        //app的座標大小
        SDL_Rect appRect = {.h = 60, .w = 60, .x = 100 + i*80, .y = 100};
        //如果點擊圖示範圍
        if(x >= appRect.x && x <= appRect.x + appRect.w && y >= appRect.y && y <= appRect.y + appRect.h){
            //判斷是哪個app
            return computerApp[i].type;
        }
    }
    //如果都沒有 回傳-1
    return -1;
}

//滑鼠是否點擊wordle option圖示
int clickWordleOptionOrNot(int x, int y){
    for(int i = 0; i<2; i++){
        //wordle option的座標大小
        SDL_Rect optionRect = {.h = 50, .w = 50, .x = 420 + i*80, .y = 30};
        //如果點擊圖示範圍
        if(x >= optionRect.x && x <= optionRect.x + optionRect.w && y >= optionRect.y && y <= optionRect.y + optionRect.h){
            //判斷是哪個功能選項
            return wordlePageOption[i].type;
        }
    }
    //如果都沒有 回傳-1
    return -1;
}

//判斷輸入字母和次數(長度)是否合理
bool wordleLettersLensIsLegal(int num_column, int num_row, int letter){
    //1. 7*7
    //字母
    if(letter >= SDL_SCANCODE_A && letter <= SDL_SCANCODE_Z){
        if(num_column + 1 < 7 && num_row < 7) return true;
        if(num_column + 1 == 7 && num_row < 7) return true;
        return false;
    }
    //backspace 往前刪除一個字母
    if(letter == SDL_SCANCODE_BACKSPACE){
        if(num_column - 1 >= 0) return true;
        return false;
    }
    //enter(輸入完整單字後 按下enter)
    if(letter == SDL_SCANCODE_RETURN){
        if(num_column == 7 && num_row < 7) return true;
        return false;
    }
}





