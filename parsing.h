#ifndef parsing_h
#define parsing_h

#include <stdint.h>

#define GET_FULL_NAME_LEN 64

typedef void* parsing_handle;
typedef void* parsing_record;

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
	//char*          item;
} Dialogue;

typedef struct _sFind
{
	int32_t         capacity;
	int32_t         count; // negative for error
	parsing_record* ppvMatches;
} Find;

typedef struct _sFullNameRecord
{
	const char*    fullName;
	int32_t        order; // if fullName equal, order decide the sorting
	parsing_record record;
} FullNameRecord;

typedef FullNameRecord* summary_record;

parsing_handle script_init();

int32_t script_uninit(parsing_handle);

int32_t script_get_full_name(parsing_record pCurr, char* fullName);

int32_t script_parsing(parsing_handle, const char* tomlFile);

// below functions should be used after script_parsing
Find script_find_record(parsing_handle, const char* tomlKey);

void script_print_record(parsing_record);

void script_print_find(Find*);

int32_t script_release_find(Find*);

// start form script_get_next(, NULL) to get the starting result of parsing.
// return NULL for end or error
parsing_record script_get_next(parsing_handle, parsing_record);

void script_print_all(parsing_handle);

// start form script_get_summary_next(, NULL) to get the starting result of summary.
// return NULL for end or error
// fullName == NULL for nay fullName
summary_record script_get_summary_next(parsing_handle, summary_record, const char* fullName);

#endif