#ifndef TKREGEX_H
#define TKREGEX_H

// C
#include <regex.h>

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>
#include <map.h>

#define TYPENAME TokenizerRegex

OBJECT (int id, const Map *map)
  int     id;
  String *name;
  regex_t init;
  Map    *post;
END(0, NULL);

#undef TYPENAME
#endif