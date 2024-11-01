#ifndef TOKEN_H
#define TOKEN_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>

#define TYPENAME Token

OBJECT (const char*) INHERIT (String)
  int line;
  int position;
  int group;
END_OBJECT("");

#undef TYPENAME
#endif