/*
1.物件移動後座標、數值改變與否，根據collision的bool回傳值判斷
2.物件的偏移量歸零
3.根據老鼠的隨機方向賦予老鼠對應的偏移量(下一步往哪個方向走是隨機的，offset的值根據方向不同而不同)
4.角色和老鼠的碰撞判斷(判斷是否呼叫generate.c的相關功能(碰撞特效等等))
*/

#ifndef MOVE_H
#define MOVE_H

//角色座標改變：如果1.地圖不動、2.角色移動合理 
void characterMove();

//地圖座標改變：如果1.角色移動、2.地圖移動合理 
void mapMove();

//如果老鼠移動合理 老鼠座標改變
void mousemove(struct mouse *eachMouse);

//移動後地圖、角色的偏移量歸零
void resetOffset();

//老鼠每次移動後偏移量歸零
void resetMouseOffset(void);

//隨機的偏移量
void randomMouseOffset(void);

//如果移動後的老鼠和角色座標有重疊 呼叫generate.c的碰撞事件
void characterMeetsMouse(struct mouse *eachMouse);

//角色扣血或加血
void hpCondition(hp_event event);

//根據collision.c的wordleLettersLensIsLegal判斷字母輸入是否有效 改變wordle儲存的狀態
void wordleLettersInput(int *num_row, int *num_column, int letter, int *wordle_success);

#endif