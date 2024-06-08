#include <stdio.h>
#include <stdbool.h>
#include "parsing.h"

int32_t main()
{
	parsing_handle h = script_init();
	if (h == NULL) { return 1; }

	int32_t ret = script_parsing(h, "script/script.toml");
	if (ret == 0)
	{

		printf("***print all\n");
		script_print_all(h);
		printf("***all printed\n\n");

		Found found;
		printf("***find scene.guild:\n");
		found = script_find_record(h, "scene.guild");
		script_print_found(&found);
		printf("***scene.guild printed\n\n");

		printf("***find dialogue.conversation.second_one:\n");
		found = script_find_record(h, "dialogue.conversation.second_one");
		script_print_found(&found);
		printf("***dialogue.conversation.second_one printed\n\n");

		printf("***find dialogue.conversation.second_one.two:\n");
		found = script_find_record(h, "dialogue.conversation.second_one.two");
		script_print_found(&found);
		printf("***dialogue.conversation.second_one.two printed\n\n");

		Structures all = script_get_all(h);

		StructInfo si;

		for (void* pvCurr = all.pvStart; pvCurr < all.pvEnd; pvCurr += si.size)
		{
			// all structures should be start from IdParentChild
			IdParentChild* pIdParentChild = (IdParentChild*)pvCurr;

			bool wanted = !(pIdParentChild->_id <= CONTAINER_END);

			if (wanted)
			{
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

			si = script_get_struct_info_by_id(pIdParentChild->_id);
		}
	}

	script_uninit(h);

	return ret;
}
