#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;
int SCREEN_BPP = 32;

SDL_Window* window = NULL;
SDL_Surface* gScreenSurface = NULL;

bool init();

SDL_Surface* loadSurface(char* path);

bool init(){
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Create window
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

SDL_Surface* loadSurface(char* path){

    // Create texture from surface
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL){
        printf("Unable to create Surface from %s! SDL Error: %s\n", path, SDL_GetError());
    }

    return loadedSurface;
}

int main(int argc, char* args[]){
    if(!init()){
        printf("Failed to initialize!\n");
        return 1;
    }
    SDL_Surface* bgSurface = NULL;
    SDL_Surface* ccSurface = NULL;
    //object to hold the image
    bgSurface = loadSurface("bg/a.png");
    SDL_BlitSurface(bgSurface, NULL, gScreenSurface, NULL);
    SDL_UpdateWindowSurface(window);
    

}