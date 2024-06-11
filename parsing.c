#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <toml.h>
#include "parsing.h"

const uint32_t MAX_PARSING_MEM    = 0x08000000;
const uint32_t MAX_SUMMARY_MEM    = 0x04000000;
const uint32_t INIT_FIND_CAPACITY = 16;

enum Target
{
	TARGET_NONE    = 0,
	TARGET_ALL     = 1,
	TARGET_CONTENT_ONLY = 2,
};

typedef struct _sStructInfo
{
	enum StructId id;
	size_t        size; // the structre size of structure id
	const char*   structName;
} StructInfo;

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

typedef struct _sProgram
{
	TomlFile     tomlFile;
	ManageMemory parsing;
	ManageMemory summary;
} Program;

void alloc_memory(ManageMemory* pManageMemory, uint32_t size)
{
    const int32_t RETRIES = 40;

    for (int i = 0; i < RETRIES; size /= 2, ++i)
    {
        pManageMemory->memory.pvMem = malloc(size);
        if (pManageMemory->memory.pvMem == NULL) { continue; }
        pManageMemory->memory.size = size;
        pManageMemory->memory.pvLast = (uint8_t*)pManageMemory->memory.pvMem + size;
        break;
    }

	pManageMemory->pvCurrMem = pManageMemory->memory.pvMem;
	pManageMemory->pvCurrLast = pManageMemory->memory.pvLast;

    return;
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
parsing_record get_struct_memory(ManageMemory* pManageMemory, size_t size)
{
	//printf("get_struct_memory size:%ld\n", size);

	parsing_record pvCurrMem = (uint8_t*)pManageMemory->pvCurrMem + size;

	if (pvCurrMem > pManageMemory->pvCurrLast)
	{
		printf("out of memory in get_struct_memory\n");
		return NULL;
	}

	parsing_record pvMem = (Container*)pManageMemory->pvCurrMem;
	memset(pvMem, 0, size);

	pManageMemory->pvCurrMem = pvCurrMem;

	return pvMem;
}

// not alignment memory
void* get_string_memory(ManageMemory* pManageMemory, size_t size)
{
	void* pvCurrLast = (uint8_t*)pManageMemory->pvCurrLast - size;

	if (pManageMemory->pvCurrMem > pvCurrLast)
	{
		printf("out of memory in get_string_memory\n");
		return NULL;
	}

	return pManageMemory->pvCurrLast = pvCurrLast;
}

Container* get_container_memory(ManageMemory* pManageMemory, Container* pParent, const char* containerName)
{
	size_t len = (containerName != NULL) ? strlen(containerName) + 1 : 1;
	//printf("get_container_memory containerName:%s len:%ld\n", containerName, len);

	parsing_record pvCurrMem = (uint8_t*)pManageMemory->pvCurrMem + sizeof(Container);
	void* pvCurrLast = (uint8_t*)pManageMemory->pvCurrLast - len;

	if (pvCurrMem > pvCurrLast)
	{
		printf("out of memory in get_container_memory\n");
		return NULL;
	}

	Container* pContainer = (Container*)pManageMemory->pvCurrMem;
	if (containerName != NULL)
	{
		strcpy(pvCurrLast, containerName);
	}
	else
	{
		*(char*)pvCurrLast = '\0';
	}

	pManageMemory->pvCurrMem = pvCurrMem;
	pManageMemory->pvCurrLast = pvCurrLast;

	pContainer->name = pvCurrLast;
	pContainer->_parent = pParent;
	pContainer->_child = get_struct_memory(pManageMemory, sizeof(IdParentChild));
	if (pContainer->_child == NULL)
	{
		printf("get_struct_memory failed in get_container_memory\n");
		return NULL;
	}

	pContainer->_child->_id = ID_PARENT_CHILD;
	IdParentChild* pFound = find_available_child_next(pContainer);
	pFound->_next = (IdParentChild*)pContainer;

	return pContainer;
}

const char* get_struct_name(parsing_record pCurr)
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

int32_t fill_struct_string_field(ManageMemory* pManageMemory, parsing_record pvMem, const char* structName,
	const char* key, const char* string)
{
	//printf("fill_struct_string_field structName:%s key:%s string:%s\n", structName, key, string);

	size_t len = (string != NULL) ? strlen(string) + 1: 1;
	char* pcMemStr = get_string_memory(pManageMemory, len);
	if (pcMemStr == NULL)
	{
		printf("get_string_memory failed in fill_struct_string_field(,, %s, %s)",
			key, string);
		return 51;
	}
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
			return 52;
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
			return 53;
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
			return 54;
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
			return 55;
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
			return 56;
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
			return 57;
		}
	}
	else
	{
		printf("structure '%s' not yet supported in fill_struct_string_field\n", structName);
		return 58;
	}

	return 0;
}

int32_t fill_struct_int_field(ManageMemory* pManageMemory, parsing_record pvMem, const char* structName, const char* key, uint32_t i)
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

void print_record_indent(parsing_record pvMem, int32_t indent)
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

void print_record(parsing_record pvMem)
{
	print_record_indent(pvMem, 0);
}

Container* read_table(Program* pProgram, toml_table_t* pTomlTable, Container* pParent, const char* tableName);

Container* read_array(Program* pProgram, toml_array_t* pTomlArray, Container* pParent, const char* key)
{
	//printf("read_array 1 key:'%s' Parent:'%s'\n", key, (pParent != NULL) ? pParent->name: "<null>");
	Container* pArray = get_container_memory(&pProgram->parsing, pParent, key);
	if (pArray == NULL)
	{
		printf("get_container_memory failed in read_array\n");
		return pArray;
	}

	pArray->_id = ARRAY;

	toml_array_t* arr = NULL;
	toml_table_t* pTomlChildTable = NULL;

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
	Container* pTable = get_container_memory(&pProgram->parsing, pParent, tableName);
	if (pTable == NULL)
	{
		printf("get_container_memory failed in read_table\n");
		return pTable;
	}

	pTable->_id = TABLE;

	const char* key = NULL;
	toml_array_t* arr = NULL;
	toml_table_t* pTomlChildTable = NULL;
	toml_datum_t d;

    const char* structName = NULL;
    StructInfo si = { .id = NONE, .size = 0 };
    parsing_record pvMem = NULL;

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
				if (structName == NULL)
				{
					printf("get_struct_name failed for toml_string_in in read_table\n");
					return NULL;
				}
			}
			if (si.size == 0)
			{
				si = get_struct_info(structName);
				if (si.size == 0) { return NULL; }
			}
			if (pvMem == NULL)
			{
				pvMem = get_struct_memory(&pProgram->parsing, si.size);
				if (pvMem == NULL)
				{
					printf("get_struct_memory failed for toml_string_in in read_table\n");
					return NULL;
				}
				((IdParentChild*)pvMem)->_id = si.id;
				((IdParentChild*)pvMem)->_parent = pTable;
				IdParentChild* pFound = find_available_child_next(pTable);
				pFound->_next = (IdParentChild*)pvMem;

			}
			//printf("read_table 12 i:%d id:%d size:%ld\n", i, si.id, si.size);
			fill_struct_string_field(&pProgram->parsing, pvMem, structName, key, d.u.s);
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
				if (structName == NULL)
				{
					printf("get_struct_name failed for toml_int_in in read_table\n");
					return NULL;
				}
			}
			if (si.size == 0)
			{
				si = get_struct_info(structName);
				if (si.size == 0) { return NULL; }
			}
			if (pvMem == NULL)
			{
				pvMem = get_struct_memory(&pProgram->parsing, si.size);
				if (pvMem == NULL)
				{
					printf("get_struct_memory failed for toml_int_in in read_table\n");
					return NULL;
				}
				((IdParentChild*)pvMem)->_id = si.id;
				((IdParentChild*)pvMem)->_parent = pTable;
				IdParentChild* pFound = find_available_child_next(pTable);
				pFound->_next = (IdParentChild*)pvMem;
			}
			fill_struct_int_field(&pProgram->parsing, pvMem, structName, key, d.u.i);
			continue;
		}

		printf("Other types are not supported for now in read_table.\n");
		break;
	}

	//printf("read_table 31\n");
	//print_record(pTable);
	//printf("read_table 32\n");
	//print_record(pvMem);
	//printf("read_table 33\n");

	return pTable;
}

