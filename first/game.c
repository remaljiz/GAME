/*
1.遊戲流程 : 遊戲開始畫面開始 -> 關卡1 -> 關卡2 -> 關卡3 -> (假結局) -> 真結局 -> 遊戲結束釋放記憶體
2.失敗(時間、沒血)、重新開始、進入下一關的條件判斷(enum)
3.整個遊戲結束記得釋放記憶體 (**預想是獨立func的圖片等等執行完可以先free)
*/

#include "basic.h"
#include "initial.h"
#include "collision.h"
#include "move.h"
#include "generate.h"
#include "game.h"
/*wordle

//根據題目的單字 判斷每次輸入字母的位置和是否正確
extern struct wordle_letters{
    //這個位置題目要求的字母
    wordle_letter target;
    //輸入的是哪個字母
    wordle_letter letter;
    //有沒有在正確的位置
    bool rightPosition;
    //字母是否存在
    bool letterInList;
    //是第幾行的輸入
    int line;
}wordle_letters;
*/
//電腦
void computer(){
    
    //電腦載入的動畫
    computerDesktopLoading();
    //滑鼠和鍵盤輸入的事件迴圈
    bool quit = false;
    //事件輸入的判斷
    bool input = false;
    //點擊wordle進入wordle遊戲的判斷
    bool wordleEvent = false;
    //點擊app的判斷(開啟、返回)
    int click_x = -1, click_y = -1;
    //進入computer()loop前 相關的static的值要初始化
    bool itemValue = false;
    while(!quit){
        //這裡也要重設點擊座標 不然進去wordle按退出會有問題 **我也不知道為什麼 印出來檢查發現滑鼠點下去的座標會一直維持上一次點的地方
        click_x = -1, click_y = -1;
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                //鍵盤輸入
                case SDL_KEYDOWN:
                    if(!input){
                        if(event.key.keysym.sym == SDLK_ESCAPE){
                    quit = true;
                    }   
                }
                break;
                //滑鼠放開(按下後)
                case SDL_MOUSEBUTTONDOWN:
                    if(!input){
                    //點擊位置座標
                    click_x = event.button.x;
                    click_y = event.button.y;
                }
                break;
                default:
                    break;
            }
            input = true;
        }
        /*if(wordleEvent)
        wordle(&wordleEvent);*/
    //如果點擊wordle 呼叫wordle
    
    //載入桌面 **測試    
        SDL_RenderClear(renderer);
        
            deskTop();
        //判斷點擊的app事件
        deskTopApp(click_x, click_y, &wordleEvent, &itemValue);
        SDL_RenderPresent(renderer);
        //如果點擊wordle事件 呼叫wordle function(game.c)

        if(wordleEvent)
        wordle(&wordleEvent);
    input = false;
    }
}

//wordle **退出wordle時wordleEvent == false **測試
void wordle(bool *wordleEvent){
    
    //初始化每一關的wordle設定 **會再多加bool or st
    initWordleAns();
    //initWordleAns();
    /*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    //wordle畫面(第一關格子7*7)

    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    */

    //滑鼠和鍵盤輸入的事件迴圈
    bool quit = false;
    //事件輸入的判斷
    bool input = false;
    //點擊右上角(規則、退出)的判斷(開啟規則、返回桌面 *wordleEvent = false)
    int click_x = -1, click_y = -1;
    //鍵盤輸入的次數(num_row: 完整行數、num_column:每一行輸入的次數)
    int num_row = 0, num_column = 0;
    //wordle成功(7次綠色 + enter = 8)
    int wordle_success = 0;
    //進入wordle()loop前 相關的static的值要初始化
    bool itemValue = false;
    while(!quit){
        click_x = -1, click_y = -1;
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                //鍵盤輸入
                case SDL_KEYDOWN:
                    //if(!input){
                        if(event.key.keysym.sym == SDLK_ESCAPE){
                            quit = true;
                    }   
                    //輸入的是英文字母、backspace、enter
                        else if(event.key.keysym.scancode >= SDL_SCANCODE_A && event.key.keysym.scancode <= SDL_SCANCODE_Z || event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE || event.key.keysym.scancode == SDL_SCANCODE_RETURN){
                            //如果wordle還沒解出來 繼續輸入
                            if(wordle_success < 8){
                                //判斷輸入、改變wordle struct儲存狀態與否
                                wordleLettersInput(&num_row, &num_column, event.key.keysym.scancode, &wordle_success);
                            }     
                    }
                //}
                break;
                //滑鼠放開(按下後)
                case SDL_MOUSEBUTTONDOWN:
                    //點擊位置座標
                    click_x = event.button.x;
                    click_y = event.button.y;
                break;
                default:
                    break;
            }
            input = true;
        }
        //載入wordle頁面 **測試
    SDL_RenderClear(renderer);
    wordlePage(num_row, num_column, wordle_success);
    //判斷點擊的wordle功能選項事件
    wordlePageOptionEvent(click_x, click_y, &quit, &itemValue);
    SDL_RenderPresent(renderer);
    input = false;
    }
    //退出wordle
    *wordleEvent = false;
}

