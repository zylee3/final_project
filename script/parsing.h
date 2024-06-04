#include <stdio.h>
#include <stdint.h>

typedef struct _sScene
{
	char name[32];
	char background[64];
}Scene;

typedef struct _sCharacter
{
	char name[32];
	char avatar[64];
	char tachie[64];
	uint32_t favor_cats;
	uint32_t favor_dogs;
}Character;

typedef struct _sItem
{
	char name[32];
	char image[64];
}
