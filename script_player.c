#include <assert.h>
#include "parsing.h"
#include "engine.h"

#define PARSING_RECORD	1
#define PARSING_ENDED	2
#define MAX_CHARACTERS	8

typedef enum _eTraverseOption
{
	TRAVERSE_NONE,
	TRAVERSE_BREAK,
	TRAVERSE_NEXT_PARSING_RECORD,
	TRAVERSE_JUMP,
} TraverseOption;

int32_t g_characters = 0;
char g_loadedCharacters[MAX_CHARACTERS][128];

const char* find_loaded_character(const char* character)
{
	for (int32_t i = 0; i < g_characters; ++i)
	{
		if (strcmp(character, g_loadedCharacters[i]) == 0) { return g_loadedCharacters[i]; }
	}

	return NULL;
}

void insert_loaded_character(const char* character)
{
	strcpy(g_loadedCharacters[g_characters++], character);
}

parsing_record handle_parsing_record(SDL_UserEvent* pUserEvent)
{
	assert(pUserEvent->code == PARSING_RECORD);

	//printf("handle_parsing_record 1\n");

	parsing_handle h = pUserEvent->data1;
	parsing_record pvCurr = pUserEvent->data2;
	TraverseOption option = TRAVERSE_NONE;

	for (; pvCurr != NULL;
		pvCurr = (option == TRAVERSE_NEXT_PARSING_RECORD) ? script_get_next(h, pvCurr) :
		((option == TRAVERSE_JUMP) ? pvCurr : NULL))
	{
		//printf("pvCurr:%p option:%d\n", pvCurr, option);
		// all structures should be start from IdParentChild
		IdParentChild* pIdParentChild = (IdParentChild*)pvCurr;

		switch (pIdParentChild->_id)
		{
		case TABLE:
			{
				// nothing happen, try next record
				// Table* pTable = (Table*)pvCurr;
				// next parsing record
				option = TRAVERSE_NEXT_PARSING_RECORD;
			}
			break;
		case ARRAY:
			{
				// nothing happen, try next record
				// Array* pArray = (Array*)pvCurr;
				// next parsing record
				option = TRAVERSE_NEXT_PARSING_RECORD;
			}
			break;
		case FINAL_PROJECT:
			{
				// nothing happen, try next record
				// FinalProject* pFinalProject = (FinalProject*)pvCurr;
				// next parsing record
				option = TRAVERSE_NEXT_PARSING_RECORD;
			}
			break;
		case SCENE:
			{
				// nothing happen, try next record
				// Scene* pScene = (Scene*)pvCurr;
				option = TRAVERSE_NEXT_PARSING_RECORD;
			}
			break;
		case CHARACTER:
			{
				// nothing happen, try next record
				// Character* pCharacter = (Character*)pvCurr;
				// next parsing record
				option = TRAVERSE_NEXT_PARSING_RECORD;
			}
			break;
		case ITEM:
			{
				// nothing happen, try next record
				// Item* pItem = (Item*)pvCurr;
				// next parsing record
				option = TRAVERSE_NEXT_PARSING_RECORD;
			}
			break;
		case EVENT:
			{
				Event* pEvent = (Event*)pvCurr;
				if ((pEvent->scene != NULL) && (pEvent->scene[0] != '\0'))
				{
					//printf("Event scene:%s\n", pEvent->scene);
					FullNameRecord* pFullNameRec = script_get_summary_next(h, NULL, pEvent->scene);
					if (pFullNameRec == NULL)
					{
						printf("Invalid Event.scene %s in handle_parsing_record\n", pEvent->scene);
						return NULL;
					}
					const char* background = ((Scene*)pFullNameRec->record)->background;
					//printf("Event scene background:%s\n", background);
					loadBackground(background);
					// clear all characters by loadBackground
					g_characters = 0;
				}
				if ((pEvent->dialogue != NULL) && (pEvent->dialogue[0] != '\0'))
				{
					//printf("Event dialogue:%s\n", pEvent->dialogue);
					FullNameRecord* pFullNameRec = script_get_summary_next(h, NULL, pEvent->dialogue);
					if (pFullNameRec == NULL)
					{
						printf("Invalid Event.dialogue %s in handle_parsing_record\n", pEvent->dialogue);
						return NULL;
					}
					// next parsing record
					option = TRAVERSE_JUMP;
					pvCurr = pFullNameRec->record;
					//printf("Event dialog ended jump %p\n", pvCurr);
				}
			}
			break;
		case DIALOGUE:
			{
				Dialogue* pDialogue = (Dialogue*)pvCurr;
				if ((pDialogue->character != NULL) && (pDialogue->character[0] != '\0'))
				{
					//printf("Dialogue character:%s\n", pDialogue->character);
					FullNameRecord* pFullNameRec = script_get_summary_next(h, NULL, pDialogue->character);
					if (pFullNameRec == NULL)
					{
						printf("Invalid Dialog.character %s in handle_parsing_record\n", pDialogue->character);
						return NULL;
					}
					const char* name      = ((Character*)pFullNameRec->record)->name;
					const char* avatar    = ((Character*)pFullNameRec->record)->avatar;
					const char* character = ((Character*)pFullNameRec->record)->tachie;
					if (find_loaded_character(name) == NULL)
					{
						loadCharacter(character);
						loadAvatar(avatar);
						insert_loaded_character(name);

					}
					loadTextbox();
					loadName(name);
				}
				else
				{
					loadTextbox();
				}
				if ((pDialogue->text != NULL) && (pDialogue->text[0] != '\0'))
				{
					//printf("Dialogue text:%s\n", pDialogue->text);
					loadDialog(pDialogue->text);
				}
				if ((pDialogue->next != NULL) && (pDialogue->next[0] != '\0'))
				{
					//printf("Dialogue next:%s\n", pDialogue->next);
					FullNameRecord* pFullNameRec = script_get_summary_next(h, NULL, pDialogue->next);
					if (pFullNameRec == NULL)
					{
						printf("Invalid Dialogue.next %s in handle_parsing_record\n", pDialogue->next);
						return NULL;
					}
					//printf("pFullNameRec:%p for next\n", pFullNameRec);
					return pFullNameRec->record; // direct jump after mouse click
				}
				else
				{
					return NULL;
				}
			}
			break;
		default:
			printf("unsupported id %d in handle_parsing_record\n", pIdParentChild->_id);
			option = TRAVERSE_BREAK;
			break;
		}

		//printf("handle_parsing_record 2\n");

		if (option == TRAVERSE_BREAK) { break; }
	}

	if (pvCurr == NULL) { return pvCurr; }

	//printf("pvCurr:%p option:%d 2\n", pvCurr, option);
	return script_get_next(h, pvCurr);
}

