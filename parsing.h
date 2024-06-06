#include <stdio.h>
#include <stdint.h>

typedef struct _sScene
{
	char name[64];
	char background[64];
} Scene;

typedef struct _sCharacter
{
	char name[64];
	char avatar[64];
	char tachie[64];
} Character;

typedef struct _sItem
{
	char name[64];
	char image[64];
} Item;

typedef struct _sFavor
{
	char name[64];
	uint32_t favor_cats;
	uint32_t favor_dogs;
} Favor;

typedef struct _sDialogueStory
{
	char name[64];
	char text[1024];
	char next[64];
	char event[64];
} DialogueStory;

typedef struct _sDialogueConversation
{
	char name[64];
	char character[64];
	char text[1024];
	char next[64];
	char event[64];
} DialogueStory;