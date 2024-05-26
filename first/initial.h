/*
1.初始化各種東西：sdl, window、renderer、座標
2.讀取不同的地圖、圖片
*/

#ifndef INITIAL_H
#define INITIAL_H

//電腦桌面的型態
typedef enum computerDeskType{computer_desktop_load, computer_desktop_home, computer_desktop_empty}computerDeskType;
//電腦桌面的程式
typedef enum computerAppType{computer_app_wordle, computer_app_camera, computer_app_book, computer_app_eCos, computer_app_photo ,computer_app_ecosLoading ,computer_app_404 ,computer_app_toDoList}computerAppType;
//wordle頁面
typedef enum wordlePageType{wordle_page_rule_click, wordle_page_quit, wordle_page_rule, wordle_page_success, wordle_page_main, wordle_page_gameover}wordlePageType;
//wordle字母判斷的顏色(完整一行的各個字母)
typedef enum letterColorType{letter_color_green, letter_color_yellow, letter_color_grey}letterColorType;

//wordle 字母
extern struct wordle_letters_square{
    //這個位置題目要求的字母
    int target_letter;
    //輸入的是哪個字母
    int input_letter;
    //有沒有在正確的位置
    bool rightPosition;
    //字母是否存在
    bool letterInList;
}wordle_letters_square[10][10];


//桌面的三種型式
extern struct computerDesktop{
    computerDeskType type;
    SDL_Texture *threeType;
}computerDesktop[3];

//桌面應用程式*4 + 相片*1 + 筆記紙*1 + 網頁動畫*2
extern struct computerApp{
    computerAppType type;
    SDL_Texture *eightType;
}computerApp[8];

//wordle主頁面、頁面的?(規則說明圖示)、退出圖示、規則頁面、成功畫面、失敗畫面
extern struct wordlePageOption{
    wordlePageType type;
    SDL_Texture *sixType;
}wordlePageOption[6];

//wordle字母的狀態(綠、黃、灰)
extern struct letterColor{
    letterColorType type;
    SDL_Texture *threeType;
}letterColor[3];

//背包儲存的東西(字母卡 **暫定測試)，背包容量最大9格(每一關紙條位置固定 被抽到才會顯示)
extern struct bagStore{
    bool gotHint; //有沒有被抽到 如果是true就印出來
    SDL_Texture *hintType; //提示紙條
}bagStore[9];

//初始化sdl, window, renderer
void initBasic(void);

//***因為function重複了所以這裡應該會再改，預想是game那裏會判斷關卡，然後傳關卡數字到讀取的文件檔路徑那裏***//
//從文件檔讀入地圖到地圖陣列 
void initMap1(void);
void initMap2(void);
void initMap3(void);

//***這裡也是***///
//讀取不同地圖所需的圖片(角色、牆壁、地板、其他)
void initPicture1(void);
void initPicture2(void);
void initPicture3(void);

//初始化遊戲開始圖片 **先分這個類測試，後面再整理
void initStartPicture(void);

//初始化computer圖片 26個英文字母卡、3個顏色、電腦螢幕(3個)
void initWordlePicture();

//地圖和角色座標的初始化
void initCoordinate();

//血量的初始化(full = 5, empty = 0, dead = false(0))
void initHp();

//五隻老鼠出現在地圖上的隨機座標, 一定生成在空地, 出現位置可以重疊
void initMouse();

//每一關的wordle初始化(1. 7*7, 2. 4*4、6*6, 3. 5*5)
void initWordleAns();

//打開背包(物品欄)的圖片初始化
void initBagPicture();

//碰到老鼠抽獎的圖片
void initluckyDrawPicture();

//初始化字體
void initFont();

//初始化每一關的背包提示紙條(字母按照順序排列 不重複)
void initBagHintPicture();

#endif

