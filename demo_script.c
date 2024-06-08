#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "toml.h"

#define GET_FULL_NAME_LEN 64
#define MAX_FOUND         16

const uint32_t MAX_MEM = 0x80000000;

typedef struct _sTomlFile
{
	FILE* pFile;
} TomlFile;

typedef struct _sMemory
{
    void*    pvMem;
    uint32_t size;
    void*    pvLast;

} Memory;

typedef struct _sManageMemory
{
	Memory memory;
    void*  pvCurrMem;
    void*  pvCurrLast;
} ManageMemory;

enum StructId
{
	NONE            = 0,
	ID_PARENT_CHILD = 1,
	NO_FOR_PRINT    = ID_PARENT_CHILD,
	TABLE           = 2,
	CONTAINER_START = TABLE,
	ARRAY           = 3,
	CONTAINER_END = ARRAY,
	FINAL_PROJECT   = 4,
	SCENE           = 5,
	CHARACTER       = 6,
	ITEM            = 7,
	EVENT           = 8,
	DIALOGUE        = 9,
};

typedef struct _sStructInfo
{
	enum StructId id;
	size_t        size; // the structre size of structure id
	const char*   structName;
} StructInfo;

struct _sContainer;

typedef struct _sIdParentChild
{
	enum StructId           _id;
	struct _sContainer*     _parent;
	struct _sIdParentChild* _child;
	struct _sIdParentChild* _next;
} IdParentChild;

typedef struct _sContainer
{
	enum StructId       _id;
	struct _sContainer* _parent;
	IdParentChild*      _child;
	IdParentChild*      _next;
	char*               name;
} Container;

typedef struct _sFinalProject
{
	enum StructId  _id;
	Container*     _parent;
	IdParentChild* _child;
	IdParentChild* _next;
	char*          name;
	char*          author;
} FinalProject;

typedef struct _sScene
{
	enum StructId  _id;
	Container*     _parent;
	IdParentChild* _child;
	IdParentChild* _next;
	char*          name;
	char*          background;
} Scene;

typedef struct _sCharacter
{
	enum StructId  _id;
	Container*     _parent;
	IdParentChild* _child;
	IdParentChild* _next;
	char*          name;
	char*          avatar;
	char*          tachie;
	uint32_t       favor_cats;
	uint32_t       favor_dogs;
} Character;

typedef struct _sItem
{
	enum StructId  _id;
	Container*     _parent;
	IdParentChild* _child;
	IdParentChild* _next;
	char*          name;
	char*          image;
} Item;

typedef struct _sEvent
{
	enum StructId  _id;
	Container*     _parent;
	IdParentChild* _child;
	IdParentChild* _next;
	char*          scene;
	char*          dialogue;
} Event;

typedef struct _sDialogue
{
	enum StructId  _id;
	Container*     _parent;
	IdParentChild* _child;
	IdParentChild* _next;
	char*          character;
	char*          text;
	char*          next;
	char*          event;
	char*          item;
} Dialogue;

typedef struct _sFound
{
	uint32_t count;
	void*    pvMatchs[MAX_FOUND];
} Found;

typedef struct _sProgram
{
	TomlFile     tomlFile;
    ManageMemory parsing;
    ManageMemory summary;
} Program;

Memory alloc_memory(Program* pProgram, uint32_t size)
{
    Memory memory = { .pvMem = NULL, .size = 0, .pvLast = NULL };

    const int32_t RETRIES = 40;

    for (int i = 0; i < RETRIES; size /= 2, ++i)
    {
        memory.pvMem = malloc(size);
        if (memory.pvMem == NULL) { continue; }
        memory.size = size;
        memory.pvLast = (uint8_t*)memory.pvMem + size;
        break;
    }

	pProgram->parsing.pvCurrMem = memory.pvMem;
	pProgram->parsing.pvCurrLast = memory.pvLast;

    return memory;
}

Program program_init()
{
    Program program =
    {
    };

    return program;
}

IdParentChild* find_available_child_next(Container* pParent)
{
	if (pParent == NULL) { return NULL; }

	if (pParent->_child == NULL) { return pParent->_child; }

	IdParentChild* pFound = pParent->_child;

	for (IdParentChild* pCheck = pFound->_next; pCheck != NULL;
		pFound = pCheck, pCheck = pCheck->_next) {}

	return pFound;
}

static const StructInfo structInfos[] =
{
	{ .id = NONE           , .size = 0                    , NULL        },
	{ .id = ID_PARENT_CHILD, .size = sizeof(IdParentChild), NULL        },
	{ .id = TABLE          , .size = sizeof(Container)    , NULL        },
	{ .id = ARRAY          , .size = sizeof(Container)    , NULL        },
	{ .id = FINAL_PROJECT  , .size = sizeof(FinalProject) , ""          },
	{ .id = SCENE          , .size = sizeof(Scene)        , "scene"     },
	{ .id = CHARACTER      , .size = sizeof(Character)    , "character" },
	{ .id = ITEM           , .size = sizeof(Item)         , "item"      },
	{ .id = EVENT          , .size = sizeof(Event)        , "event"     },
	{ .id = DIALOGUE       , .size = sizeof(Dialogue)     , "dialogue"  },
};

StructInfo get_struct_info_by_id(enum StructId id)
{
	assert(id == structInfos[id].id);

	return structInfos[id];
}

StructInfo get_struct_info(const char* structName)
{
	if ((structName == NULL) || (*structName == '\0') || (strcmp(structName, "Final Project") == 0))
	{
		return get_struct_info_by_id(FINAL_PROJECT);
	}

	for (int32_t i = 0; i < sizeof(structInfos) / sizeof(structInfos[0]); ++i)
	{
		const StructInfo* pStructInfo = &structInfos[i];
		if (pStructInfo->structName == NULL) { continue; }

		if (strcmp(structName, pStructInfo->structName) == 0)
		{
			return *pStructInfo;
		}
	}

	printf("structure %s not yet supported in get_struct_info\n", structName);

	return get_struct_info_by_id(NONE);
}

// alignment memory
void* get_struct_memory(Program* pProgram, size_t size)
{
	//printf("get_struct_memory size:%ld\n", size);

	void* pvCurrMem = (uint8_t*)pProgram->parsing.pvCurrMem + size;

	if (pvCurrMem > pProgram->parsing.pvCurrLast)
	{
		printf("out of memory for get_struct_memory\n");
		return NULL;
	}

	void* pvMem = (Container*)pProgram->parsing.pvCurrMem;
	memset(pvMem, 0, size);

	pProgram->parsing.pvCurrMem = pvCurrMem;

	return pvMem;
}

// not alignment memory
void* get_string_memory(Program* pProgram, size_t size)
{
	void* pvCurrLast = (uint8_t*)pProgram->parsing.pvCurrLast - size;

	if (pProgram->parsing.pvCurrMem > pvCurrLast)
	{
		printf("out of memory for get_string_memory\n");
		return NULL;
	}

	return pProgram->parsing.pvCurrLast = pvCurrLast;
}

Container* get_container_memory(Program* pProgram, Container* pParent, const char* containerName)
{
	size_t len = (containerName != NULL) ? strlen(containerName) + 1 : 1;
	//printf("get_container_memory containerName:%s len:%ld\n", containerName, len);

	void* pvCurrMem = (uint8_t*)pProgram->parsing.pvCurrMem + sizeof(Container);
	void* pvCurrLast = (uint8_t*)pProgram->parsing.pvCurrLast - len;

	if (pvCurrMem > pvCurrLast)
	{
		printf("out of memory for get_container_memory\n");
		return NULL;
	}

	Container* pContainer = (Container*)pProgram->parsing.pvCurrMem;
	if (containerName != NULL)
	{
		strcpy(pvCurrLast, containerName);
	}
	else
	{
		*(char*)pvCurrLast = '\0';
	}

	pProgram->parsing.pvCurrMem = pvCurrMem;
	pProgram->parsing.pvCurrLast = pvCurrLast;

	pContainer->name = pvCurrLast;
	pContainer->_parent = pParent;
	pContainer->_child = get_struct_memory(pProgram, sizeof(IdParentChild));
	if (pContainer->_child == NULL)
	{
		printf("out of child memory for get_container_memory\n");
		return NULL;
	}

	pContainer->_child->_id = ID_PARENT_CHILD;
	IdParentChild* pFound = find_available_child_next(pContainer);
	pFound->_next = (IdParentChild*)pContainer;

	return pContainer;
}

const char* get_struct_name(void* pCurr)
{
	if (pCurr == NULL) { return NULL; }

	Container* pFound = pCurr;
	for (Container* it = pCurr; it != NULL; it = it->_parent)
	{
		if (it->name[0] != '\0')
		{
			pFound = it;
		}
		//printf("get_struct_name it->name:%s\n", it->name);
	}

	//printf("get_struct_name pFound->name:%s\n", pFound->name);
	return pFound->name;
}

void get_full_name(Container* pCurr, char* fullName)
{
	*fullName = '\0';

	if (pCurr == NULL) { return; }

	if (pCurr->_id > CONTAINER_END) { pCurr = pCurr->_parent; }

	if (pCurr == NULL) { return; }

	Container* allContainers[16] = { NULL };
	int32_t containers = 0;

	for (Container* it = pCurr; it != NULL; it = it->_parent)
	{
		if (it->name == NULL) { continue; }
		if (it->name[0] == '\0') { continue; }

		allContainers[containers++] = it;
	}

	if (containers > sizeof(allContainers) / sizeof(allContainers[0]))
	{
		printf("allContainers is not enough in get_full_name\n");
		return;
	}

	size_t len = 0;

	for (int32_t i = containers - 1; i >= 0; --i)
	{
		Container* it = allContainers[i];

		if (len == 0)
		{
			strcpy(fullName + len, it->name);
		}
		else
		{
			*(fullName + len) = '.';
			++len;
			strcpy(fullName + len, it->name);
		}
		len += strlen(it->name);
		//printf("get_full_name it->name:%s\n", it->name);
	}

	return;
}

