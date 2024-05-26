/*
1.物件移動後座標、數值改變與否，根據collision的bool回傳值判斷
2.物件的偏移量歸零
3.根據老鼠的隨機方向賦予老鼠對應的偏移量(下一步往哪個方向走是隨機的，offset的值根據方向不同而不同)
4.角色和老鼠的碰撞判斷(判斷是否呼叫generate.c的相關功能(碰撞特效等等))
*/

#include "basic.h"
#include "initial.h"
#include "collision.h"
#include "move.h"
#include "generate.h"
#include "game.h"

//判斷角色碰到老鼠的事件 事件觸發後過一段冷卻(無敵)時間才會對下一次事件有反應
static bool collisionHappened = false;
//碰撞事件計時器
Uint32 collisionTimer = 0;
//1秒冷卻
#define COLLISION_COOLDOWN 1000

//如果地圖不動 角色移動合理 座標改變 //如果不是空地 判斷是不是互動方塊(電腦、其他) 呼叫對應function
void characterMove(void){
    if(characterMoveOrNot() && mapMoveOrNot() == false){
        character.x += character.offset_x;
        character.y += character.offset_y;
    }
    //如果不是空地 判斷是不是互動方塊(電腦、其他)
    else if(!characterMoveOrNot())
        switch(nextStepIsComputerOrNot()){
            //不是互動物件
            case -1:
                break;
            //電腦(2)
            case 2:
                //執行computer的function(game.c)
                computer();
                break;
            //上鎖的門
            case 3:
                //如果背包中有鑰匙(index:8), 代表通關)
                if(bagStore[8].gotHint)
                    win = true;
                break;
            default:
                break;
        }
}

//如果角色移動 地圖移動合理 地圖座標改變
void mapMove(void){
    if(mapMoveOrNot()){
        mapPoint.x += mapPoint.offset_x;
        mapPoint.y += mapPoint.offset_y;
    }
}

//如果老鼠移動合理 老鼠座標改變
void mousemove(struct mouse *eachMouse){
        //如果當前老鼠移動合理 座標改變
        if(mouseMoveOrNot(eachMouse)){
            eachMouse -> x += eachMouse -> offset_x;
            eachMouse -> y += eachMouse -> offset_y;
        }
    
}

//移動後偏移量歸零
void resetOffset(){
    character.offset_x = 0;
    character.offset_y = 0;
    mapPoint.offset_x = 0;
    mapPoint.offset_y = 0;
}

//老鼠每次移動偏移量歸零
void resetMouseOffset(void){
    for(int i = 0; i<TOTAL_MOUSE; i++){
        //全部歸零
        mouse[i].offset_y = 0;
        mouse[i].offset_x = 0;
    }
}

//老鼠隨機的偏移量
void randomMouseOffset(void){
    //隨機生成數字 0 ~ 3 代表上左下右
    for(int i = 0; i<TOTAL_MOUSE; i++){
        mouse[i].mouse_dir = rand() % 4;
        switch(mouse[i].mouse_dir){
        case mouse_up: //up
            mouse[i].offset_y = -1;
            break;
        case mouse_left: //left
            mouse[i].offset_x = -1;
            break;
        case mouse_down: //down
            mouse[i].offset_y = 1;
            break;
        case mouse_right: //right
            mouse[i].offset_x = 1;
            break;
        default:
            break;
        }
    } 
}

//如果移動後的老鼠和角色座標有重疊 呼叫generate.c的碰撞事件
void characterMeetsMouse(struct mouse *eachMouse){
    if(characterMeetsMouseOrNot(eachMouse) && !collisionHappened){//如果還沒碰撞(或者是過了冷卻時間)
        //SDL_Delay(1000);
        //呼叫generate的碰撞特效(圖片, 暫停個一秒?)
        //crashMessage();
        //如果是黑老鼠 扣血 (代表不同血量的值改變)
        if(eachMouse -> color == black)
            hpCondition(hp_reduction);
        //如果是白老鼠 抽獎 (generate.c的抽獎)
        else
            luckyDraw();
        //碰撞發生
        collisionHappened = true;
        //開始碰撞冷卻時間計時
        collisionTimer = SDL_GetTicks();
    }
    //如果過了冷卻時間 重製碰撞事件判斷
    if(collisionHappened && SDL_GetTicks() - collisionTimer >= COLLISION_COOLDOWN)
        collisionHappened = false;
}

//角色扣血或加血
void hpCondition(hp_event event){
    if(!collisionHappened){
    switch(event){
        //回血
        case hp_recovery:
            if(hp.full < 5){
                hp.full += 1;
                hp.empty -= 1;
            }
            break;
        //扣血
        case hp_reduction:
            printf("attacked!\n");
            hp.full -= 1;
            hp.empty += 1;
           //圖
            SDL_Rect rect = {.h = 300, .w = 300, .x = 150, .y = 150};
            SDL_RenderCopy(renderer, attackedTexture, NULL, &rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(800);
            //如果沒血了 更新死亡狀態
            if(hp.full == 0)
                hp.deadOrNot = true;
            break;
        default:
            break;
    }
    }
}

//根據collision.c的wordleLettersLensIsLegal判斷字母輸入是否有效 改變wordle儲存的狀態 還有判斷wordle成功的次數
void wordleLettersInput(int *num_row, int *num_column, int letter, int *wordle_success){
    if(wordleLettersLensIsLegal(*num_column, *num_row, letter)){
        //字母
        if(letter >= SDL_SCANCODE_A && letter <= SDL_SCANCODE_Z){
            //重置wordle_success(如果*num_column == 0)
            if(*num_column == 0)
                *wordle_success = 0;

            *num_column += 1;
            wordle_letters_square[*num_row][*num_column].input_letter = letter;
            //如果輸入字母正確 位置正確 在單字中(綠色)
            if(letter == wordle_letters_square[*num_row][*num_column].target_letter){
                wordle_letters_square[*num_row][*num_column].rightPosition = true;
                wordle_letters_square[*num_row][*num_column].letterInList = true;
                //wordle成功次數+1
                *wordle_success += 1;
                return;
            }
            //如果字母不正確 尋找其他位置判斷字母是否在單字中
            else{
                wordle_letters_square[*num_row][*num_column].rightPosition = false;
                for(int i = 1; i<=wordle_column; i++){
                    if(wordle_letters_square[*num_row][i].target_letter == letter){
                        //字母出現在單字其他位置 代表在單字中 但是位置不正確(黃色)
                        wordle_letters_square[*num_row][*num_column].letterInList = true;
                        return;
                    }
                }
                //不在正確位置 也沒有出現在單字其他位置(灰色)
                wordle_letters_square[*num_row][*num_column].letterInList = false;
                    return;
            }
        }
            
        //backspace 往前刪除當前字母狀態
        if(letter == SDL_SCANCODE_BACKSPACE){
            wordle_letters_square[*num_row][*num_column].input_letter = SDL_SCANCODE_UNKNOWN;
            wordle_letters_square[*num_row][*num_column].rightPosition = false;
            wordle_letters_square[*num_row][*num_column].letterInList = false;
            *num_column -= 1;
            return;
        }
            
        //enter 如果滿了 跳下一行
        if(letter == SDL_SCANCODE_RETURN){
            *num_column = 0;
            *num_row += 1;
            //wordle_success++
            *wordle_success += 1;
            return;
        }

    }
    
    
}




