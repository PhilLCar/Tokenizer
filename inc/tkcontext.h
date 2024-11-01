#ifndef TKCONTEXT_H
#define TKCONTEXT_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>
#include <map.h>
#include <exception.h>

#define TYPENAME TokenizerContext

OBJECT (const Map *map) NOBASE
  char    escape;
  String *open;
  String *close;
END_OBJECT(NULL);

#undef TYPENAME
#endif
