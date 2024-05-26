/*
1.把物件圖片印到螢幕上，順序為地板 >> 牆壁 >> 角色 >>　老鼠　>> 其他物件 
2.後面印上去的圖片會覆蓋原有的圖的部分畫面 *畫面被覆蓋的位置和面積是要印上去的圖片設定的座標和大小(rect)
3.角色和老鼠碰撞特效(暫定)
4.白老鼠的抽獎(回血、提示、槓龜)
*/

#include "basic.h"
#include "initial.h"
#include "collision.h"
#include "move.h"
#include "generate.h"
#include "game.h"

//碰到白老鼠的抽獎
typedef enum luckyDrawType{draw_recovery, draw_hint, draw_so_sad}luckyDrawType;
//地板
void generateFloor(void){
    SDL_Rect floorRect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH};
    SDL_RenderCopy(renderer, floorTexture, NULL, &floorRect);
}

//地圖物件
void generateMap(){
    //判斷地圖座標有沒有改變
    mapMove();
    
    //根據地圖的物件編號印出地圖 *地圖陣列的x, y要和sdl畫面的x, y相反
    for(int i = mapPoint.x / 40; i<(mapPoint.x + SCREEN_WIDTH) / 40; i++){
        for(int j = mapPoint.y / 40; j<(mapPoint.y + SCREEN_HEIGHT) / 40; j++){
            if(map[j][i] == 1){
                SDL_Rect wallRect = {.h = ITEM_SIZE, .w = ITEM_SIZE, .x = i * ITEM_SIZE - mapPoint.x, .y = j * ITEM_SIZE- mapPoint.y};
                SDL_RenderCopy(renderer, wallTexture, NULL, &wallRect);
            }
            else if(map[j][i] == 2){
                SDL_Rect computerRect = {.h = ITEM_SIZE, .w = ITEM_SIZE, .x = i * ITEM_SIZE - mapPoint.x, .y = j * ITEM_SIZE - mapPoint.y};
                SDL_RenderCopy(renderer, computerTexture, NULL, &computerRect);
            }
            else if(map[j][i] == 3){ //上鎖的門
                SDL_Rect doorRect = {.h = ITEM_SIZE, .w = ITEM_SIZE, .x = i * ITEM_SIZE - mapPoint.x, .y = j * ITEM_SIZE - mapPoint.y};
                SDL_RenderCopy(renderer, doorTexture, NULL, &doorRect);
            }
        }
    }
}

//角色
void generateCharacter(dir direction){
    characterMove();
    SDL_Rect characterRect = {.h = ITEM_SIZE, .w = ITEM_SIZE, .x = character.x, .y = character.y};
    //判斷輸入的方向鍵，決定角色面向哪裡
    switch(direction)
    {
    case character_up:
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);
        break;
    case character_left:
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);
        break;
    case character_down:
        SDL_RenderCopy(renderer, characterTexture, NULL, &characterRect);
        break;
    case character_right:
        SDL_RenderCopyEx(renderer, characterTexture, NULL, &characterRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        break;
    default:
        break;
    }
}

