#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

// CUT
#include <diagnostic.h>
#include <token.h>
#include <tokenizer.h>
#include <jsonfile.h>
#include <filestream.h>
#include <trackedstream.h>
#include <map.h>
#include <array.h>
#include <oop.h>

#define TYPENAME TokenStream

OBJECT (CharStream *stream, Map *config) INHERIT (Stream)
  Tokenizer     *tokenizer;
  TrackedStream *ts;
  ObjectArray   *next;
END(NULL, NULL);

void  _(close)()            VIRTUAL (close);
void *_(peek) ()            VIRTUAL (peek);
void *_(get)  ()            VIRTUAL (get);
void  _(unget)(void *token) VIRTUAL (unget);
void  _(put)  (void *token) VIRTUAL (put);

TokenStream *STATIC (open) (const char *filename, const char *config);

#undef TYPENAME
#endif