parsing_record get_next(Program* pProgram, parsing_record pvCurrRec, enum Target target)
{
	for (; pvCurrRec < pProgram->parsing.pvCurrMem;)
	{
		IdParentChild* pIdParentChild;

		if (pvCurrRec != NULL)
		{
			pIdParentChild = (IdParentChild*)pvCurrRec;
			StructInfo si = get_struct_info_by_id(pIdParentChild->_id);
			pvCurrRec = (uint8_t*)pvCurrRec + si.size;
		}
		else
		{
			pvCurrRec = pProgram->parsing.memory.pvMem;
		}

		pIdParentChild = (IdParentChild*)pvCurrRec;
		bool wanted = true;
		if (target == TARGET_CONTENT_ONLY)
		{
			wanted = !(pIdParentChild->_id <= CONTAINER_END);
		}
		if (wanted) { return pvCurrRec; }
	}

	return NULL;
}

FullNameRecord* get_fullname_record_memory(ManageMemory* pManageMemory,
	const char* fullName, int32_t order, parsing_record record)
{
	size_t len = (fullName != NULL) ? strlen(fullName) + 1 : 1;
	//printf("get_fullname_record_memory fullName:%s len:%ld\n", fullName, len);

	parsing_record pvCurrMem = (uint8_t*)pManageMemory->pvCurrMem + sizeof(FullNameRecord);
	void* pvCurrLast = (uint8_t*)pManageMemory->pvCurrLast - len;

	if (pvCurrMem > pvCurrLast)
	{
		printf("out of memory in get_fullname_record_memory\n");
		return NULL;
	}

	FullNameRecord* pFullNameRecord = (FullNameRecord*)pManageMemory->pvCurrMem;
	if (fullName != NULL)
	{
		strcpy(pvCurrLast, fullName);
	}
	else
	{
		*(char*)pvCurrLast = '\0';
	}

	pManageMemory->pvCurrMem = pvCurrMem;
	pManageMemory->pvCurrLast = pvCurrLast;

	pFullNameRecord->fullName = pvCurrLast;
	pFullNameRecord->order = order;
	pFullNameRecord->record = record;

	return pFullNameRecord;
}

int cmp(const void* pvLhs, const void* pvRhs)
{
	const FullNameRecord* pLhs = (FullNameRecord*)pvLhs;
	const FullNameRecord* pRhs = (FullNameRecord*)pvRhs;

	int c = strcmp(pLhs->fullName, pRhs->fullName);
	if (c == 0)
	{
		return pLhs->order < pRhs->order;
	}

	return c;
}

int32_t summarize(Program* pProgram, enum Target target)
{
	if (pProgram->summary.memory.pvMem != NULL) { return 0; }

	alloc_memory(&pProgram->summary, MAX_SUMMARY_MEM);
	if (pProgram->summary.memory.pvMem == NULL)
	{
		printf("alloc_memory failed for pProgram->summary in summary");
		return 101;
	}

	parsing_record pvStartRec = get_next(pProgram, NULL, target);
	int32_t count = 0;

	for (parsing_record pvCurrRec = pvStartRec; pvCurrRec != NULL;
		pvCurrRec = get_next(pProgram, pvCurrRec, target), ++count)
	{
		// all structures should be start from IdParentChild
		IdParentChild* pIdParentChild = (IdParentChild*)pvCurrRec;

		char fullName[GET_FULL_NAME_LEN] = { '\0' };

		script_get_full_name(pIdParentChild, fullName);
		if (fullName[sizeof(fullName) / sizeof(fullName[0]) - 1] != '\0')
		{
			printf("fullName is not big enough in script_summarize\n");
			return 102;
		}

		get_fullname_record_memory(&pProgram->summary, fullName, count, pvCurrRec);
	}

	qsort(pProgram->summary.memory.pvMem, count, sizeof(FullNameRecord), cmp);

	return 0;
}

