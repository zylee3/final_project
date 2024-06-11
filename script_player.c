#include <assert.h>
#include "parsing.h"
#include "engine.h"

#define PARSING_RECORD	1

parsing_record handle_parsing_record(SDL_UserEvent* pUserEvent)
{
	assert(pUserEvent->code == PARSING_RECORD);

	parsing_handle h = pUserEvent->data1;
	parsing_record pvCurr = pUserEvent->data2;

	// all structures should be start from IdParentChild
	IdParentChild* pIdParentChild = (IdParentChild*)pvCurr;

	switch (pIdParentChild->_id)
	{
	case FINAL_PROJECT:
		{
			FinalProject* pFinalProject = (FinalProject*)pvCurr;
		}
		break;
	case SCENE:
		{
			Scene* pScene = (Scene*)pvCurr;
		}
		break;
	case CHARACTER:
		{
			Character* pCharacter = (Character*)pvCurr;
		}
		break;
	case ITEM:
		{
			Item* pItem = (Item*)pvCurr;
		}
		break;
	case EVENT:
		{
			Event* pEvent = (Event*)pvCurr;
		}
		break;
	case DIALOGUE:
		{
			Dialogue* pDialogue = (Dialogue*)pvCurr;
		}
		break;
	default:
		printf("unsupported id %d\n", pIdParentChild->_id);
		break;
	}

	return pvCurr;
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
	SDL_PushEvent((SDL_Event*)&userEvent);

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
	                    break;
   					}	
   				}
   				else
   				{
   					// mouse click for next record in toml script
   					pvCurr = script_get_next(h, pvCurr);
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
    SDL_FreeSurface(bgSurface);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

script_uninit_label:
	script_uninit(h);

	return ret;
}