int32_t fill_struct_string_field(Program* pProgram, void* pvMem, const char* structName,
	const char* key, const char* string)
{
	//printf("fill_struct_string_field structName:%s key:%s string:%s\n", structName, key, string);

	size_t len = (string != NULL) ? strlen(string) + 1: 1;
	char* pcMemStr = get_string_memory(pProgram, len);
	strcpy(pcMemStr, string);

	if ((structName == NULL) || (*structName == '\0') || (strcmp(structName, "Final Project") == 0))
	{
		if (strcmp(key, "name") == 0)
		{
			((FinalProject*)pvMem)->name = pcMemStr;
		}
		else if (strcmp(key, "author") == 0)
		{
			((FinalProject*)pvMem)->author = pcMemStr;
		}
		else
		{
			printf("structure '%s' not yet supported field '%s' in fill_struct_string_field\n", structName, key);
			return 51;
		}
	}
	else if (strcmp(structName, "item") == 0)
	{
		if (strcmp(key, "name") == 0)
		{
			((Item*)pvMem)->name = pcMemStr;
		}
		else if (strcmp(key, "image") == 0)
		{
			((Item*)pvMem)->image = pcMemStr;
		}
		else
		{
			printf("structure '%s' not yet supported field '%s' in fill_struct_string_field\n", structName, key);
			return 52;
		}
	}
	else if (strcmp(structName, "scene") == 0)
	{
		if (strcmp(key, "name") == 0)
		{
			((Scene*)pvMem)->name = pcMemStr;
		}
		else if (strcmp(key, "background") == 0)
		{
			((Scene*)pvMem)->background = pcMemStr;
		}
		else
		{
			printf("structure '%s' not yet supported field '%s' in fill_struct_string_field\n", structName, key);
			return 53;
		}
	}
	else if (strcmp(structName, "character") == 0)
	{
		if (strcmp(key, "name") == 0)
		{
			((Character*)pvMem)->name = pcMemStr;
		}
		else if (strcmp(key, "avatar") == 0)
		{
			((Character*)pvMem)->avatar = pcMemStr;
		}
		else if (strcmp(key, "tachie") == 0)
		{
			((Character*)pvMem)->tachie = pcMemStr;
		}
		else
		{
			printf("structure '%s' not yet supported field '%s' in fill_struct_string_field\n", structName, key);
			return 54;
		}
	}
	else if (strcmp(structName, "event") == 0)
	{
		if (strcmp(key, "scene") == 0)
		{
			((Event*)pvMem)->scene = pcMemStr;
		}
		else if (strcmp(key, "dialogue") == 0)
		{
			((Event*)pvMem)->dialogue = pcMemStr;
		}
		else
		{
			printf("structure '%s' not yet supported field '%s' in fill_struct_string_field\n", structName, key);
			return 55;
		}
	}
	else if (strcmp(structName, "dialogue") == 0)
	{
		if (strcmp(key, "character") == 0)
		{
			((Dialogue*)pvMem)->character = pcMemStr;
		}
		else if (strcmp(key, "text") == 0)
		{
			((Dialogue*)pvMem)->text = pcMemStr;
		}
		else if (strcmp(key, "next") == 0)
		{
			((Dialogue*)pvMem)->next = pcMemStr;
		}
		else if (strcmp(key, "event") == 0)
		{
			((Dialogue*)pvMem)->event = pcMemStr;
		}
		else if (strcmp(key, "item") == 0)
		{
			((Dialogue*)pvMem)->item = pcMemStr;
		}
		else
		{
			printf("structure '%s' not yet supported field '%s' in fill_struct_string_field\n", structName, key);
			return 56;
		}
	}
	else
	{
		printf("structure '%s' not yet supported in fill_struct_string_field\n", structName);
		return 57;
	}

	return 0;
}

int32_t fill_struct_int_field(Program* pProgram, void* pvMem, const char* structName, const char* key, uint32_t i)
{
	if (strcmp(structName, "character") == 0)
	{
		if (strcmp(key, "favor_cats") == 0)
		{
			((Character*)pvMem)->favor_cats = i;
		}
		else if (strcmp(key, "favor_dogs") == 0)
		{
			((Character*)pvMem)->favor_dogs = i;
		}
		else
		{
			printf("structure %s not yet supported field %s in fill_struct_int_field\n", structName, key);
			return 61;
		}
	}
	else
	{
		printf("structure %s not yet supported in fill_struct_int_field\n", structName);
		return 62;
	}

	return 0;
}

