/*
1.遊戲流程 : 遊戲開始畫面開始 -> 關卡1 -> 關卡2 -> 關卡3 -> (假結局) -> 真結局 -> 遊戲結束釋放記憶體
2.失敗(時間、沒血)、重新開始、進入下一關的條件判斷(enum)
3.整個遊戲結束記得釋放記憶體
*/

#ifndef GAME_H
#define GAME_H

//電腦
void computer();

//wordle **退出wordle時wordleEvent == false
void wordle(bool *wordleEvent);

//遊戲開始畫面
void start();

//關卡1
void level_1();

//關卡2
void level_1();

//關卡3
void level_1();

//真結局
void true_end();

//假結局
void fake_end();

//整個遊戲結束釋放記憶體
void closeSDL();

#endif