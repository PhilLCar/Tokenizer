#ifndef TRACKEDSTREAM_H
#define TRACKEDSTREAM_H

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <array.h>
#include <diagnostic.h>
#include <charstream.h>
#include <str.h>

#define TYPENAME TrackedStream

OBJECT (CharStream *stream, int lookahead) INHERIT (CharStream)
  String buffer;
  Array  linestack;
  int    line;
  int    position;
END(NULL, 0);

void  _(close)()            VIRTUAL (close);
void *_(get)  ()            VIRTUAL (get);
void  _(unget)(void *token) VIRTUAL (unget);
void  _(put)  (void *token) VIRTUAL (put);


int _(peek)(int distance);

__attribute__((unused))
static int (*tspeek)(CharStream *, int) = (void*)TrackedStream_peek;

__attribute__((unused))
static void *_tspeek(CharStream *stream) {
  return (void*)(long)tspeek(stream, 1);
}

FOREIGN_VIRTUAL(peek, _tspeek);

#undef TYPENAME
#endif