void print_record_indent(void* pvMem, int32_t indent)
{
	if (pvMem == NULL) { return; }

	if (indent > 0)
	{
		char buf[64] = { '\0' };
		for (int32_t i = 0; i < indent; ++i) { buf[i] = ' '; }
		buf[indent] = '\0';
		if (buf[sizeof(buf) / sizeof(buf[0]) - 1] != '\0')
		{
			printf("buf is not big enough\n");
		}
		printf("%s", buf);
	}

	enum StructId id = ((IdParentChild*)pvMem)->_id;
	char fullName[GET_FULL_NAME_LEN] = { '\0' };

	switch (id)
	{
	case ID_PARENT_CHILD:
		{
			IdParentChild* pIdParentChild = (IdParentChild*)pvMem;
			Container* pParent = pIdParentChild->_parent;
			get_full_name(pParent, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("IdParentChild parent name:'%s' fullName:'%s'\n",
				(pParent != NULL) ? pParent->name : "<null>", fullName);
		}
		break;
	case TABLE:
	case ARRAY:
		{
			Container* pContainer = (Container*)pvMem;
			Container* pParent = pContainer->_parent;
			get_full_name(pContainer, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("%s name:'%s' parent name:'%s' fullName:'%s'\n",
				(pContainer->_id == TABLE) ? "Table" : "Array",
				pContainer->name, (pParent != NULL) ? pParent->name : "<null>", fullName);

			if ((pParent != NULL) && (pParent->name[0] != '\0')) { print_record_indent(pParent, indent + 1); }
		}
		break;
	case FINAL_PROJECT:
		{
			FinalProject* pFinalProject = ((FinalProject*)pvMem);
			Container* pParent = pFinalProject->_parent;
			get_full_name(pParent, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("FinalProject name:'%s' author:'%s' parent name:'%s' fullName:'%s'\n",
				pFinalProject->name, pFinalProject->author, pParent->name, fullName);
		}
		break;
	case SCENE:
		{
			Scene* pScene = ((Scene*)pvMem);
			Container* pParent = pScene->_parent;
			get_full_name(pParent, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("Scene name:'%s' background:'%s' parent name:'%s' fullName:'%s'\n",
				pScene->name, pScene->background, pParent->name, fullName);
		}
		break;
	case CHARACTER:
		{
			Character* pCharacter = ((Character*)pvMem);
			Container* pParent = pCharacter->_parent;
			get_full_name(pParent, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("Character name:'%s' avatar:'%s' tachie:'%s' favor_cats:%d favor_dogs:%d"
				" parent name:'%s' fullName:'%s'\n",
				pCharacter->name, pCharacter->avatar, pCharacter->tachie,
				pCharacter->favor_cats, pCharacter->favor_dogs, pParent->name, fullName);
		}
		break;
	case ITEM:
		{
			Item* pItem = ((Item*)pvMem);
			Container* pParent = pItem->_parent;
			get_full_name(pParent, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("Item name:'%s' image:'%s' parent name:'%s' fullName:'%s'\n",
				pItem->name, pItem->image, pParent->name, fullName);
		}
		break;
	case EVENT:
		{
			Event* pEvent = ((Event*)pvMem);
			Container* pParent = pEvent->_parent;
			get_full_name(pParent, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("Event scene:'%s' dialogue:'%s' parent name:'%s' fullName:'%s'\n",
				pEvent->scene, pEvent->dialogue, pParent->name, fullName);
		}
		break;
	case DIALOGUE:
		{
			Dialogue* pDialogue = ((Dialogue*)pvMem);
			Container* pParent = pDialogue->_parent;
			get_full_name(pParent, fullName);
			if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
			{
				printf("fullName is not big enough\n");
			}
			printf("Dialogue character:'%s' text:'%s' next:'%s' event:'%s' item:'%s'"
				" parent name:'%s' fullName:'%s'\n",
				pDialogue->character, pDialogue->text, pDialogue->next, pDialogue->event,
					pDialogue->item, pParent->name, fullName);
		}
		break;
	default:
		printf("invalid IdParentChild->id %d", id);
		break;
	}
}

void print_record(void* pvMem)
{
	print_record_indent(pvMem, 0);
}

Container* read_table(Program* pProgram, toml_table_t* pTomlTable, Container* pParent, const char* tableName);

Container* read_array(Program* pProgram, toml_array_t* pTomlArray, Container* pParent, const char* key)
{
	//printf("read_array 1 key:'%s' Parent:'%s'\n", key, (pParent != NULL) ? pParent->name: "<null>");
	Container* pArray = get_container_memory(pProgram, pParent, key);
	if (pArray == NULL) { return pArray; }

	pArray->_id = ARRAY;

	toml_array_t* arr = NULL;
	toml_table_t* pTomlChildTable = NULL;
	toml_datum_t d;

    const char* structName = NULL;
    StructInfo si = { .id = NONE, .size = 0 };
    void* pvMem = NULL;

	int n = toml_array_nelem(pTomlArray);

	for (int32_t i = 0; i < n; ++i)
	{
		//printf("read_array 11 i:%d\n", i);
		if (NULL != (pTomlChildTable = toml_table_at(pTomlArray, i)))
		{
			Container* pTable = read_table(pProgram, pTomlChildTable, pArray, "");
			IdParentChild* pFound = find_available_child_next(pArray);
			pFound->_next = (IdParentChild*)pTable;
			continue;
		}

		printf("Other types are not supported for now in read_array.\n");
		break;
	}

	return pArray;
}

Container* read_table(Program* pProgram, toml_table_t* pTomlTable, Container* pParent, const char* tableName)
{
	//printf("read_table 1 tableName:'%s' Parent:'%s'\n", tableName, (pParent != NULL) ? pParent->name: "<null>");
	Container* pTable = get_container_memory(pProgram, pParent, tableName);
	if (pTable == NULL) { return pTable; }

	pTable->_id = TABLE;

	const char* key = NULL;
	toml_array_t* arr = NULL;
	toml_table_t* pTomlChildTable = NULL;
	toml_datum_t d;

    const char* structName = NULL;
    StructInfo si = { .id = NONE, .size = 0 };
    void* pvMem = NULL;

	for (int32_t i = 0; NULL != (key = toml_key_in(pTomlTable, i)); ++i)
	{
		//printf("read_table 11 i:%d\n", i);
		if (NULL != (arr = toml_array_in(pTomlTable, key)))
		{
			read_array(pProgram, arr, pTable, key);
			continue;
	    }
		if (NULL != (pTomlChildTable = toml_table_in(pTomlTable, key)))
		{
			read_table(pProgram, pTomlChildTable, pTable, key);
			continue;
		}

		d = toml_string_in(pTomlTable, key);
		if (d.ok)
		{
			if (structName == NULL)
			{
				structName = get_struct_name(pTable);
				if (structName == NULL) { return NULL; }
			}
			if (si.size == 0)
			{
				si = get_struct_info(structName);
				if (si.size == 0) { return NULL; }
			}
			if (pvMem == NULL)
			{
				pvMem = get_struct_memory(pProgram, si.size);
				if (pvMem == NULL) { return NULL; }
				((IdParentChild*)pvMem)->_id = si.id;
				((IdParentChild*)pvMem)->_parent = pTable;
				IdParentChild* pFound = find_available_child_next(pTable);
				pFound->_next = (IdParentChild*)pvMem;

			}
			//printf("read_table 12 i:%d id:%d size:%ld\n", i, si.id, si.size);
			fill_struct_string_field(pProgram, pvMem, structName, key, d.u.s);
			//printf("read_table 13 i:%d\n", i);
			continue;
		}

		d = toml_int_in(pTomlTable, key);
		if (d.ok)
		{
			//printf("read_table 21 i:%d\n", i);
			if (structName == NULL)
			{
				structName = get_struct_name(pTable);
				if (structName == NULL) { return NULL; }
			}
			if (si.size == 0)
			{
				si = get_struct_info(structName);
				if (si.size == 0) { return NULL; }
			}
			if (pvMem == NULL)
			{
				pvMem = get_struct_memory(pProgram, si.size);
				if (pvMem == NULL) { return NULL; }
				((IdParentChild*)pvMem)->_id = si.id;
				((IdParentChild*)pvMem)->_parent = pTable;
				IdParentChild* pFound = find_available_child_next(pTable);
				pFound->_next = (IdParentChild*)pvMem;
			}
			fill_struct_int_field(pProgram, pvMem, structName, key, d.u.i);
			continue;
		}

		printf("Other types are not supported for now in read_table.\n");
		break;
	}

	//printf("read_table 31\n");
	//print_record(pTable);
	//print_record(pvMem);
	return pTable;
}

int32_t parse_toml(Program* pProgram)
{
	const char* script_file = "script/script.toml";

	pProgram->tomlFile.pFile = fopen(script_file, "r");
	if (pProgram->tomlFile.pFile == NULL)
	{
		printf("fopen %s failed\n", script_file);
		return 11;
	}

	pProgram->parsing.memory = alloc_memory(pProgram, MAX_MEM);
	if (pProgram->parsing.memory.pvMem == NULL)
	{
		printf("out of memory for alloc_memory");
		return 12;
	}

	char buf[256] = { '\0' };
	toml_table_t* toml_table = toml_parse_file(pProgram->tomlFile.pFile, buf,
		sizeof(buf) / sizeof(buf[0]));
	if (toml_table == NULL)
	{
		printf("cannot parse - %s\n", buf);
		return 13;
    }

    int32_t ret = 0;

    // init key is ""
	Container* pContainer = read_table(pProgram, toml_table, NULL, "");
	if (pContainer == NULL) { ret = 14; }

	toml_free(toml_table);

	return ret;
}

void printf_all_records(Program* pProgram)
{
	for (void* pvCurrRec = pProgram->parsing.memory.pvMem; pvCurrRec < pProgram->parsing.pvCurrMem; )
	{
		print_record(pvCurrRec);
		StructInfo si = get_struct_info_by_id(((IdParentChild*)pvCurrRec)->_id);
		pvCurrRec += si.size;
	}
}

Found find_record(Program* pProgram, const char* tomlKey)
{
	Found found = { .count = 0 };

	for (void* pvCurrRec = pProgram->parsing.memory.pvMem; pvCurrRec < pProgram->parsing.pvCurrMem; )
	{
		do
		{
			//print_record(pvCurrRec);
			IdParentChild* pIdParentChild = (IdParentChild*)pvCurrRec;

			if (pIdParentChild->_id <= NO_FOR_PRINT)
			{
				//printf("skipped 1\n");
				break;
			}

			char fullName[GET_FULL_NAME_LEN];

			if ((pIdParentChild->_id >= CONTAINER_START) && (pIdParentChild->_id <= CONTAINER_END))
			{
				//get_full_name((Container*)pIdParentChild, fullName);
				//printf("skipped 2 fullName:%s\n", fullName);
				break;
			}

			get_full_name(pIdParentChild->_parent, fullName);
			if (strstr(fullName, tomlKey) != fullName)
			{
				//printf("skipped 3 fullName:%s\n", fullName);
				break;
			}

			found.pvMatchs[found.count++] = pvCurrRec;
			assert(found.count <= sizeof(found.pvMatchs) / sizeof(found.pvMatchs[0]));
		}
		while (0);

		StructInfo si = get_struct_info_by_id(((IdParentChild*)pvCurrRec)->_id);
		pvCurrRec += si.size;
	}

	return found;
}

void print_found(Found* pFound)
{
	for (int32_t i = 0; i < pFound->count; ++i)
	{
		print_record(pFound->pvMatchs[i]);
	}
}

int32_t program_execute(Program* pProgram)
{
	// if help, print_help() and return

	int32_t ret = parse_toml(pProgram);
	if (ret != 0) { return ret; }

	//printf_all_records(pProgram);
	//printf("\nall records printed\n");

	Found found;

	printf("***find scene.guild:\n");
	found = find_record(pProgram, "scene.guild");
	print_found(&found);
	printf("***scene.guild printed\n\n");

	printf("***find dialogue.conversation.home:\n");
	found = find_record(pProgram, "dialogue.conversation.home");
	print_found(&found);
	printf("***dialogue.conversation.home printed\n\n");

	printf("***find dialogue.conversation.home.one:\n");
	found = find_record(pProgram, "dialogue.conversation.home.one");
	print_found(&found);
	printf("***dialogue.conversation.home.one\n\n");

	return ret;
}

void program_uninit(Program* pProgram)
{
    if (pProgram->parsing.memory.pvMem != NULL) { free(pProgram->parsing.memory.pvMem); }
    if (pProgram->tomlFile.pFile != NULL) { fclose(pProgram->tomlFile.pFile); }
}

int32_t main()
{
	Program program = program_init();

	int32_t ret = program_execute(&program);

	program_uninit(&program);

	return ret;
}