//老鼠
void generateMouse(void){
    for(int i = 0; i<TOTAL_MOUSE; i++){
        mousemove(&mouse[i]);
        //老鼠座標
        SDL_Rect mouseRect = {.h = ITEM_SIZE, .w = ITEM_SIZE, .x = mouse[i].x * ITEM_SIZE - mapPoint.x, .y = mouse[i].y * ITEM_SIZE- mapPoint.y};
        //1.顏色2.方向
        switch(mouse[i].color){
            case white:
                //方向
                switch(mouse[i].mouse_dir){
                    case mouse_up://up
                        SDL_RenderCopy(renderer, whiteMouseTexture, NULL, &mouseRect);
                        break;
                    case mouse_left://left
                        SDL_RenderCopyEx(renderer, whiteMouseTexture, NULL, &mouseRect, -90, NULL, SDL_FLIP_NONE);
                        break;
                    case mouse_down:
                        SDL_RenderCopyEx(renderer, whiteMouseTexture, NULL, &mouseRect, 0, NULL, SDL_FLIP_VERTICAL);
                        break;
                    case mouse_right:
                        SDL_RenderCopyEx(renderer, whiteMouseTexture, NULL, &mouseRect, 90, NULL, SDL_FLIP_NONE);
                        break;
                    default:
                        break;
                }
                //SDL_RenderCopy(renderer, whiteMouseTexture, NULL, &mouseRect);
                break;
            case black:
                switch(mouse[i].mouse_dir){
                    case mouse_up://up
                        SDL_RenderCopy(renderer, blackMouseTexture, NULL, &mouseRect);
                        break;
                    case mouse_left://left
                        SDL_RenderCopyEx(renderer, blackMouseTexture, NULL, &mouseRect, -90, NULL, SDL_FLIP_NONE);
                        break;
                    case mouse_down:
                        SDL_RenderCopyEx(renderer, blackMouseTexture, NULL, &mouseRect, 0, NULL, SDL_FLIP_VERTICAL);
                        break;
                    case mouse_right:
                        SDL_RenderCopyEx(renderer, blackMouseTexture, NULL, &mouseRect, 90, NULL, SDL_FLIP_NONE);
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        characterMeetsMouse(&mouse[i]);
    }
}

//角色、老鼠的碰撞特效
void crashMessage(){
    SDL_Rect rect = {.h = 80, .w = 200, .x = character.x, .y = character.y};
    SDL_RenderCopy(renderer, crashMessageTexture, NULL, &rect);
}

//血量(hp) 地圖右上角(左到右)
void generateHP(){
    int HP_x = 400, HP_y = 0; //y軸不變
    //empty
    for(int i = 0; i<hp.empty; i++){
        SDL_Rect rect = {.h = 40, .w = 40, .x = HP_x, .y = HP_y};
        SDL_RenderCopy(renderer, hp_emptyTexture, NULL, &rect);
        HP_x += ITEM_SIZE;
    }
     //full
    for(int i = 0; i<hp.full; i++){
        SDL_Rect rect = {.h = 40, .w = 40, .x = HP_x, .y = HP_y};
        SDL_RenderCopy(renderer, hp_fullTexture, NULL, &rect);
        HP_x += ITEM_SIZE;
    }
    //如果血量為0(死亡) lose設為true
    if(hp.deadOrNot == true){
        lose = true;
    }
}

//白老鼠的抽獎(回血、提示、槓龜) **提示還沒做
void luckyDraw(){
    //第幾關的判斷(根據每一關的wordle答案 給相關提示)
    //隨機抽獎
    luckyDrawType luckyOrNot = rand() % 3;
    switch(luckyOrNot){
        case draw_recovery:{
            hpCondition(hp_recovery);
            SDL_Rect rect = {.h = 300, .w = 300, .x = 150, .y = 150};
            SDL_RenderCopy(renderer, HPrecoverTexture, NULL, &rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(800);
            printf("recover!\n");
            }
            break;
        case draw_hint:{
            //隨機提示 **背包
            //第一關背包提示紙條有8個
            int hint = rand() % 8; // bagStore[0] ~ [7]
            //如果還沒被抽過 標記為true 在背包中印出來
            if(bagStore[hint].gotHint == false)
                bagStore[hint].gotHint = true;
            //圖
            SDL_Rect rect = {.h = 300, .w = 300, .x = 150, .y = 150};
            SDL_RenderCopy(renderer, gotSomethingTexture, NULL, &rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(800);
            printf("got a letter!\n");
            }
            break;
        case draw_so_sad:{
            //圖
            SDL_Rect rect = {.h = 300, .w = 300, .x = 150, .y = 150};
            SDL_RenderCopy(renderer, oopsTexture, NULL, &rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(800);
            printf("oops...qq\n");
            }
            break;
        default:
            break;
    }
}

//電腦載入的動畫
void computerDesktopLoading(){
    //電腦loading畫面

    //時間條的最常長度
    int timeLine = 380;
    //時間條的面積和座標
    SDL_Rect timeRect = {.h = 20, .w = 2, .x = 100, .y = 400};
    SDL_Rect desktopRect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
    bool quit = false;
    while(!quit){
        //loading畫面
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, computerDesktop[computer_desktop_load].threeType, NULL, &desktopRect);
        //設定白色
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //時間條填充為白色
        SDL_RenderFillRect(renderer, &timeRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
        //時間條在寬度限制內延長
        if(timeRect.w < timeLine)
            timeRect.w ++;
        else
            quit = true;
    }
    SDL_Delay(1000);
}

//桌面和觸控選項(應用程式)
void deskTop(){
    //桌面
    SDL_Rect deskRect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
    SDL_RenderCopy(renderer, computerDesktop[computer_desktop_home].threeType, NULL, &deskRect);
    //應用程式*4
    for(int i = 0; i<4; i++){
        SDL_Rect appRect = {.h = 60, .w = 60, .x = 100 + i*80, .y = 100};
        SDL_RenderCopy(renderer, computerApp[i].eightType, NULL, &appRect);       
    }  
}

//桌面app
void deskTopApp(int x, int y, bool *wordleEvent, bool *itemValue){
    //滑鼠是否點擊app圖示 呼叫collision.c判斷
    
    //ecos圈圈動畫的判斷
    bool static load = false;
    //延續點擊的app項目
    static int appItem = -1;

    if(*itemValue == false){
        appItem = -1;
        *itemValue = true;
    }
    //
    if(appItem == computer_app_wordle && *wordleEvent == false)
        appItem = -1;
    //如果click_x && click_y == -1 代表沒有點擊
    if(x != -1 && y != -1)
        appItem = clickAppOrNot(x, y);

    switch(appItem){
        case -1://沒有點任何圖示
            //恢復一寇死圈圈動畫
            load = false;
            break;
        case computer_app_wordle:{//wordle遊戲()
            *wordleEvent = true;
            break;
        }
            
        case computer_app_camera:{
            //相片
            SDL_Rect rect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
            SDL_RenderCopy(renderer, computerApp[computer_app_photo].eightType, NULL, &rect);
            break;
            }
            
        case computer_app_book:{
            //to do list
            SDL_Rect rect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
            SDL_RenderCopy(renderer, computerApp[computer_app_toDoList].eightType, NULL, &rect);  
            break;
        }
            
        case computer_app_eCos:{
            //ecos loading
            if(!load){
            SDL_Rect rect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
            SDL_RenderCopy(renderer, computerApp[computer_app_ecosLoading].eightType, NULL, &rect);
            SDL_RenderPresent(renderer);
            //延遲一秒
            SDL_Delay(1000);
            load = true;
            }
            //404
            SDL_Rect rect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
            SDL_RenderCopy(renderer, computerApp[computer_app_404].eightType, NULL, &rect);
            break;
        }
        default:
            break;
    }
}

//wordle頁面
void wordlePage(int num_row, int num_column, int wordle_success){
    //黑色
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    //wordle頁面
    SDL_Rect rect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
    SDL_RenderCopy(renderer, wordlePageOption[wordle_page_main].sixType, NULL, &rect);

    //選項2(rule、quit)
    for(int i = 0; i<2; i++){
        SDL_Rect optionRect = {.h = 50, .w = 50, .x = 420 + i*80, .y = 30};
        SDL_RenderCopy(renderer, wordlePageOption[i].sixType, NULL, &optionRect);       
    }  
    //wordle空格(關卡一 7*7)

    //印出已存在的完整行數的狀況(綠色:位置正確、字母在單字中，黃色:位置錯誤、字母在單字中，灰色:都不在)
    for(int i = 0; i<num_row; i++){
        for(int j = 1; j<=wordle_column; j++){
            //綠色
            if(wordle_letters_square[i][j].rightPosition && wordle_letters_square[i][j].letterInList){
                SDL_Rect rect = {.h = 60, .w = 60, .x = 55 + 70*(j - 1), .y = 110 + 70*(i)};
                SDL_RenderCopy(renderer, letterColor[letter_color_green].threeType, NULL, &rect);
            }
            //黃色
            else if(wordle_letters_square[i][j].rightPosition == false && wordle_letters_square[i][j].letterInList){
                SDL_Rect rect = {.h = 60, .w = 60, .x = 55 + 70*(j - 1), .y = 110 + 70*(i)};
                SDL_RenderCopy(renderer, letterColor[letter_color_yellow].threeType, NULL, &rect);
            }
            //灰色
            else{
                SDL_Rect rect = {.h = 60, .w = 60, .x = 55 + 70*(j - 1), .y = 110 + 70*(i)};
                SDL_RenderCopy(renderer, letterColor[letter_color_grey].threeType, NULL, &rect);
            }

        }
    }
   //wordle的空白格子 第一關7*7 還有每次輸入的字母
    for(int i = 0; i<wordle_row; i++){
        for(int j = 1; j<=wordle_column; j++){
            //格子
            SDL_Rect rect = {.h = 60, .w = 60, .x = 55 + 70*(j - 1), .y = 110 + 70*(i)};
            SDL_RenderDrawRect(renderer, &rect);
            //字母
            
            //字體顏色黑色
            SDL_Color textColor = {0, 0, 0, 255};
            //判斷單字(A == 65 scancode_a == 4 所以用61去加scancode)
            char input_letter;
            //如果這一格的字母存在
            if(wordle_letters_square[i][j].input_letter != SDL_SCANCODE_UNKNOWN){
                input_letter = 'A' + wordle_letters_square[i][j].input_letter - 4;
                SDL_Surface *textSurface = TTF_RenderText_Solid(font, &input_letter, textColor);
                if(textSurface == NULL){
                    printf("unable to load textSurface, %s\n", SDL_GetError());
                    exit(1);
                }
            //texture
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if(textTexture == NULL){
                    printf("unable to load textTexture, %s\n", SDL_GetError());
                    exit(1);
                }
            SDL_FreeSurface(textSurface);
            //字母位置
            SDL_Rect textRect = {.h = 60, .w = 60, .x = 55 + 70*(j - 1), .y = 110 + 70*(i)};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }     
    }
}
    //wordle_success(8次 印出成功畫面)
    if(wordle_success == 8){
        SDL_Rect success_rect = {.h = 300, .w = 300, .x = 150, .y = 150};
        SDL_RenderCopy(renderer, wordlePageOption[wordle_page_success].sixType, NULL, &success_rect);
        //背包中的鑰匙(index:8)設為true
        bagStore[8].gotHint = true;
    }
    //wordle game over(如果num_row == 7、wordle_success != 8)
    if(num_row == 7 && wordle_success != 8){
        SDL_Rect gameover_rect = {.h = 300, .w = 300, .x = 150, .y = 150};
        SDL_RenderCopy(renderer, wordlePageOption[wordle_page_gameover].sixType, NULL, &gameover_rect);
    }
    
}

//wordle選項2(rule、quit)的事件(1.點擊查看規則、2.退出wordle回到桌面)
void wordlePageOptionEvent(int x, int y, bool *wordleQuit, bool *itemValue){
    //滑鼠是否點擊wordle功能 呼叫collision.c判斷
    
    //延續點擊的項目
    static int optionItem = -1;

    if(*itemValue == false){
        optionItem = -1;
        *itemValue = true;
    }
    //如果click_x && click_y == -1 代表沒有點擊
    if(x != -1 && y != -1)
        optionItem = clickWordleOptionOrNot(x, y);
   
    switch(optionItem){
        case -1://沒有點任何圖示
            break;
        case wordle_page_rule_click:{//點擊規則(?)
            //規則畫面
            SDL_Rect rect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
            SDL_RenderCopy(renderer, wordlePageOption[wordle_page_rule].sixType, NULL, &rect);
            break;
        }
        case wordle_page_quit:{//點擊退出
            //退出wordle遊戲 回到桌面
            *wordleQuit = true;
            break;
            }
        default:
            break;
    }
}

//主地圖按e打開背包的動畫
void openBagAnimation(){
    SDL_Rect rect = {.h = 300, .w = 300, .x = 150, .y = 150};
    SDL_RenderCopy(renderer, bag1Texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(800);
    SDL_RenderCopy(renderer, bag2Texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(800);
    SDL_RenderCopy(renderer, bag3Texture, NULL, &rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(800);
    
}

//打開背包
void bagInnerItems(){
    //背包內襯
    SDL_Rect rect = {.h = 400, .w = 400, .x = 100, .y = 100};
    SDL_RenderCopy(renderer, bagInnerTexture, NULL, &rect);
    //背包內的紙條、鑰匙
    for(int i = 0; i<9; i++){
        switch(i){
            case 0:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 100, .y = 170};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 1:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 240, .y = 170};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 2:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 380, .y = 170};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 3:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 100, .y = 270};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 4:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 240, .y = 270};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 5:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 380, .y = 270};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 6:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 100, .y = 360};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 7:
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 240, .y = 360};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            case 8: //儲存鑰匙
                if(bagStore[i].gotHint){
                    SDL_Rect rect = {.h = 60, .w = 120, .x = 360, .y = 360};
                    SDL_RenderCopy(renderer, bagStore[i].hintType, NULL, &rect);
                }
                break;
            default:
                break;
        }
    }

}




