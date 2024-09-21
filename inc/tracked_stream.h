#ifndef TRACKED_STREAM_H
#define TRACKED_STREAM_H

// C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <array.h>
#include <diagnostic.h>
#include <stream.h>
#include <str.h>

#define TYPENAME TrackedStream

OBJECT (Stream*) INHERIT (Stream)
  String buffer;
  Array  linestack;
  int    line;
  int    position;
END_OBJECT;

// RETURNS a new tracked stream from a <stream> with <lookahead> characters of buffer
TrackedStream *STATIC(open)(Stream *stream, int lookahead);

// Closes the tracked stream
void _(close)() VIRTUAL (close);

// RETURNS a character from the tracked stream <ts>
int _(getc)() VIRTUAL (getc);

// Puts back a character <c> on the tracked stream <ts>
void _(ungetc)(int c) VIRTUAL (ungetc);

// RETURNS the character <distance> places ahead, without altering the stream <ts>
int _(peek)(int distance);

__attribute__((unused))
static int (*tspeek)(Stream *, int) = (void*)TrackedStream_peek;

__attribute__((unused))
static int tspeek1(Stream *s) {
  return tspeek(s, 1);
}

FOREIGN_VIRTUAL(peek, tspeek1);

#undef TYPENAME
#endif