//遊戲開始畫面
void start(){
    //設一個互動的圖片座標(press)和長寬 press圖片大小為w = 120, h = 20, 座標為(240, 290)
    struct pressCoordinate{
        int w;
        int h;
        int x;
        int y;
    }pressCoordinate = {.h = 40, .w = 240, .x = 180, .y = 280};

    
    //事件迴圈
    bool quit = false;
    //防止滑鼠點擊事件會重複觸發產生錯誤
    bool pressed = false;
    //偵測有沒有在開始圖的範圍按下滑鼠
    bool pressToEnter = false;
    while(!quit){
        //滑鼠事件
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            //滑鼠點擊
            switch(event.type){
                    //滑鼠放開(開門)
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.x >= pressCoordinate.x && event.button.x <= pressCoordinate.x + pressCoordinate.w && event.button.y >= pressCoordinate.y && event.button.y <= pressCoordinate.y + pressCoordinate.h){
                        quit = true;
                        pressToEnter = true;
                    }
                    break;
                default :
                    break;
            }
            pressed = true;
        }
        pressed = false;
        //印
        SDL_Rect doorRect = {.h = SCREEN_HEIGHT, .w = SCREEN_WIDTH, .x = 0, .y = 0};
        SDL_Rect PressRect = {.h = pressCoordinate.h, .w = pressCoordinate.w, .x = pressCoordinate.x, .y = pressCoordinate.y};
        SDL_RenderClear(renderer);
        if(!pressToEnter){
            SDL_RenderCopy(renderer ,doorCloseTexture, NULL, &doorRect);
            SDL_RenderCopy(renderer ,pressToEnterTexture, NULL, &PressRect);
        }
        else
            SDL_RenderCopy(renderer ,doorOpenTexture, NULL, &doorRect);
        
        SDL_RenderPresent(renderer); 
    }

    //進入第一關的延遲動畫
    if(pressToEnter){
            SDL_Delay(1000);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
            //進入第一關
        }
}

