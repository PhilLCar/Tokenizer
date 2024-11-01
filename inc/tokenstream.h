#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

// CUT
#include <diagnostic.h>
#include <jsonfile.h>
#include <filestream.h>
#include <map.h>
#include <array.h>
#include <oop.h>
#include <token.h>
#include <tokenizer.h>
#include <trackedstream.h>

#define TYPENAME TokenStream

OBJECT (const Tokenizer *tokenizer, CharStream *stream) INHERIT (Stream)
  const Tokenizer *tokenizer;
  ObjectArray     *next;
END_OBJECT(NULL, NULL);

void   _(Close)()             VIRTUAL (Close);
Token *_(Peek) ()             VIRTUAL (Peek);
Token *_(Get)  ()             VIRTUAL (Get);
void   _(Unget)(Token *token) VIRTUAL (Unget);
void   _(Put)  (Token *token) VIRTUAL (Put);

TokenStream *STATIC (Open) (const Tokenizer *tokenizer, const char *filename);

#undef TYPENAME
#endif