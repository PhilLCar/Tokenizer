#ifndef TKCONTEXT_H
#define TKCONTEXT_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>
#include <map.h>

#define TYPENAME TokenizerContext

OBJECT (int id, const Map *map)
  int     id;
  char    escape;
  String *open;
  String *close;
END(0, NULL);

#undef TYPENAME
#endif
