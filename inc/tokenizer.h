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
#include <jsonfile.h>
#include <rgx.h>

#include "tkcontext.h"
#include "tkgroup.h"

#define TYPENAME Tokenizer

OBJECT (Map *config) NOBASE
  int          lookahead;
  String      *whitespaces;
  ObjectArray *symbols;
  Map         *keywords;
  Map         *regexes;
  ObjectArray *groups;
END_OBJECT(NULL);

int CONST (Group)(const char *name);
int CONST (WhiteSpace)(const String *search);
int CONST (Symbol)(const String *search, int *size);
int CONST (Regex)(const String *search);
int CONST (Keyword)(const String *search);

Tokenizer *STATIC (Open)(const char *filename);

#undef TYPENAME
#endif
