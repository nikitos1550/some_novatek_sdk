#ifndef _JSONPARSER_H_
#define _JSONPARSER_H_


#include <stddef.h>


#define JSON_PARENT_LINKS ENABLE
/**
 * JSON type identifier. Basic types are:
 * 	o Object
 * 	o Array
 * 	o String
 * 	o Other primitive: number, boolean (true/false) or null
 */
typedef enum {
	JSON_UNDEFINED = 0,
	JSON_OBJECT = 1,
	JSON_ARRAY = 2,
	JSON_STRING = 3,
	JSON_PRIMITIVE = 4
} jsontype_t;

enum jsonerr {
	/* Not enough tokens were provided */
	JSON_ERROR_NOMEM = -1,
	/* Invalid character inside JSON string */
	JSON_ERROR_INVAL = -2,
	/* The string is not a full JSON packet, more bytes expected */
	JSON_ERROR_PART = -3
};

/**
 * JSON token description.
 * type		type (object, array, string etc.)
 * start	start position in JSON data string
 * end		end position in JSON data string
 */
typedef struct {
	jsontype_t type;
	int start;
	int end;
	int size;
#ifdef JSON_PARENT_LINKS
	int parent;
#endif
} jsontok_t;

/**
 * JSON parser. Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string
 */
typedef struct {
	unsigned int pos; /* offset in the JSON string */
	unsigned int toknext; /* next token to allocate */
	int toksuper; /* superior token node, e.g parent object or array */
} json_parser;

/**
 * Create JSON parser over an array of tokens
 */
void json_init(json_parser *parser);

/**
 * Run JSON parser. It parses a JSON data string into and array of tokens, each describing
 * a single JSON object.
 */
int json_parse(json_parser *parser, const char *js, size_t len,
		jsontok_t *tokens, unsigned int num_tokens);



#endif //_JSONPARSER_H_
