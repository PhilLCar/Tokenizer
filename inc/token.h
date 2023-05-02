#ifndef TOKEN_H
#define TOKEN_H

// C
#include <stdlib.h>
#include <string.h>

// CUT
#include <array.h>
#include <diagnostic.h>
#include <oop.h>
#include <tokenizer.h>
#include <tracked_stream.h>

#define TYPENAME Token


#define TOKEN_CAT_NEWLINE  -1
#define TOKEN_CAT_VARIABLE -2
#define TOKEN_CAT_NUMBER   -3
#define TOKEN_CAT_EOS      -4
#define TOKEN_CAT_VOID     -5

OBJECT (const char*) INHERIT (String)
  int                                       line;
  int                                       position;
  int                                       cat;
  PAIR(TokenizerElement, TokenizerElement) *delimiters;
END_OBJECT;

#undef TYPENAME
#endif