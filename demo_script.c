#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "toml.h"

const uint32_t MAX_MEM = 0x80000000;

typedef struct _sTomlFile
{
	FILE* pFile;
} TomlFile;

typedef struct _sMemory
{
    void*    pvMem;
    uint32_t size;
    char*    pLast;

} Memory;

typedef struct _sScene
{
	char name[32];
	char background[64];
} Scene;

typedef struct _sCharacter
{
	char     name[32];
	char     avatar[64];
	char     tachie[64];
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

typedef struct _sProgram
{
	TomlFile tomlFile;
    Memory   memory;

} Program;

Memory alloc_memory(uint32_t size)
{
    Memory memory = { .pvMem = NULL, .size = 0, .pLast = NULL };

    const int32_t RETRIES = 40;

    for (int i = 0; i < RETRIES; size /= 2, ++i)
    {
        memory.pvMem = malloc(size);
        if (memory.pvMem == NULL) { continue; }
        memory.size = size;
        memory.pLast = (uint8_t*)memory.pvMem + size;
        break;
    }

    return memory;
}

Program program_init()
{
    Program program =
    {
    };

    return program;
}

int32_t program_execute(Program* pProgram)
{
	// if help, print_help() and return

	const char* script_file = "script/script.toml";

	pProgram->tomlFile.pFile = fopen(script_file, "r");
	if (pProgram->tomlFile.pFile == NULL)
	{
		printf("fopen %s failed\n", script_file);
		return 11;
	}

	pProgram->memory = alloc_memory(MAX_MEM);

	char buf[256] = { '\0' };
	toml_table_t* table = toml_parse_file(pProgram->tomlFile.pFile, buf,
		sizeof(buf) / sizeof(buf[0]));
	if (table == NULL)
	{
		printf("cannot parse - %s\n", buf);
		return 12;
    }

	for (int32_t i = 0;; ++i) {
	    const char* key = toml_key_in(table, i);
	    if (!key) break;
	    printf("key %d: %s\n", i, key);
	}

	return 0;
}

void program_uninit(Program* pProgram)
{
    if (pProgram->memory.pvMem != NULL) { free(pProgram->memory.pvMem); }
    if (pProgram->tomlFile.pFile != NULL) { fclose(pProgram->tomlFile.pFile); }
}

int32_t main()
{
	Program program = program_init();

	int32_t ret = program_execute(&program);

	program_uninit(&program);

	return ret;
}