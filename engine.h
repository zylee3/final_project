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

extern int textboxexist;
extern SDL_Window* window;

#define loadTextbox() \
    SDL_Rect textboxRect = {0, 0, 1200, 400};\
    textboxRect.x = (SCREEN_WIDTH - textboxRect.w) / 2;\
    textboxRect.y = (SCREEN_HEIGHT - textboxRect.h);\
    loadItem("textbox",textboxRect);\
    textboxexist = 1

#define loadDialog(text) \
    _LOADTEXT(text, 2)

#define loadName(name) \
    _LOADTEXT(name, 1)

bool init();

//Every function below is used to load image to screen
//Name of toml file must be the same as the name of the image, " " need to be replaced by "_"
//Example: "character 1" -> "character_1"
//Every image must use .png format

int loadCharacter(char *name);//load character to screen. name: name of the image. Character must in folder "cc"

int loadItem(char *name, SDL_Rect rect);//load item to screen. name: name of the image, rect: position of the image. Item must in folder "item"

int loadBackground(char *name);//load background to screen. name: name of the image. Background must in folder "bg"

SDL_Surface* _LOADSURFACE(char* path);//load image to surface (NOT TO USE

int _LOADMEDIA(char *path, SDL_Rect rect, int type);//BlitSurface to screen (NOT TO USE
//type 1: character
//type 2: item(object excpet character and background)
//type 3: background

int _LOADTEXT(char *text, int type);//load text to screen (NOT TO USE
//type 1: name
//type 2: dialog

SDL_Surface* _FLIPSURFACE(SDL_Surface* surface);//flip the image(NOT TO USE