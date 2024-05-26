/*
1.偵測物件之間碰撞的關係：角色、地圖、怪物的移動是否合理，角色和怪物碰撞後的反應
2.判斷移動是否合理，回傳bool值到move.c中物件座標改變與否的判斷的function
*/

#ifndef COLLISION_H
#define COLLISION_H

//角色移動與否(判斷下一步和地圖關係)
bool characterMoveOrNot(void);

//根據角色移動(characterMoveOrNot)，如果下一步不是空地(false)，判斷是否為電腦(或是其他互動選項) 回傳編號(ex.電腦 == 2)
int nextStepIsComputerOrNot();

//地圖移動與否(和角色移動相關)
bool mapMoveOrNot(void);

//老鼠移動與否(和地圖相關)
bool mouseMoveOrNot(struct mouse *eachMouse);

//角色和怪物的碰撞(判斷每一隻老鼠(移動後)的座標，和角色(移動後)的座標，兩者有無重疊)
bool characterMeetsMouseOrNot(struct mouse *eachmouse);

//滑鼠是否點擊app圖示
int clickAppOrNot(int x, int y);

//滑鼠是否點擊wordle option圖示
int clickWordleOptionOrNot(int x, int y);

//判斷輸入字母和次數(長度)是否合理
bool wordleLettersLensIsLegal(int num_column, int num_row, int letter);

#endif