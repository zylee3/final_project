#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define SCREEN_BPP 32

#define NAME_RECTx 455
#define NAME_RECTy 730
//name position

extern int total_cc_on_screen;//number of character on screen
extern int textboxexist;//check if textbox exist
extern SDL_Window* window;
extern SDL_Surface* gScreenSurface;


#define loadTextbox() \
    SDL_Rect textboxRect = {0, 0, 1200, 400};\
    textboxRect.x = (SCREEN_WIDTH - textboxRect.w) / 2;\
    textboxRect.y = (SCREEN_HEIGHT - textboxRect.h);\
    _LOADOBJECT("textbox",textboxRect);\
    textboxexist = 1

#define loadDialog(text) \
    _LOADTEXT(text, 2)

#define loadName(name) \
    _LOADTEXT(name, 1)

#define close() \
    SDL_FreeSurface(gScreenSurface);\
    SDL_DestroyWindow(window);\
    IMG_Quit();\
    TTF_Quit();\
    SDL_Quit();\
    return 0


bool init();

//Every function below is used to load image to screen
//Name of toml file must be the same as the name of the image, " " need to be replaced by "_"
//Example: "character 1" -> "character_1"
//Every image must use .png format

int _LOADITEMBOX(int type);//type 1: 2 box, type 2: 3 box(NOT TO USE

int chooseFromTwo(char *name1, char *name2);//load 2 items to screen, return the number of the item that player choose

int chooseFromThree(char *name1, char *name2, char *name3);//load 3 items to screen, return the number of the item that player choose

SDL_Surface* _FLIPSURFACE(SDL_Surface* surface);//flip the image(NOT TO USE

int loadCharacter(const char *name);//load character to screen. name: name of the image. Character must in folder "cc"

int _LOADOBJECT(char *name, SDL_Rect rect);//load item to screen. name: name of the image, rect: position of the image. Item must in folder "item"

int loadBackground(const char *name);//load background to screen. name: name of the image. Background must in folder "bg"

void _TRANSITION();//transition effect(NOT TO USE

int _LOADMEDIA(char *path, SDL_Rect rect, int type);//BlitSurface to screen (NOT TO USE
//type 1: character
//type 2: item(object excpet character and background)
//type 3: background

int _LOADTEXT(const char *text, int type);//load text to screen (NOT TO USE
//type 1: name
//type 2: dialog

SDL_Surface* _LOADSURFACE(char* path);//load image to surface (NOT TO USE