int32_t parse_toml(Program* pProgram, const char* scriptFile)
{
	pProgram->tomlFile.pFile = fopen(scriptFile, "r");
	if (pProgram->tomlFile.pFile == NULL)
	{
		printf("fopen %s failed in parse_toml\n", scriptFile);
		return 11;
	}

	alloc_memory(&pProgram->parsing, MAX_PARSING_MEM);
	if (pProgram->parsing.memory.pvMem == NULL)
	{
		printf("alloc_memory failed for pProgram->parsing in parse_toml");
		return 12;
	}

	char buf[256] = { '\0' };
	toml_table_t* toml_table = toml_parse_file(pProgram->tomlFile.pFile, buf,
		sizeof(buf) / sizeof(buf[0]));
	if (toml_table == NULL)
	{
		printf("toml_parse_file failed in parse_toml\n");
		return 13;
    }

    int32_t ret = 0;

    // init key is ""
	Container* pContainer = read_table(pProgram, toml_table, NULL, "");
	if (pContainer == NULL)
	{
		printf("read_table failed in parse_toml\n");
		ret = 14;
	}

	toml_free(toml_table);

	if (ret == 0)
	{
		ret = summarize(pProgram, TARGET_CONTENT_ONLY);
	}

	return ret;
}

void print_all(Program* pProgram, enum Target target)
{
	if (target == TARGET_NONE) { return; }

	for (parsing_record pvCurrRec = pProgram->parsing.memory.pvMem;
		pvCurrRec < pProgram->parsing.pvCurrMem; )
	{
		IdParentChild* pIdParentChild = (IdParentChild*)pvCurrRec;

		bool print = !((target == TARGET_CONTENT_ONLY) && (pIdParentChild->_id <= CONTAINER_END));
		//printf("print_all id:%d print:%d\n", pIdParentChild->_id, print);
		if (print) { print_record(pvCurrRec); }
		StructInfo si = get_struct_info_by_id(pIdParentChild->_id);
		pvCurrRec += si.size;
	}
}

Find find_record(Program* pProgram, const char* tomlKey, enum Target target)
{
	Find find = { .capacity = 0, .count = 0, .ppvMatches = NULL };

	if (target == TARGET_NONE) { return find; }

	for (parsing_record pvCurrRec = pProgram->parsing.memory.pvMem; pvCurrRec < pProgram->parsing.pvCurrMem; )
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

			if (target == TARGET_CONTENT_ONLY)
			{
				// container is skipped
				if ((pIdParentChild->_id >= CONTAINER_START) && (pIdParentChild->_id <= CONTAINER_END))
				{
					//get_full_name((Container*)pIdParentChild, fullName);
					//printf("skipped 2 fullName:%s\n", fullName);
					break;
				}
			}

			get_full_name(pIdParentChild->_parent, fullName);
			if (strstr(fullName, tomlKey) != fullName)
			{
				//printf("skipped 3 fullName:%s\n", fullName);
				break;
			}

			if (find.count == find.capacity)
			{
				int32_t capacity = find.capacity == 0 ? INIT_FIND_CAPACITY : find.capacity * 2;
				parsing_record* ppvMatches = malloc(capacity * sizeof(parsing_record));
				if (ppvMatches == NULL)
				{
					printf("malloc Find.ppvMatches failed in find_record\n");
					break;
				}
				find.capacity = capacity;
				memcpy(ppvMatches, find.ppvMatches, find.count * sizeof(parsing_record));
				if (find.ppvMatches != NULL) { free(find.ppvMatches); }
				find.ppvMatches = ppvMatches;
			}
			find.ppvMatches[find.count++] = pvCurrRec;
		}
		while (0);

		StructInfo si = get_struct_info_by_id(((IdParentChild*)pvCurrRec)->_id);
		pvCurrRec += si.size;
	}

	return find;
}

void print_find(Find* pFind)
{
	for (int32_t i = 0; i < pFind->count; ++i)
	{
		print_record(pFind->ppvMatches[i]);
	}
}

