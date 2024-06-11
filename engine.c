#include "engine.h"

bool init(){
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    window = SDL_CreateWindow("Furry adventure!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

int chooseFromTwo(char *name1, char *name2){
    loadTextbox();//load textbox
    _LOADITEMBOX(1);
    SDL_Rect itemrect = {420, 410, 280, 280};
    if(_LOADOBJECT(name1, itemrect) == 1){
        printf("Failed to load image from %s!\n", name1);
    }
    itemrect.x = 1220;
    if(_LOADOBJECT(name2, itemrect) == 1){
        printf("Failed to load image from %s!\n", name2);
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
        printf("Failed to load image from %s!\n", name1);
    }
    itemrect.x = 820;
    if(_LOADOBJECT(name2, itemrect) == 1){
        printf("Failed to load image from %s!\n", name2);
    }
    itemrect.x = 1220;
    if(_LOADOBJECT(name3, itemrect) == 1){
        printf("Failed to load image from %s!\n", name3);
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

int loadCharacter(char *name){
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
            ccrect.w = 400;
            ccrect.h = 400;
            break;
        case 2:
            ccrect.x = 400;
            ccrect.y = 400;
            ccrect.w = 400;
            ccrect.h = 400;
            break;
        case 3:
            ccrect.x = 1120;
            ccrect.y = 400;
            ccrect.w = 400;
            ccrect.h = 400;
            break;
        case 4:
            ccrect.x = 1520;
            ccrect.y = 400;
            ccrect.w = 400;
            ccrect.h = 400;
            break;
        default:
            printf("Cannot load more character!\n");
            free(path);
            return 1;
    }
    if(_LOADMEDIA(path, ccrect, 1) == 1){
        printf("Failed to load image from %s!\n", path);
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
        printf("Failed to load image from %s!\n", path);
        free(path);
        return 1;
    }
    free(path);
}

int loadBackground(char *name){
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
    if(_LOADMEDIA(path, bgrect, 3) == 1){
        printf("Failed to load image from %s!\n", path);
        free(path);
        return 1;
    }
    free(path);
}

int _LOADMEDIA(char *path, SDL_Rect rect, int type){
    SDL_Surface* loadedSurface = _LOADSURFACE(path);
    if(loadedSurface == NULL){
        printf("Failed to load image from %s!\n", path);
        return 1;
    }
    if(type == 1 && total_cc_on_screen > 2){
            loadedSurface = _FLIPSURFACE(loadedSurface);
    }
    SDL_BlitScaled(loadedSurface, NULL, gScreenSurface, &rect);
    SDL_UpdateWindowSurface(window);
    SDL_FreeSurface(loadedSurface);
}

int _LOADTEXT(char *text , int type){
    TTF_Font *font = TTF_OpenFont("ttf/lazy_dog.ttf", 32);
    if(font == NULL){
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
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
        printf("Unable to create Surface from %s! SDL Error: %s\n", path, SDL_GetError());
    }
    return loadedSurface;
}

/*int main(int argc, char* args[]){
    if(!init()){
        printf("Failed to initialize!\n");
        return 1;
    }
    SDL_Surface* bgSurface = NULL;
    //object to hold the image

    int start = 0;
    //start screen
    loadBackground("bgstart");
    SDL_Rect buttonRect = {0, 650, 350, 150};
    buttonRect.x = buttonRect.x = (SCREEN_WIDTH - buttonRect.w) / 2;
    _LOADOBJECT("startbutton", buttonRect); 
    SDL_Rect titleRect = {0, 75, 650, 450};
    titleRect.x = (SCREEN_WIDTH - titleRect.w) / 2;
    _LOADOBJECT("title", titleRect);

    
    while(1){
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                SDL_FreeSurface(bgSurface);
                SDL_DestroyWindow(window);
                IMG_Quit();
                TTF_Quit();
                SDL_Quit();
                return 0;
            }
            else if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){//exit
                    SDL_FreeSurface(bgSurface);
                    SDL_DestroyWindow(window);
                    IMG_Quit();
                    TTF_Quit();
                    SDL_Quit();
                    return 0;
                }
                else if(e.key.keysym.sym == SDLK_c){//sample character
                    loadCharacter("blackcat");
                }
                else if(e.key.keysym.sym == SDLK_t){//sample textbox showing
                    loadName("Black Cat");
                    loadDialog("Hi, I'm Black Cat, is this guild's knight captain. Nice to meet you! freshman! I'm here to help you with your first mission. Let's go to the basement to get your first mission!");
                }
                else if(e.key.keysym.sym == SDLK_i){
                    chooseFromTwo("catnip", "frisbee");
                }
                else if(e.key.keysym.sym == SDLK_b){
                    loadBackground("basement");
                }
                else if(e.key.keysym.sym == SDLK_x){
                    chooseFromThree("catnip", "frisbee", "goldring");
                }
                
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){//sample button click
                int x, y;
                SDL_GetMouseState(&x, &y);
                while(SDL_PollEvent(&e)){
                    //clean the event queue, prevent the event from being triggered multiple times
                }
                if(x >= 790 && x <= 1140 && y >= 640 && y <= 790 && start == 0){
                    start = 1;
                    loadBackground("basement");
                    loadTextbox();
                    break;
                }
            }
        }
    }
    SDL_FreeSurface(bgSurface);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;

}*/