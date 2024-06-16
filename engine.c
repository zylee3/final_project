#include "engine.h"

int total_cc_on_screen = 0;//number of character on screen
int textboxexist = 0;//check if textbox exist
SDL_Window* window = NULL;
SDL_Surface* gScreenSurface = NULL;



bool init(char *title){
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL){
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    gScreenSurface = SDL_GetWindowSurface(window);
    if(gScreenSurface == NULL){
        printf("Unable to get window surface! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    if(TTF_Init() == -1){
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

int _LOADITEMBOX(int type){
    SDL_Rect itemboxrect = {0, 400, 300, 300};
    if(type == 1){
        for(int i = 0; i < 2; i++){
            itemboxrect.x = 410 + i * 800;
            _LOADOBJECT("itembox", itemboxrect);
        }
    }
    else if(type == 2){
        for(int i = 0; i < 3; i++){
            itemboxrect.x = 410 + i * 400;
            _LOADOBJECT("itembox", itemboxrect);
        }
    }
}

int loadAvatar(const char *name){
    _LOADAVATARBOX();
    SDL_Rect avatarRect = {110, 800, 200, 200};
    char *avatar = calloc(strlen(name) + 20, sizeof(char));
    strcpy(avatar, name);
    for(int i = 0; i < strlen(avatar); i++){
        if(avatar[i] == ' '){
            avatar[i] = '_';
        }
    }
    if(_LOADOBJECT(avatar , avatarRect) == 1){
        printf("Failed to load image from %s! ,%d\n", avatar, __LINE__);
    }
    free(avatar);
}

int chooseFromTwo(char *name1, char *name2){
    loadTextbox();//load textbox
    _LOADITEMBOX(1);
    SDL_Rect itemrect = {420, 410, 280, 280};
    if(_LOADOBJECT(name1, itemrect) == 1){
        printf("Failed to load image from %s! ,%d\n", name1, __LINE__);
    }
    itemrect.x = 1220;
    if(_LOADOBJECT(name2, itemrect) == 1){
        printf("Failed to load image from %s! ,%d\n", name2, __LINE__);
    }
    //load choosing dialog
    char* text = calloc(strlen(name1) + strlen(name2) + 100, sizeof(char));
    strcpy(text, "Choose one of the item above! 1. ");
    strcat(text, name1);
    strcat(text, " 2. ");
    strcat(text, name2);
    loadDialog(text);
    free(text);
    while(1){
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                return 0;
            }
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){//exit
                    return 0;
                }
                else if(e.key.keysym.sym == SDLK_1){
                    return 1;
                }
                else if(e.key.keysym.sym == SDLK_2){
                    return 2;
                }
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT){
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    while(SDL_PollEvent(&e)){
                        //clean the event queue, prevent the event from being triggered multiple times
                    }
                    if(x >= 410 && x <= 690 && y >= 410 && y <= 690){
                        //printf("Choose %s\n", name1);
                        return 1;
                    }
                    else if(x >= 1220 && x <= 1500 && y >= 410 && y <= 690){
                        //printf("Choose %s\n", name2);
                        return 2;
                    }
                }
            }
        }
    }
}

int chooseFromThree(char *name1, char *name2, char *name3){
    loadTextbox();//load textbox
    _LOADITEMBOX(2);
    SDL_Rect itemrect = {420, 410, 280, 280};
    if(_LOADOBJECT(name1, itemrect) == 1){
        printf("Failed to load image from %s! ,%d\n", name1, __LINE__);
    }
    itemrect.x = 820;
    if(_LOADOBJECT(name2, itemrect) == 1){
        printf("Failed to load image from %s! ,%d\n", name2, __LINE__);
    }
    itemrect.x = 1220;
    if(_LOADOBJECT(name3, itemrect) == 1){
        printf("Failed to load image from %s! ,%d\n", name3, __LINE__);
    }
    //load choosing dialog
    char* text = calloc(strlen(name1) + strlen(name2) + strlen(name3) + 100, sizeof(char));
    strcpy(text, "Choose one of the item above! 1. ");
    strcat(text, name1);
    strcat(text, " 2. ");
    strcat(text, name2);
    strcat(text, " 3. ");
    strcat(text, name3);
    loadDialog(text);
    free(text);
    while(1){
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                return 0;
            }
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){//exit
                    return 0;
                }
                else if(e.key.keysym.sym == SDLK_1){
                    return 1;
                }
                else if(e.key.keysym.sym == SDLK_2){
                    return 2;
                }
                else if(e.key.keysym.sym == SDLK_3){
                    return 3;
                }
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT){
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    while(SDL_PollEvent(&e)){
                        //clean the event queue, prevent the event from being triggered multiple times
                    }
                    if(x >= 410 && x <= 690 && y >= 410 && y <= 690){
                        //printf("Choose %s\n", name1);
                        return 1;
                    }
                    else if(x >= 820 && x <= 1100 && y >= 410 && y <= 690){
                        //printf("Choose %s\n", name2);
                        return 2;
                    }
                    else if(x >= 1220 && x <= 1500 && y >= 410 && y <= 690){
                        //printf("Choose %s\n", name3);
                        return 3;
                    }
                }
            }
        }
    }
}

SDL_Surface* _FLIPSURFACE(SDL_Surface* surface){
    SDL_Surface* flipped = SDL_CreateRGBSurfaceWithFormat(0, surface->w, surface->h, 32, surface->format->format);

    SDL_LockSurface(surface);
    SDL_LockSurface(flipped);

    Uint32* pixels = (Uint32*)surface->pixels;
    Uint32* flippedPixels = (Uint32*)flipped->pixels;

    for(int y = 0; y < surface->h; y++) {
        for(int x = 0; x < surface->w; x++) {
            flippedPixels[y * surface->w + (surface->w - x - 1)] = pixels[y * surface->w + x];
        }
    }

    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(flipped);

    return flipped;
}

