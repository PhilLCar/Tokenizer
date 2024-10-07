#ifndef TKGROUP_H
#define TKGROUP_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>
#include <objectarray.h>
#include <tkcontext.h>

#define TYPENAME TokenizerGroup

typedef struct {
  int start; // inclusive
  int end;   // exclusive
} Interval;

OBJECT (int id, const TokenizerContext *context)
  int                     id;
  const TokenizerContext *context;
  Array                  *intervals;
END(0, NULL);

#undef TYPENAME
#endif