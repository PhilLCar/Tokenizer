#ifndef TOKENIZER_H
#define TOKENIZER_H

// C
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// CUT
#include <tokenizer_group.h>
#include <object_array.h>
#include <diagnostic.h>
#include <oop.h>
#include <stream.h>
#include <pair.h>
#include <str.h>
#include <filestream.h>

#define TYPENAME Tokenizer

// The category of a specific list
typedef enum tokenizer_category {
  TOKENIZER_CATEGORY_ESCAPE          = 'X',
  TOKENIZER_CATEGORY_WHITESPACE      = 'W',
  TOKENIZER_CATEGORY_PUNCTUATION     = 'P',
  TOKENIZER_CATEGORY_EXCERPT         = 'E',
  TOKENIZER_CATEGORY_RESERVED        = 'R',
  TOKENIZER_CATEGORY_LOOKAHEAD_MATCH = 'L',
  TOKENIZER_CATEGORY_POSTPARSE_MATCH = 'M'
} TokenizerCategory;

OBJECT ()
  TokenizerGroup *escape;
  TokenizerGroup *whitespace;
  ObjectArray    *punctuation;
  ObjectArray    *excerpt;
  ObjectArray    *reserved;
  ObjectArray    *regex1;
  ObjectArray    *regex2;
  Array          *groups;
END_OBJECT;

// Returns number of characters read
int _(escape)(const char *lookahead);

// Returns number of characters read
int _(whitespace)(const char *lookahead);

// Returns number of characters read and sets group
int _(symbol)(const char *lookahead, int *group);

// Returns number of characters read and sets group and index
int _(xinit)(const char *lookahead, int *group, int *index);

// Returns number of characters read for given group and index
int _(xesc)(const char *lookahead, int group, int index);

// Returns number of characters read for given group and index
int _(xend)(const char *lookahead, int group, int index);

// Returns the group if match
int _(reserved)(const char *result);

// Returns the number of characters read and sets group
int _(match)(const char *lookahead, int* group);

// Returns the group if match
int _(postmatch)(int group, const char *result);

// Returns group number
int _(group)(const char *name);

TokenizerGroup *_(byname)(const char *name);
TokenizerGroup *_(byid)(int id);

#undef TYPENAME
#endif
