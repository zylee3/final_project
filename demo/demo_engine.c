#include "../engine.h"

int main(int argc, char* args[]){
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
}