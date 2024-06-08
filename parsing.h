#ifndef parsing_h
#define parsing_h

#include <stdint.h>

#define MAX_FOUND 16

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

typedef void* parsing_handle;

parsing_handle script_init();

int32_t script_parsing(parsing_handle, const char* tomlFile);

Found script_find_record(parsing_handle, const char* tomlKey);

void script_print_found(parsing_handle, Found*);

void script_print_all(parsing_handle);

int32_t script_uninit(parsing_handle);

#endif