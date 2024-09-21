#ifndef TOKENIZER_ARRAY_H
#define TOKENIZER_ARRAY_H

// CUT
#include <oop.h>
#include <object_array.h>
#include <str.h>

#define TYPENAME TokenizerGroup

OBJECT (const char*, int, int) INHERIT (ObjectArray)
  String *tag;       // the name of this parameter array
  int     multiple;  // true if multiple characters in any symbol
  int     dimension; // dimension of the array
END_OBJECT;

// Adds a new character or sequence to the array
int _(add)(const char *);

// Returns the index of a character or a sequence of characters
int _(indexof)(const char*);

#undef TYPENAME
#endif