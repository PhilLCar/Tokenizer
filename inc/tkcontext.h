#ifndef TKCONTEXT_H
#define TKCONTEXT_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>
#include <map.h>

#define TYPENAME TokenizerContext

OBJECT (const Map *map)
  char    escape;
  String *open;
  String *close;
END(NULL);

#undef TYPENAME
#endif
