#include <stdio.h>
#include "parsing.h"

int32_t main()
{
	parsing_handle h = script_init();
	if (h == NULL) { return 1; }

	int32_t ret = script_parsing(h, "script/script.toml");
	if (ret == 0)
	{

		printf("***paint all\n");
		script_print_all(h);
		printf("***all printed\n\n");

		Found found;
		printf("***find scene.guild:\n");
		found = script_find_record(h, "scene.guild");
		script_print_found(h, &found);
		printf("***scene.guild printed\n\n");

		printf("***find dialogue.conversation.second_one:\n");
		found = script_find_record(h, "dialogue.conversation.second_one");
		script_print_found(h, &found);
		printf("***dialogue.conversation.second_one printed\n\n");

		printf("***find dialogue.conversation.second_one.two:\n");
		found = script_find_record(h, "dialogue.conversation.second_one.two");
		script_print_found(h, &found);
		printf("***dialogue.conversation.second_one.two printed\n\n");
	}
	script_uninit(h);

	return ret;
}
