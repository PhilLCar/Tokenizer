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
END(NULL, 0);

void  _(close)()      VIRTUAL (close);
int   _(get)  ()      VIRTUAL (get);
void  _(unget)(int c) VIRTUAL (unget);
void  _(put)  (int c) VIRTUAL (put);

void _(skip)(int distance);

int _(peek)(int distance);

__attribute__((unused))
static int (*tspeek)(TrackedStream*, int) = TrackedStream_peek;

__attribute__((unused))
static int _tspeek(TrackedStream *stream) {
  return TrackedStream_peek(stream, 1);
}

FOREIGN_VIRTUAL(peek, _tspeek);

#undef TYPENAME
#endif
