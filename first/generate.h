/*
1.把物件圖片印到螢幕上，順序為地板 >> 牆壁 >> 角色 >>　老鼠　>> 其他物件
2.後面印上去的圖片會覆蓋原有的圖的部分畫面 *畫面被覆蓋的位置和面積是要印上去的圖片設定的座標和大小
3.角色和老鼠碰撞特效(暫定)
4.白老鼠的抽獎(回血、提示、槓龜)
*/

#ifndef GENERATE_H
#define GENERATE_H

//地板
void generateFloor(void);

//地圖物件
void generateMap(void);

//角色
void generateCharacter(dir direction);

//老鼠
void generateMouse(void);

//角色、老鼠的碰撞特效
void crashMessage();

//血量(hp) 地圖右上角(左到右)
void generateHP();

//白老鼠的抽獎(回血、提示、槓龜) **提示還沒做、槓龜的圖還沒做
void luckyDraw();

//電腦載入的動畫
void computerDesktopLoading();

//桌面和觸控選項(應用程式)
void deskTop();

//桌面app
void deskTopApp(int x, int y, bool *wordleEvent, bool *itemValue);

//wordle頁面
void wordlePage(int num_row, int num_column, int wordle_success);

//wordle選項2(rule、quit)的事件(1.點擊查看規則、2.退出wordle回到桌面)
void wordlePageOptionEvent(int x, int y, bool *wordleQuit, bool *itemValue);

//主地圖按e打開背包的動畫
void openBagAnimation();

//打開背包
void bagInnerItems();

#endif