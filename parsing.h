#include <stdio.h>
#include <stdint.h>

typedef struct _sID
{
	uint8_t key;
	char name[64];
} ID;

typedef struct _sScene
{
	ID* pID;
	char background[64];
} Scene;

typedef struct _sCharacter
{
	ID* pID;
	char avatar[64];
	char tachie[64];
} Character;

typedef struct _sItem
{
	ID* pID;
	char image[64];
} Item;

typedef struct _sFavor
{
	ID* pID;
	uint32_t favor_cats;
	uint32_t favor_dogs;
} Favor;

typedef struct _sDialogueStory
{
	ID* pID;
	char text[1024];
	char next[64];
	char event[64];
} DialogueStory;

typedef struct _sDialogueConversation
{
	ID* pID;
	char character[64];
	char text[1024];
	char next[64];
	char event[64];
} DialogueConversation;