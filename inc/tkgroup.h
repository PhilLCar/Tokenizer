#ifndef TKGROUP_H
#define TKGROUP_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>
#include <objectarray.h>

#include "tkcontext.h"

#define TYPENAME TokenizerGroup

OBJECT (const String *name) INHERIT (String)
  TokenizerContext *context;
END_OBJECT(NEW (String)(""));

#undef TYPENAME
#endif