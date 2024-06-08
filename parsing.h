#ifndef parsing_h
#define parsing_h

#include <stdint.h>

#define GET_FULL_NAME_LEN 64
#define MAX_FOUND         16

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
	int32_t count; // negative for error
	void*   pvMatchs[MAX_FOUND];
} Found;

typedef struct _sStructInfo
{
	enum StructId id;
	size_t        size; // the structre size of structure id
	const char*   structName;
} StructInfo;

// include any of the structrues IdParentChild, Container, FinalProject, Scene, Character, Item, Event, Dialogue
// you can traverse it by cast void* to IdParentChild and check its _id
typedef struct _sStructures
{
	void* pvStart;
	void* pvEnd;
} Structures;

typedef void* parsing_handle;

parsing_handle script_init();

int32_t script_uninit(parsing_handle);

StructInfo script_get_struct_info_by_id(enum StructId id);

int32_t script_get_full_name(void* pCurr, char* fullName);

int32_t script_parsing(parsing_handle, const char* tomlFile);

// below functions should be used after script_parsing
Found script_find_record(parsing_handle, const char* tomlKey);

void script_print_found(Found*);

Structures script_get_all(parsing_handle);

void script_print_all(parsing_handle);

#endif