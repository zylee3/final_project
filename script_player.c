#include <assert.h>
#include "parsing.h"
#include "engine.h"

#define PARSING_RECORD	1
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
		// all structures should be start from IdParentChild
		IdParentChild* pIdParentChild = (IdParentChild*)pvCurr;

		switch (pIdParentChild->_id)
		{
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
					const char* background = ((Scene*)pFullNameRec->record)->background;
					//printf("Event scene background:%s\n", background);
					loadBackground(background);
					// clear all characters by loadBackground
					g_characters = 0;
				}
				if ((pEvent->dialogue != NULL) && (pEvent->dialogue[0] != '\0'))
				{
					//printf("Event dialog:%s\n", pEvent->dialogue);
					FullNameRecord* pFullNameRec = script_get_summary_next(h, NULL, pEvent->dialogue);
					// next parsing record
					option = TRAVERSE_JUMP;
					pvCurr = pFullNameRec->record;
					//printf("Event dialog ended\n");
				}
			}
			break;
		case DIALOGUE:
			{
				Dialogue* pDialogue = (Dialogue*)pvCurr;
				if ((pDialogue->character != NULL) && (pDialogue->character[0] != '\0'))
				{
					//printf("Dialog character:%s\n", pDialogue->character);
					FullNameRecord* pFullNameRec = script_get_summary_next(h, NULL, pDialogue->character);
					//printf("pFullNameRec:%p for character\n", pFullNameRec);
					const char* character = ((Character*)pFullNameRec->record)->tachie;
					if (find_loaded_character(character) == NULL)
					{
						loadCharacter(character);
						insert_loaded_character(character);

					}
					loadTextbox();
					const char* name = ((Character*)pFullNameRec->record)->name;
					loadName(name);
				}
				else
				{
					loadTextbox();
				}
				if ((pDialogue->text != NULL) && (pDialogue->text[0] != '\0'))
				{
					//printf("Dialog text:%s\n", pDialogue->text);
					loadDialog(pDialogue->text);
				}
				if ((pDialogue->next != NULL) && (pDialogue->next[0] != '\0'))
				{
					//printf("Dialog next:%s\n", pDialogue->next);
					FullNameRecord* pFullNameRec = script_get_summary_next(h, NULL, pDialogue->next);
					//printf("pFullNameRec:%p for next\n", pFullNameRec);
					return pFullNameRec->record; // direct jump after mouse click 
				}
				option = TRAVERSE_BREAK;
			}
			break;
		default:
			printf("unsupported id %d\n", pIdParentChild->_id);
			option = TRAVERSE_BREAK;
			break;
		}

		//printf("handle_parsing_record 2\n");

		if (option == TRAVERSE_BREAK) { break; }
	}

	if (pvCurr == NULL) { return pvCurr; }

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

	if (!init())
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
						loadBackground("basement");
						loadTextbox();
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
					}
   				}
            }
            else if (e.type == scriptEvent)
            {
        		pvCurr = handle_parsing_record((SDL_UserEvent*)&e);
            }
        }
    }

engine_uninit_label:    
    close();

script_uninit_label:
	script_uninit(h);

	return ret;
}