int main(int argc, char* args[])
{
	if (argc < 2)
	{
		printf("This program need a toml file argument!\n"
			"for ex.:\n"
			"    ./script_player script/script.toml\n");
		return 1;
	}

	parsing_handle h = script_init();
	if (h == NULL)
	{
		printf("script_init failed\n");
		return 2;
	}

	int32_t ret = script_parsing(h, args[1]);
	if (ret != 0)
	{
		ret = 3;
		printf("script_parsing failed\n");
		goto script_uninit_label;
	}

	parsing_record pvCurr = script_get_next(h, NULL);
	if (pvCurr == NULL)
	{
		ret = 4;
		printf("empty script!\n");
		goto script_uninit_label;
	}

	const char* finalProjectName = ((FinalProject*)pvCurr)->name;
	//printf("finalProjectName:%s\n", finalProjectName);

	if (!init(finalProjectName))
	{
		ret = 5;
		printf("Failed to initialize the engine!\n");
		goto script_uninit_label;
	}

	SDL_Surface* bgSurface = NULL;
	//object to hold the image

	bool start = false;

	//start screen
	loadBackground("bgstart");
	SDL_Rect buttonRect = {0, 650, 350, 150};
	buttonRect.x = buttonRect.x = (SCREEN_WIDTH - buttonRect.w) / 2;
	_LOADOBJECT("startbutton", buttonRect);
	SDL_Rect titleRect = {0, 75, 650, 450};
	titleRect.x = (SCREEN_WIDTH - titleRect.w) / 2;
	_LOADOBJECT("title", titleRect);

	uint32_t scriptEvent = SDL_RegisterEvents(1);
	if (scriptEvent == (uint32_t)-1) { goto engine_uninit_label; }

	SDL_UserEvent userEvent = { .type = scriptEvent, .code = PARSING_RECORD, .data1 = h, .data2 = pvCurr };

	while (1)
	{
		SDL_Event e;

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT) { goto engine_uninit_label; }

			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE) { goto engine_uninit_label; }
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				//printf("Click on %d %d\n", x, y);
				SDL_GetMouseState(&x, &y);
				//clean the event queue, prevent the event from being triggered multiple times
				while (SDL_PollEvent(&e)) {}

				if (!start)
				{
					if ((x >= 790 && x <= 1140) && (y >= 640 && y <= 790))
					{
						start = true;
						SDL_PushEvent((SDL_Event*)&userEvent);
						break;
					}
				}
				else
				{
					// mouse click for next record in toml script
					if (pvCurr != NULL)
					{
						userEvent = (SDL_UserEvent){ .type = scriptEvent, .code = PARSING_RECORD, .data1 = h, .data2 = pvCurr };
						SDL_PushEvent((SDL_Event*)&userEvent);
					}
					else
					{
						// end of toml script
						userEvent = (SDL_UserEvent){ .type = scriptEvent, .code = PARSING_ENDED, .data1 = h, .data2 = pvCurr };
						SDL_PushEvent((SDL_Event*)&userEvent);
					}
				}
			}
			else if (e.type == scriptEvent)
			{
				if (((SDL_UserEvent*)&e)->code == PARSING_RECORD)
				{
					pvCurr = handle_parsing_record((SDL_UserEvent*)&e);
				}
				else
				{
					goto engine_uninit_label;
				}
			}
		}
	}

engine_uninit_label:
	SDL_FreeSurface(gScreenSurface);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();

script_uninit_label:
	script_uninit(h);

	return ret;
}
