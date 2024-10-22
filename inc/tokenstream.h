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

OBJECT (const Tokenizer *tokenizer, CharStream *stream) INHERIT (Stream)
  Tokenizer     *tokenizer;
  ObjectArray   *next;
END(NULL, NULL);

void   _(close)()             VIRTUAL (close);
Token *_(peek) ()             VIRTUAL (peek);
Token *_(get)  ()             VIRTUAL (get);
void   _(unget)(Token *token) VIRTUAL (unget);
void   _(put)  (Token *token) VIRTUAL (put);

TokenStream *STATIC (open) (const Tokenizer *tokenizer, const char *filename);

#undef TYPENAME
#endif