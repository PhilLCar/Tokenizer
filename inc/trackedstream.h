#ifndef TRACKEDSTREAM_H
#define TRACKEDSTREAM_H

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <diagnostic.h>
#include <array.h>
#include <charstream.h>
#include <str.h>

#define TYPENAME TrackedStream

OBJECT (CharStream *stream, int lookahead) INHERIT (CharStream)
  String buffer;
  Array  linestack;
  int    line;
  int    position;
END_OBJECT(NULL, 0);

void  _(Close)()      VIRTUAL (Close);
int   _(Get)  ()      VIRTUAL (Get);
void  _(Unget)(int c) VIRTUAL (Unget);
void  _(Put)  (int c) VIRTUAL (Put);

void _(Skip)(int distance);

int _(Peek)(int distance);

__attribute__((unused))
static int _tspeek(TrackedStream *stream) {
  return TrackedStream_Peek(stream, 1);
}

FOREIGN_VIRTUAL(Peek, _tspeek);

#undef TYPENAME
#endif