int loadCharacter(const char *name){
    char* path = calloc(strlen(name) + 20, sizeof(char));
    strcpy(path, "cc/");
    strcat(path, name);
    for(int i = 0; i < strlen(path); i++){
        if(path[i] == ' '){
            path[i] = '_';
        }
    }
    strcat(path, ".png");
    total_cc_on_screen ++;//increase the number of character on screen
    SDL_Rect ccrect;//object to hold the position of the image
    
    switch(total_cc_on_screen){
        case 1:
            ccrect.x = 0;
            ccrect.y = 400;
            ccrect.w = 200;
            ccrect.h = 400;
            break;
        case 2:
            ccrect.x = 300;
            ccrect.y = 400;
            ccrect.w = 200;
            ccrect.h = 400;
            break;
        case 3:
            ccrect.x = 600;
            ccrect.y = 400;
            ccrect.w = 200;
            ccrect.h = 400;
            break;
        case 4:
            ccrect.x = 1120;
            ccrect.y = 400;
            ccrect.w = 200;
            ccrect.h = 400;
            break;
        case 5:
            ccrect.x = 1420;
            ccrect.y = 400;
            ccrect.w = 200;
            ccrect.h = 400;
            break;
        case 6:
            ccrect.x = 1720;
            ccrect.y = 400;
            ccrect.w = 200;
            ccrect.h = 400;
            break;
        default:
            printf("Cannot load more character! %d\n", __LINE__);
            free(path);
            return 1;
    }
    if(_LOADMEDIA(path, ccrect, 1) == 1){
        printf("Failed to load image from %s! ,%d\n", path, __LINE__);
        free(path);
        return 1;
    }
    if(textboxexist == 1){
        loadTextbox();
    }
    free(path);
}

int _LOADOBJECT(char *name, SDL_Rect rect){
    char *path = calloc(strlen(name) + 20, sizeof(char));
    strcpy(path, "item/");
    strcat(path, name);
    for(int i = 0; i < strlen(path); i++){
        if(path[i] == ' '){
            path[i] = '_';
        }
    }
    strcat(path, ".png");
    if(_LOADMEDIA(path, rect, 2) == 1){
        printf("Failed to load image from %s! ,%d\n", path, __LINE__);
        free(path);
        return 1;
    }
    free(path);
}

int loadBackground(const char *name){
    char *path = calloc(strlen(name) + 20, sizeof(char));
    strcpy(path, "bg/");
    strcat(path, name);
    for(int i = 0; i < strlen(path); i++){
        if(path[i] == ' '){
            path[i] = '_';
        }
    }
    strcat(path, ".png");
    total_cc_on_screen = 0;
    textboxexist = 0;
    SDL_Rect bgrect = {0, 0, 1920, 1080};
    _TRANSITION();
    if(_LOADMEDIA(path, bgrect, 3) == 1){
        printf("Failed to load image from %s! ,%d\n", path, __LINE__);
        free(path);
        return 1;
    }
    free(path);
}

void _TRANSITION(){
    SDL_Rect transitionRect = {0, 0, 200, 1080};
    for(int i = 0 ; i < 1920 ; i += 7){
        transitionRect.x = i;
        _LOADOBJECT("Transitions", transitionRect);
    }
}

int _LOADMEDIA(char *path, SDL_Rect rect, int type){
    SDL_Surface* loadedSurface = _LOADSURFACE(path);
    if(loadedSurface == NULL){
        printf("Failed to load image from %s! ,%d\n", path, __LINE__);
        return 1;
    }
    if(type == 1 && total_cc_on_screen > 3){
        SDL_Surface* originalSurface = loadedSurface;
        loadedSurface = _FLIPSURFACE(loadedSurface);
        SDL_FreeSurface(originalSurface);
    }
    SDL_BlitScaled(loadedSurface, NULL, gScreenSurface, &rect);
    SDL_UpdateWindowSurface(window);
    SDL_FreeSurface(loadedSurface);
}

int _LOADTEXT(const char *text , int type){
    TTF_Font *font = TTF_OpenFont("ttf/lazy_dog.ttf", 32);
    if(font == NULL){
        printf("Failed to load font! SDL_ttf Error: %s , %d\n", TTF_GetError(), __LINE__);
        return 1;
    }
    SDL_Color textColor = {0, 0, 0};
    if(type == 1){//name
        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 200);
        SDL_Rect destRect = {NAME_RECTx, NAME_RECTy, 300, 50};
        SDL_BlitSurface(textSurface, NULL, gScreenSurface, &destRect);
        SDL_UpdateWindowSurface(window);
        TTF_CloseFont(font);
        SDL_FreeSurface(textSurface);
    }
    else if(type == 2){//dialog
        SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 1100);
        SDL_Rect textRect = {430, 825, 0, 0};
        SDL_BlitSurface(textSurface, NULL, gScreenSurface, &textRect);
        SDL_UpdateWindowSurface(window);
        TTF_CloseFont(font);
        SDL_FreeSurface(textSurface);
    }

}

SDL_Surface* _LOADSURFACE(char* path){

    // Create texture from surface
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL){
        printf("Unable to create Surface from %s! SDL Error: %s , %d\n", path, SDL_GetError(), __LINE__);
    }
    return loadedSurface;
}
