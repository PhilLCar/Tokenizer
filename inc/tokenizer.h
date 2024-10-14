#ifndef TOKENIZER_H
#define TOKENIZER_H

// C
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// CUT
#include <diagnostic.h>
#include <json.h>
#include <set.h>
#include <rgx.h>

#include "tkcontext.h"
#include "tkgroup.h"

#define TYPENAME Tokenizer

OBJECT (Map *config)
  int          lookahead;
  String      *whitespaces;
  ObjectArray *symbols;
  Map         *keywords;
  Map         *regexes;
  ObjectArray *groups;
END(NULL);

int _(group)(const char *name);
int _(whitespace)(String *search);
int _(symbol)(String *search, int *size);
int _(regex)(String *search);
int _(keyword)(String *search);


#undef TYPENAME
#endif