//關卡一
void level_1(){
    //初始化遊戲失敗、成功的判斷
    lose = false;
    win = false;
//read map
    initMap1();
    initCoordinate();
    initMouse();
    initHp();
    //初始化每一關的背包提示紙條
    initBagHintPicture();
    
//event loop
    bool quit = false;
    dir direction = character_left;
    //bool moved = false;
    //打開背包的判斷
    bool openbag = false;
    //time //unit32 == unsigned int
    Uint32 mouseTimer = 0; //計時器
    Uint32 lastTime = 0; //上一次結束的時間
    Uint32 currentTime = 0; //sdl運行到目前的時間
    while(!quit){
        SDL_Event event;
        //input
        while(SDL_PollEvent(&event)){
            //input event
            switch(event.type){
                case SDL_QUIT:
                    puts("Exited");
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    //if(!moved){
                        if(event.key.keysym.sym == SDLK_ESCAPE){
                            puts("escape");
                            quit = true;
                           // moved = true;
                        }
                        else if(event.key.keysym.scancode == SDL_SCANCODE_W){//up
                            character.offset_y = -40;
                            mapPoint.offset_y = -40;
                            //moved = true;
                        }
                        else if(event.key.keysym.scancode == SDL_SCANCODE_S){//down
                            character.offset_y = 40; 
                            mapPoint.offset_y = 40;
                           // moved = true;
                        }
                        else if(event.key.keysym.scancode == SDL_SCANCODE_A){//left
                            character.offset_x = -40; 
                            mapPoint.offset_x = -40;
                            direction = character_left;
                            //moved = true;
                        }
                        else if(event.key.keysym.scancode == SDL_SCANCODE_D){//right
                            character.offset_x = 40; 
                            mapPoint.offset_x = 40;
                            direction = character_right;
                            //moved = true;
                        }
                        else if(event.key.keysym.scancode == SDL_SCANCODE_E){//right
                            //如果沒有打開背包 打開背包
                            if(!openbag){
                                openbag = true;
                                openBagAnimation();
                            }
                            //如果已經打開背包 再按e表示關閉背包
                            else if(openbag)
                                openbag = false;
                            //moved = true;
                        }
                        //printf("%d %d\n", character.x, character.y);
                    //}
                    break; 
                default:
                    break;
            }
            //moved = true;
            SDL_Delay(5);   
        }
        //運行到目前的時間
        currentTime = SDL_GetTicks();
        //目前的時間到上一次結束的時間間隔
        Uint32 deltaTime = currentTime - lastTime;
        //更新結束時間
        lastTime = currentTime;
        //計時器增加時間
        mouseTimer += deltaTime;

        //print
        SDL_RenderClear(renderer);
        //
        generateFloor();
        generateMap();
        generateCharacter(direction);
        generateMouse();
        generateHP();
        if(openbag){//按e打開背包
            bagInnerItems();
        }
        //印到螢幕上
        SDL_RenderPresent(renderer);
        resetOffset();
        resetMouseOffset();
        
        //老鼠移動的時間間隔(300毫秒(0.3秒)走一次，毫秒 = 1/1000秒)
        if(mouseTimer >= MOUSE_MOVE_INTERVAL){
            randomMouseOffset(); //老鼠隨機往某個方向跑
            mouseTimer = 0;// 重置計時器
        }
        
        //如果遊戲失敗(血量0、時間到) **這裡要添加遊戲失敗選項(失敗畫面、詢問是否重來)  目前暫時是印game over的圖 然後reset全部 還沒有設計失敗的選項
        if(lose && win == false){
            SDL_Rect gameover_rect = {.h = 300, .w = 300, .x = 150, .y = 150};
            SDL_RenderCopy(renderer, wordlePageOption[wordle_page_gameover].sixType, NULL, &gameover_rect);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
            initCoordinate();
            initMouse();
            initHp();
            //初始化每一關的背包提示紙條
            initBagHintPicture();
            lose = false;
        }
        //如果遊戲勝利(通過wordle拿到鑰匙 進入鎖起來的門，前進下一關)  **測試 目前還沒做下一關  
        if(win && lose == false){
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            SDL_Delay(1000);
            quit = true;
        }
        
            
    }
}


//遊戲結束釋放記憶體

void closeSDL(){
    SDL_DestroyTexture(characterTexture);
    SDL_DestroyTexture(wallTexture);
    SDL_DestroyTexture(computerTexture);
    SDL_DestroyTexture(floorTexture);
    SDL_DestroyTexture(whiteMouseTexture);
    SDL_DestroyTexture(blackMouseTexture);
    SDL_DestroyTexture(crashMessageTexture);
    SDL_DestroyTexture(hp_fullTexture);
    SDL_DestroyTexture(hp_emptyTexture);
    SDL_DestroyTexture(hp_breakTexture);
    SDL_DestroyTexture(doorTexture);
    SDL_DestroyTexture(doorCloseTexture);
    SDL_DestroyTexture(doorOpenTexture);
    SDL_DestroyTexture(pressToEnterTexture);
    SDL_DestroyTexture(bag1Texture);
    SDL_DestroyTexture(bag2Texture);
    SDL_DestroyTexture(bag3Texture);
    SDL_DestroyTexture(bagInnerTexture);
    SDL_DestroyTexture(oopsTexture);
    SDL_DestroyTexture(HPrecoverTexture);
    SDL_DestroyTexture(gotSomethingTexture);
    SDL_DestroyTexture(attackedTexture);
    //letter
    for(int i = 0; i<TOTAL_LETTERS; i++)
        SDL_DestroyTexture(letters[i].wordle_letterTexture);
    //3desktop
    for(int i = 0; i<3; i++)
        SDL_DestroyTexture(computerDesktop[i].threeType);
    //應用程式圖示(4個 + 4張照片)
    for(int i = 0; i<8; i++)
        SDL_DestroyTexture(computerApp[i].eightType);
    //wordle(規則點擊圖示(問號)、退出圖示)、成功畫面、wordle主頁面
    for(int i = 0; i<6; i++)
        SDL_DestroyTexture(wordlePageOption[i].sixType);
    //每一行字母狀態(綠、黃、灰)
    for(int i = 0; i<3; i++)
        SDL_DestroyTexture(letterColor[i].threeType);
    //背包
    for(int i = 0; i<9; i++)
        SDL_DestroyTexture(bagStore[i].hintType);
        
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}
