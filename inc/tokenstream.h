#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

// CUT
#include <token.h>
#include <tokenizer.h>
#include <trackedstream.h>
#include <array.h>
#include <oop.h>

#define TYPENAME TokenStream

OBJECT (CharStream *stream, const Tokenizer *tokenizer) INHERIT (Stream)
  TrackedStream   *ts;
  const Tokenizer *tokenizer;
  ObjectArray     *next;
END(NULL, NULL);

void  _(close)()            VIRTUAL (close);
void *_(peek) ()            VIRTUAL (peek);
void *_(get)  ()            VIRTUAL (get);
void  _(unget)(void *token) VIRTUAL (unget);
void  _(put)  (void *token) VIRTUAL (put);

#undef TYPENAME
#endif