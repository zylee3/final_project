#include <stdio.h>
#include <stdbool.h>
#include "parsing.h"

int32_t main(int argc, char* args[])
{
	if (argc < 2)
	{
		printf("This program need a toml file argument!\n"
			"for ex.:\n"
			"    ./demo_script script/script.toml\n");
		return 1;
	}

	parsing_handle h = script_init();
	if (h == NULL) { return 2; }

	int32_t ret = script_parsing(h, args[1]);
	ret = script_summarize(h);
	if (ret == 0)
	{

		printf("***print all\n");
		script_print_all(h);
		printf("***all printed\n\n");

		Find find;
		printf("***find scene.guild:\n");
		find = script_find_record(h, "scene.guild");
		script_print_find(&find);
		script_release_find(&find);
		printf("***scene.guild printed\n\n");

		printf("***find dialogue.conversation.second_one:\n");
		find = script_find_record(h, "dialogue.conversation.second_one");
		script_print_find(&find);
		script_release_find(&find);
		printf("***dialogue.conversation.second_one printed\n\n");

		printf("***find dialogue.conversation.second_one.two:\n");
		find = script_find_record(h, "dialogue.conversation.second_one.two");
		script_print_find(&find);
		script_release_find(&find);
		printf("***dialogue.conversation.second_one.two printed\n\n");

		for (parsing_record pvCurr = script_get_next(h, NULL); pvCurr != NULL; pvCurr = script_get_next(h, pvCurr))
		{
			// all structures should be start from IdParentChild
			IdParentChild* pIdParentChild = (IdParentChild*)pvCurr;

			char fullName[GET_FULL_NAME_LEN] = { '\0' };

			script_get_full_name(pIdParentChild, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}

			switch (pIdParentChild->_id)
			{
			case FINAL_PROJECT:
				{
					FinalProject* pFinalProject = (FinalProject*)pvCurr;
					printf("FinalProject name:%s fullName:%s\n", pFinalProject->name, fullName);
				}
				break;
			case SCENE:
				{
					Scene* pScene = (Scene*)pvCurr;
					printf("Scene name:%s fullName:%s\n", pScene->name, fullName);
				}
				break;
			case CHARACTER:
				{
					Character* pCharacter = (Character*)pvCurr;
					printf("Character name:%s fullName:%s\n", pCharacter->name, fullName);
				}
				break;
			case ITEM:
				{
					Item* pItem = (Item*)pvCurr;
					printf("Item name:%s fullName:%s\n", pItem->name, fullName);
				}
				break;
			case EVENT:
				{
					Event* pEvent = (Event*)pvCurr;
					printf("Event scene:%s fullName:%s\n", pEvent->scene, fullName);
				}
				break;
			case DIALOGUE:
				{
					Dialogue* pDialogue = (Dialogue*)pvCurr;
					printf("Dialogue character:%s fullName:%s\n", pDialogue->character, fullName);
				}
				break;
			default:
				printf("unsupported id %d\n", pIdParentChild->_id);
				break;
			}
		}

        for (FullNameRecord* pFullNameRecord = script_get_summary_next(h, NULL, "character.squirrel_one");
			pFullNameRecord != NULL; pFullNameRecord = script_get_summary_next(h, pFullNameRecord, "character.squirrel_one"))
		{
			script_print_record(pFullNameRecord->record);
        }

	}

	script_uninit(h);

	return ret;
}
