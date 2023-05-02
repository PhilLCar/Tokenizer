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

#define TYPENAME TrackedStream

OBJECT (Stream*) INHERIT (Stream)
  String buffer;
  Array  linestack;
  int    line;
  int    position;
END_OBJECT;

// RETURNS a new tracked stream from a <stream> with <lookahead> characters of buffer
TrackedStream *STATIC(open)(Stream *stream, int lookahead) ALIAS (tsopen);

// Closes the tracked stream
void _(close)() ALIAS (tsclose);

// RETURNS a character from the tracked stream <ts>
int _(getc)() ALIAS (tsgetc);

// Puts back a character <c> on the tracked stream <ts>
void _(ungetc)(int c) ALIAS (tsungetc);

// RETURNS the character <distance> places ahead, without altering the stream <ts>
int _(peek)(int distance) ALIAS (tspeek);

#undef TYPENAME
#endif
