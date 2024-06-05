#include <stdio.h>
#include <stdint.h>

typedef struct _sScene
{
	char name[32];
	char background[64];
} Scene;

typedef struct _sCharacter
{
	char name[32];
	char avatar[64];
	char tachie[64];
} Character;

typedef struct _sItem
{
	char name[32];
	char image[64];
} Item;

typedef struct _sFavor
{
	char name[32];
	uint32_t favor_cats;
	uint32_t favor_dogs;
} Favor;
