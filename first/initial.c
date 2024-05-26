/*
1.��l�ƦU�تF��Gsdl, window�Brenderer�B�y��
2.Ū�����P���a�ϡB�Ϥ�
3.�]�w�����ѹ��X�{�b�a�ϤW���H���y��, �@�w�ͦ��b�Ŧa, �X�{��m�i�H���|
*/

#include "basic.h"
#include "initial.h"
#include "collision.h"
#include "move.h"
#include "generate.h"
#include "game.h"

//�ୱ���T�ث���
struct computerDesktop computerDesktop[3];
//�ୱ���ε{��*4 + �ۤ�*1 + ���O��*1 + �����ʵe*2
struct computerApp computerApp[8];
//wordle�D�����B������?(�W�h�����ϥ�)�B�h�X�ϥܡB�W�h�����B���\�e���B���ѵe��
struct wordlePageOption wordlePageOption[6];
//wordle�r����J
struct wordle_letters_square wordle_letters_square[10][10];
//�C�@��r�������A
struct letterColor letterColor[3];
//�I�]�x�s���F��(�r���d)�A�I�]�e�q�̤j10��(�C�@���ȱ���m�T�w �Q���~�|���)
struct bagStore bagStore[9];

//��l��sdl, img, ttf, window, renderer
void initBasic(void){
    //init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    //init IMG lib
    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;; //�Ϥ��榡:PNG�BJPG
    int initImgFlag = IMG_Init(imgFlags); //��l��
    if(!(IMG_Init(imgFlags) & imgFlags)){
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        exit(1);
    }

    //init ttf
	if(TTF_Init() < 0){
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
	}

    //create window
    window = SDL_CreateWindow("map test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    //create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

//�q�����Ū�J�a�Ϩ�a�ϰ}�C
void initMap1(void){
    FILE *fMap = fopen("map1.txt", "r");
    if(fMap == NULL){
        printf("cannot find the file\n");
        exit(1);
    }
    //Ū�J�}�C
    int mapWidth, mapHeight;
    fscanf(fMap, "%d %d", &mapWidth, &mapHeight);

    for(int i = 0; i<mapWidth; i++){
        for(int j = 0; j<mapHeight; j++){
            fscanf(fMap, "%d", &map[i][j]);
        }
    }
    fclose(fMap);
}
void initMap2(void){
     FILE *fMap = fopen("map2.txt", "r");
    if(fMap == NULL){
        printf("cannot find the file\n");
        exit(1);
    }
    //Ū�J�}�C
    for(int i = 0; i<MAP_WIDTH; i++){
        for(int j = 0; j<MAP_HEIGHT; j++){
            fscanf(fMap, "%d", &map[i][j]);
        }
    }
    fclose(fMap);
}
void initMap3(void){
     FILE *fMap = fopen("map3.txt", "r");
    if(fMap == NULL){
        printf("cannot find the file\n");
        exit(1);
    }
    //Ū�J�}�C
    for(int i = 0; i<MAP_WIDTH; i++){
        for(int j = 0; j<MAP_HEIGHT; j++){
            fscanf(fMap, "%d", &map[i][j]);
        }
    }
    fclose(fMap);
}

//Ū�����P�a�ϩһݪ��Ϥ�(����B����B�a�O�B��L)
void initPicture1(void){
    //floor texture
    SDL_Surface *floorSurface = IMG_Load("picture\\floorGrey.png");
    if(floorSurface == NULL){
        printf("Unable to load floor image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    floorTexture = SDL_CreateTextureFromSurface(renderer, floorSurface);
    SDL_FreeSurface(floorSurface);
    //wall texture
    SDL_Surface *wallSurface = IMG_Load("picture\\bookshelf-brown-1.png");
    if(wallSurface == NULL){
        printf("Unable to load wall image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    wallTexture = SDL_CreateTextureFromSurface(renderer,wallSurface);
    SDL_FreeSurface(wallSurface);

    //computer texture
    SDL_Surface *computerSurface = IMG_Load("picture\\computerLoading.png");
    if(computerSurface == NULL){
        printf("Unable to load wall image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    computerTexture = SDL_CreateTextureFromSurface(renderer,computerSurface);
    SDL_FreeSurface(computerSurface);

    //door texture
    SDL_Surface *doorSurface = IMG_Load("picture\\door.png");
    if(doorSurface == NULL){
        printf("Unable to load wall image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    doorTexture = SDL_CreateTextureFromSurface(renderer,doorSurface);
    SDL_FreeSurface(doorSurface);

    //character texture
    SDL_Surface *characterSurface = IMG_Load("picture\\chiback.png");
    if(characterSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface);

    //mouse
    SDL_Surface *whiteMouseSurface = IMG_Load("picture\\whiteMouseUp.png");
    SDL_Surface *blackMouseSurface = IMG_Load("picture\\blackMouseUp.png");
    if(whiteMouseSurface == NULL || blackMouseSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    whiteMouseTexture = SDL_CreateTextureFromSurface(renderer, whiteMouseSurface);
    blackMouseTexture = SDL_CreateTextureFromSurface(renderer, blackMouseSurface);
    SDL_FreeSurface(whiteMouseSurface);
    SDL_FreeSurface(blackMouseSurface);

    //�ѹ��b�Ĥ@�����C��ƶq
    whiteMouse = 3;
    blackMouse = 2;

    //hp full and empty and break
    SDL_Surface *hp_fullSurface = IMG_Load("picture\\HP_full.png");
    SDL_Surface *hp_emptySurface = IMG_Load("picture\\HP_empty.png");
    SDL_Surface *hp_breakSurface = IMG_Load("picture\\heartBreak.png");
    if(hp_fullSurface == NULL || hp_emptySurface == NULL || hp_breakSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    hp_fullTexture = SDL_CreateTextureFromSurface(renderer, hp_fullSurface);
    hp_emptyTexture = SDL_CreateTextureFromSurface(renderer, hp_emptySurface);
    hp_breakTexture = SDL_CreateTextureFromSurface(renderer, hp_breakSurface);
    SDL_FreeSurface(hp_fullSurface);
    SDL_FreeSurface(hp_emptySurface);
    SDL_FreeSurface(hp_breakSurface);

    //crash message
    SDL_Surface *crashMessageSurface = IMG_Load("picture\\crashMessage.png");
    if(crashMessageSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    crashMessageTexture = SDL_CreateTextureFromSurface(renderer, crashMessageSurface);
    SDL_FreeSurface(crashMessageSurface);
}

void initPicture2(void){
    //floor texture
    SDL_Surface *floorSurface = IMG_Load("picture\\floorGrey.png");
    if(floorSurface == NULL){
        printf("Unable to load floor image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    floorTexture = SDL_CreateTextureFromSurface(renderer, floorSurface);
    SDL_FreeSurface(floorSurface);
    //wall texture
    SDL_Surface *wallSurface = IMG_Load("picture\\bookshelf-brown-1.png");
    if(wallSurface == NULL){
        printf("Unable to load wall image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    wallTexture = SDL_CreateTextureFromSurface(renderer,wallSurface);
    SDL_FreeSurface(wallSurface);

    //computer texture
    SDL_Surface *computerSurface = IMG_Load("picture\\computerLoading.png");
    if(computerSurface == NULL){
        printf("Unable to load wall image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    computerTexture = SDL_CreateTextureFromSurface(renderer,computerSurface);
    SDL_FreeSurface(computerSurface);

    //character texture
    SDL_Surface *characterSurface = IMG_Load("picture\\chiback.png");
    if (characterSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface);
}
void initPicture3(void){
    //floor texture
    SDL_Surface *floorSurface = IMG_Load("picture\\floorGrey.png");
    if(floorSurface == NULL){
        printf("Unable to load floor image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    floorTexture = SDL_CreateTextureFromSurface(renderer, floorSurface);
    SDL_FreeSurface(floorSurface);
    //wall texture
    SDL_Surface *wallSurface = IMG_Load("picture\\bookshelf-brown-1.png");
    if(wallSurface == NULL) {
        printf("Unable to load wall image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    wallTexture = SDL_CreateTextureFromSurface(renderer,wallSurface);
    SDL_FreeSurface(wallSurface);

    //computer texture
    SDL_Surface *computerSurface = IMG_Load("picture\\computerLoading.png");
    if(computerSurface == NULL) {
        printf("Unable to load wall image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    computerTexture = SDL_CreateTextureFromSurface(renderer,computerSurface);
    SDL_FreeSurface(computerSurface);

    //character texture
    SDL_Surface *characterSurface = IMG_Load("picture\\chiback.png");
    if(characterSurface == NULL) {
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    characterTexture = SDL_CreateTextureFromSurface(renderer, characterSurface);
    SDL_FreeSurface(characterSurface);
}

//��l�ƹC���}�l�Ϥ� **�����o�����A�᭱�A��z (�Q�n�|�@�P�Ψ쪺�Ϥ���@�_...) �Bwordle����
void initStartPicture(void){
    //��enter�i�J�B����Ƕ}���B��������
    SDL_Surface *pressToEnterSurface = IMG_Load("picture\\pressToEnter.png");
    SDL_Surface *doorCloseSurface = IMG_Load("picture\\doorClose.png");
    SDL_Surface *doorOpenSurface = IMG_Load("picture\\doorOpen.png");
    if(pressToEnterSurface == NULL || doorCloseSurface == NULL || doorOpenSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    pressToEnterTexture = SDL_CreateTextureFromSurface(renderer, pressToEnterSurface);
    doorCloseTexture = SDL_CreateTextureFromSurface(renderer, doorCloseSurface);
    doorOpenTexture = SDL_CreateTextureFromSurface(renderer, doorOpenSurface);
    //free
    SDL_FreeSurface(pressToEnterSurface);
    SDL_FreeSurface(doorCloseSurface);
    SDL_FreeSurface(doorOpenSurface);
}

//��l��computer�Mwordle�Ϥ�:3���C��B�q���ù�(3��)�B���ε{���ϥ�(4�� + 1�i�Ӥ�)�Bwordle(?�B�h�X)�B�C�@��r�����A(��B���B��)
void initWordlePicture(){
    
    //3desktop
    for(int i = 0; i<3; i++){
        SDL_Surface *mutualSurface = NULL;
        switch(i){
            case computer_desktop_empty:
                mutualSurface = IMG_Load("picture\\emptyDeskTop.png");
                computerDesktop[i].threeType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerDesktop[i].type = computer_desktop_empty;
                break;
            case computer_desktop_home:
                mutualSurface = IMG_Load("picture\\deskTop.png");
                computerDesktop[i].threeType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerDesktop[i].type = computer_desktop_home;
                break;
            case computer_desktop_load:
                mutualSurface = IMG_Load("picture\\deskTopLoading.png");
                computerDesktop[i].threeType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerDesktop[i].type = computer_desktop_load;
                break;
            default:
                break;
        }
        if(mutualSurface == NULL){
            printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_FreeSurface(mutualSurface);
    }
    //���ε{���ϥ�(4�� + 4�i�Ӥ�)
   for(int i = 0; i<8; i++){
        SDL_Surface *mutualSurface = NULL;
        switch(i){
            case computer_app_wordle:
                mutualSurface = IMG_Load("picture\\wordle.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_wordle;
                break;
            case computer_app_camera:
                mutualSurface = IMG_Load("picture\\camera.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_camera;
                break;
            case computer_app_book:
                mutualSurface = IMG_Load("picture\\deskBook.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_book;
                break;
            case computer_app_eCos:
                mutualSurface = IMG_Load("picture\\Ecourse2.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_eCos;
                break;
            case computer_app_photo:
                mutualSurface = IMG_Load("picture\\cameraPhoto1.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_photo;
                break;
            case computer_app_ecosLoading:
                mutualSurface = IMG_Load("picture\\ecosLoading.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_ecosLoading;
                break;
            case computer_app_404:
                mutualSurface = IMG_Load("picture\\404notFound.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_404;
                break;
            case computer_app_toDoList:
                mutualSurface = IMG_Load("picture\\toDoList.png");
                computerApp[i].eightType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                computerApp[i].type = computer_app_toDoList;
                break;
            default:
                break;
        }
        if(mutualSurface == NULL){
            printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_FreeSurface(mutualSurface);
    }

    //wordle(�W�h�I���ϥ�(�ݸ�)�B�h�X�ϥ�)�B���\�e���Bwordle�D����
    for(int i = 0; i<6; i++){
        SDL_Surface *mutualSurface = NULL;
        switch(i){
            case wordle_page_rule_click:
                mutualSurface = IMG_Load("picture\\wordle_rule_click.png");
                wordlePageOption[i].sixType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                wordlePageOption[i].type = wordle_page_rule_click;
                break;
            case wordle_page_quit:
                mutualSurface = IMG_Load("picture\\wordle_quit.png");
                wordlePageOption[i].sixType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                wordlePageOption[i].type = wordle_page_quit;
                break;
            case wordle_page_rule:
                mutualSurface = IMG_Load("picture\\wordleRule.png");
                wordlePageOption[i].sixType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                wordlePageOption[i].type = wordle_page_rule;
                break;
            case wordle_page_success:
                mutualSurface = IMG_Load("picture\\wordle_success.png");
                wordlePageOption[i].sixType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                wordlePageOption[i].type = wordle_page_success;
                break;
            case wordle_page_main:
                mutualSurface = IMG_Load("picture\\wordle_page.png");
                wordlePageOption[i].sixType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                wordlePageOption[i].type = wordle_page_main;
                break;
            case wordle_page_gameover:
                mutualSurface = IMG_Load("picture\\wordle_gameover.png");
                wordlePageOption[i].sixType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                wordlePageOption[i].type = wordle_page_gameover;
                break;
            default:
                break;
        }
        if(mutualSurface == NULL){
            printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_FreeSurface(mutualSurface);
    }

    //�C�@��r�����A(��B���B��)
    for(int i = 0; i<3; i++){
        SDL_Surface *mutualSurface = NULL;
        switch(i){
            case letter_color_green:
                mutualSurface = IMG_Load("picture\\letters\\color\\green.png");
                letterColor[i].threeType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                letterColor[i].type = letter_color_green;
                break;
            case letter_color_yellow:
                mutualSurface = IMG_Load("picture\\letters\\color\\yellow.png");
                letterColor[i].threeType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                letterColor[i].type = letter_color_yellow;
                break;
            case letter_color_grey:
                mutualSurface = IMG_Load("picture\\letters\\color\\grey.png");
                letterColor[i].threeType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                letterColor[i].type = letter_color_grey;
                break;
            default:
                break;
        }
        if(mutualSurface == NULL){
            printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_FreeSurface(mutualSurface);
    }

}

//�a�ϩM����y�Ъ���l��
void initCoordinate(){
    //��l�ƨ��⪺�y�ЩM�첾�q
    character.x = CHARACTER_X, character.y = CHARACTER_Y, character.offset_x = 0, character.offset_y = 0;
    //��l�Ʀa�Ϫ��y�ЩM�첾�q
    mapPoint.x = MAPPOINT_X, mapPoint.y = MAPPOINT_Y, mapPoint.offset_x = 0, mapPoint.offset_y = 0;
}

//��q����l��(full = 5, empty = 0, dead = false(0))
void initHp(){
    hp.full = TOTAL_HP, hp.empty = 0, hp.deadOrNot = false;
}

//�ѹ��������ѹ��X�{�b�a�ϤW���H���y��, �@�w�ͦ��b�Ŧa, �X�{��m�i�H���|
void initMouse(){
    //�ͦ����Ʀr�N���ѹ��b�a�ϤW���y��
    int tmp_x, tmp_y;
    int num_white = 0, num_black = 0;
    //�C���ѹ�
    for(int i = 0; i<TOTAL_MOUSE; i++){
        //�H���ͦ��Ʀr
        do{
            tmp_x = rand() % (MAP_WIDTH / ITEM_SIZE); //�l0~24 ���W�L�a�Ͻd��
            tmp_y = rand() % (MAP_HEIGHT / ITEM_SIZE);
        }while(map[tmp_y][tmp_x] != 0 || (tmp_x == CHARACTER_X && tmp_y == (CHARACTER_Y + MAP_LIMIT) / ITEM_SIZE)); //�ѹ��X�{�b���I�ΫD�Ŧa ���s�]�H���Ʀr
        //��l�ƨC���ѹ��y��
        //�H���¥�
        //0�չ�, 1�¹�
        if(rand() % 2 == 0 && num_white < whiteMouse){
            mouse[i].color = white;
            num_white++;
        }
        else{
            if(num_black < blackMouse){//�p�G�¦ѹ��ƶq�S��
                mouse[i].color = black;
                num_black++;
            }    
            else{
                mouse[i].color = white;
                num_white++;
            }
        }
        //�y��
        mouse[i].x = tmp_x;
        mouse[i].y = tmp_x;
        mouse[i].offset_x = 0;
        mouse[i].offset_y = 0;
        //printf("%d %d\n", mouse[i].x, mouse[i].y);
    }
    
}

//�C�@����wordle��l��(1. 7*7, 2. 4*4�B6*6, 3. 5*5)
void initWordleAns(){
    //�Ĥ@��7*7
    wordle_row = 7, wordle_column = 7;
    //wordle����l
    //wordle����l
    wordle_letters_square[wordle_row][wordle_column];
    //�Ĥ@�� bondage
    for(int i = 0; i<wordle_row; i++){
        for(int j = 1; j<=wordle_column; j++){
        switch(j){
            case 1:
                wordle_letters_square[i][j].target_letter = SDL_SCANCODE_B;
                wordle_letters_square[i][j].input_letter = SDL_SCANCODE_UNKNOWN;
                break;
            case 2:
                wordle_letters_square[i][j].target_letter = SDL_SCANCODE_O;
                wordle_letters_square[i][j].input_letter = SDL_SCANCODE_UNKNOWN;
                break;
            case 3:
                wordle_letters_square[i][j].target_letter = SDL_SCANCODE_N;
                wordle_letters_square[i][j].input_letter = SDL_SCANCODE_UNKNOWN;
                break;
            case 4:
                wordle_letters_square[i][j].target_letter = SDL_SCANCODE_D;
                wordle_letters_square[i][j].input_letter = SDL_SCANCODE_UNKNOWN;
                break;
            case 5:
                wordle_letters_square[i][j].target_letter = SDL_SCANCODE_A;
                wordle_letters_square[i][j].input_letter = SDL_SCANCODE_UNKNOWN;
                break;
            case 6:
                wordle_letters_square[i][j].target_letter = SDL_SCANCODE_G;
                wordle_letters_square[i][j].input_letter = SDL_SCANCODE_UNKNOWN;
                break;
            case 7:
                wordle_letters_square[i][j].target_letter = SDL_SCANCODE_E;
                wordle_letters_square[i][j].input_letter = SDL_SCANCODE_UNKNOWN;
                break;
        }
        }
    }
}

//���}�I�](���~��)���Ϥ���l��
void initBagPicture(){
    //��e���}�I�]����
    SDL_Surface *bag1Surface = IMG_Load("picture\\bag.png");
    SDL_Surface *bag2Surface = IMG_Load("picture\\bag1.png");
    SDL_Surface *bag3Surface = IMG_Load("picture\\bag2.png");
    SDL_Surface *bagInnerSurface = IMG_Load("picture\\bagInner.png");
    if(bag1Surface == NULL || bag2Surface == NULL || bag3Surface == NULL || bagInnerSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    bag1Texture = SDL_CreateTextureFromSurface(renderer, bag1Surface);
    bag2Texture = SDL_CreateTextureFromSurface(renderer, bag2Surface);
    bag3Texture = SDL_CreateTextureFromSurface(renderer, bag3Surface);
    bagInnerTexture = SDL_CreateTextureFromSurface(renderer, bagInnerSurface);
    //free
    SDL_FreeSurface(bag1Surface);
    SDL_FreeSurface(bag2Surface);
    SDL_FreeSurface(bag3Surface);
    SDL_FreeSurface(bagInnerSurface);

}

//�I��ѹ�������Ϥ�
void initluckyDrawPicture(){
    SDL_Surface *oopsSurface = IMG_Load("picture\\oops.png");
    SDL_Surface *HPrecoverSurface = IMG_Load("picture\\HPrecover.png");
    SDL_Surface *gotSomethingSurface = IMG_Load("picture\\gotSomething.png");
    SDL_Surface *attackedSurface = IMG_Load("picture\\attacked.png");
    if(oopsSurface == NULL || HPrecoverSurface == NULL || gotSomethingSurface == NULL || attackedSurface == NULL){
        printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    oopsTexture = SDL_CreateTextureFromSurface(renderer, oopsSurface);
    HPrecoverTexture = SDL_CreateTextureFromSurface(renderer, HPrecoverSurface);
    gotSomethingTexture = SDL_CreateTextureFromSurface(renderer, gotSomethingSurface);
    attackedTexture = SDL_CreateTextureFromSurface(renderer, attackedSurface);
    //free
    SDL_FreeSurface(oopsSurface);
    SDL_FreeSurface(HPrecoverSurface);
    SDL_FreeSurface(gotSomethingSurface);
    SDL_FreeSurface(attackedSurface);
}

//��l�Ʀr��
void initFont(){
    //�r��j�p
    int font_size = 24;
    //�w�]�r��
    font = TTF_OpenFont("Amestonsans-mLMj5.ttf", font_size);
    if(font == NULL){
        printf("Unable to load font! %s\n", SDL_GetError());
    }
}

//��l�ƨC�@�����I�]���ܯȱ�
void initBagHintPicture(){
    //level 1 **if level_1
    for(int i = 0; i<9; i++){
        SDL_Surface *mutualSurface = NULL;
        switch(i){
            case 0:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintA.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 1:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintB.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 2:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintD.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 3:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintE.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 4:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintG.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 5:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintN.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 6:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintO.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 7:
                mutualSurface = IMG_Load("picture\\hintLV1\\hintNothing.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            case 8: //�x�s�_��
                mutualSurface = IMG_Load("picture\\key.png");
                bagStore[i].hintType = SDL_CreateTextureFromSurface(renderer, mutualSurface);
                bagStore[i].gotHint = false;
                break;
            default:
                break;
        }
        if(mutualSurface == NULL){
            printf("Unable to load character image! SDL_Error: %s\n", SDL_GetError());
            exit(1);
        }
        SDL_FreeSurface(mutualSurface);
    }
}


