#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// 螢幕寬度和高度
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// 渲染器和窗口
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

// 按鈕的矩形和狀態
SDL_Rect startButtonRect = {300, 400, 200, 50};
SDL_Rect exitButtonRect = {300, 500, 200, 50};
bool isMouseOverStartButton = false;
bool isMouseOverExitButton = false;
bool isMouseDown = false;

// 載入圖像的函數
SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// 繪製文字的函數
void renderText(const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text, color);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (textTexture == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(gRenderer, textTexture, NULL, &renderQuad);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// 繪製場景
void drawScene() {
    // 清空螢幕
    SDL_SetRenderDrawColor(gRenderer, 30, 30, 30, 255); // 深灰色背景
    SDL_RenderClear(gRenderer);

    // 繪製房子
    SDL_Texture* houseTexture = loadTexture("house.png");
    if (houseTexture) {
        int houseWidth, houseHeight;
        SDL_QueryTexture(houseTexture, NULL, NULL, &houseWidth, &houseHeight);
        SDL_Rect houseRect = { (SCREEN_WIDTH - houseWidth) / 2, (SCREEN_HEIGHT - houseHeight) / 2, houseWidth, houseHeight };
        SDL_RenderCopy(gRenderer, houseTexture, NULL, &houseRect);
        SDL_DestroyTexture(houseTexture);
    }

    // 繪製按鈕
    if (isMouseOverStartButton) {
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255); // 綠色高亮
    } else {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255); // 藍色
    }
    SDL_RenderFillRect(gRenderer, &startButtonRect);

    if (isMouseOverExitButton) {
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255); // 紅色高亮
    } else {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255); // 藍色
    }
    SDL_RenderFillRect(gRenderer, &exitButtonRect);

    // 繪製文字
    SDL_Color textColor = { 255, 255, 255, 255 };
    renderText("Mickey Maze in Crazy Cruel University", 100, 50, textColor);
    renderText("Start Game", 320, 410, textColor);
    renderText("Exit", 380, 510, textColor);

    // 更新螢幕
    SDL_RenderPresent(gRenderer);
}

// 處理滑鼠事件
void handleMouseEvent(SDL_Event* event) {
    if (event->type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w &&
            y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h) {
            isMouseOverStartButton = true;
        } else {
            isMouseOverStartButton = false;
        }

        if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w &&
            y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h) {
            isMouseOverExitButton = true;
        } else {
            isMouseOverExitButton = false;
        }
    } else if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w &&
            y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h) {
            isMouseDown = true;
        }

        if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w &&
            y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h) {
            isMouseDown = true;
        }
    } else if (event->type == SDL_MOUSEBUTTONUP) {
        if (isMouseDown) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (x >= startButtonRect.x && x <= startButtonRect.x + startButtonRect.w &&
                y >= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h) {
                // 在此處添加遊戲開始的邏輯
                printf("START!\n");
                // 執行第二個程序
                system("main.exe");
            }

            if (x >= exitButtonRect.x && x <= exitButtonRect.x + exitButtonRect.w &&
                y >= exitButtonRect.y && y <= exitButtonRect.y + exitButtonRect.h) {
                // 在此處添加遊戲退出的邏輯
                printf("EXIT!\n");
                exit(0);
            }
        }
        isMouseDown = false;
    }
}

// 初始化 SDL 和 TTF
bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Mickey Maze in Crazy Cruel University", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    gFont = TTF_OpenFont("arial.ttf", 28);
    if (gFont == NULL) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    // 初始化 SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

// 清理 SDL 和 TTF
void close() {
    TTF_CloseFont(gFont);
    gFont = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
        return EXIT_FAILURE;
    } else {
        bool quit = false;
        SDL_Event e;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                handleMouseEvent(&e);
            }
            drawScene();
        }
    }

    close();
    return 0;
}

// 添加 WinMain 函數
#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif
