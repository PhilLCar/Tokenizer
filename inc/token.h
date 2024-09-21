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

OBJECT (const char*) INHERIT (String)
  int line;
  int position;
  int group;
END_OBJECT;

#undef TYPENAME
#endif