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

#define TYPENAME Tokenizer

OBJECT (const JSON *config)
  int          lookahead;
  String      *whitespaces;
  ObjectArray *symbols;
  ObjectArray *tags;
  ObjectArray *groups;
  ObjectArray *contexts;
  ObjectArray *regexes;
  Map         *reserved;
END(NULL);



#undef TYPENAME
#endif