FullNameRecord* get_summary_next(Program* pProgram,
	FullNameRecord* pvCurrRec, const char* fullName)
{
	//printf("get_summary_next 1 fullName:%s\n", fullName);

	for (; pvCurrRec < (FullNameRecord*)pProgram->summary.pvCurrMem;)
	{
		if (pvCurrRec != NULL)
		{
			++pvCurrRec;
		}
		else
		{
			pvCurrRec = pProgram->summary.memory.pvMem;
		}

		if (fullName == NULL) { return pvCurrRec; }

		int c = strcmp(pvCurrRec->fullName, fullName);
		//printf("get_summary_next 2 pvCurrRec->fullName:%s c:%d\n", pvCurrRec->fullName, c);
		if (c == 0) { return pvCurrRec; }
		if (c > 0) { break; }
	}

	//printf("get_summary_next 3\n");

	return NULL;
}

Program program_init()
{
    Program program =
    {
    };

    return program;
}

void program_uninit(Program* pProgram)
{
	if (pProgram->summary.memory.pvMem != NULL)
	{
		free(pProgram->summary.memory.pvMem);
		pProgram->summary.memory.pvMem = NULL;
	}
	if (pProgram->parsing.memory.pvMem != NULL)
	{
		free(pProgram->parsing.memory.pvMem);
		pProgram->parsing.memory.pvMem = NULL;
	}
	if (pProgram->tomlFile.pFile != NULL)
	{
		fclose(pProgram->tomlFile.pFile);
		pProgram->tomlFile.pFile = NULL;
	}
}

parsing_handle script_init()
{
	Program* pProgram = malloc(sizeof(Program));
	*pProgram = program_init();

	return (parsing_handle)pProgram;
}

int32_t script_uninit(parsing_handle h)
{
	Program* pProgram = (Program*)h;
	if (pProgram == NULL) { return -1; }

	program_uninit(pProgram);
	free(pProgram);

	return 0;
}

int32_t script_get_full_name(parsing_record pCurr, char* fullName)
{
	if (fullName == NULL) { return -1; }

	get_full_name(pCurr, fullName);

	return (fullName[0] != '\0') ? 0 : -2;
}

int32_t script_parsing(parsing_handle h, const char* tomlFile)
{
	Program* pProgram = (Program*)h;
	if (pProgram == NULL) { return -1; }

	int32_t ret = parse_toml(pProgram, tomlFile);
	if (ret != 0) { return ret; }

	return 0;
}

Find script_find_record(parsing_handle h, const char* tomlKey)
{
	Program* pProgram = (Program*)h;
	if (pProgram == NULL)
	{
		// -1, error case
		return (Find){ .capacity = 0, .count = -1, .ppvMatches = NULL };
	}

	return find_record(pProgram, tomlKey, TARGET_CONTENT_ONLY);
}

void script_print_record(parsing_record pvCurrRec)
{
	print_record(pvCurrRec);
}

void script_print_find(Find* pFind)
{
	return print_find(pFind);
}

int32_t script_release_find(Find* pFind)
{
	if (pFind == NULL) { return -1; }

	if (pFind->ppvMatches == NULL) { return 0; }

	free(pFind->ppvMatches);
	pFind->capacity = 0;
	pFind->count = 0;
	pFind->ppvMatches = NULL;

	return 0;
}

parsing_record script_get_next(parsing_handle h, parsing_record pvCurrRec)
{
	Program* pProgram = (Program*)h;
	if (pProgram == NULL) { return NULL; }

	return get_next(pProgram, pvCurrRec, TARGET_CONTENT_ONLY);
}

void script_print_all(parsing_handle h)
{
	Program* pProgram = (Program*)h;
	if (pProgram == NULL) { return; }

	return print_all(pProgram, TARGET_CONTENT_ONLY);
}

summary_record script_get_summary_next(parsing_handle h,
	summary_record pvCurrRec, const char* fullName)
{
	Program* pProgram = (Program*)h;
	if (pProgram == NULL) { return NULL; }

	return get_summary_next(pProgram, pvCurrRec, fullName);
}
