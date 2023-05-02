#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

// CUT
#include <token.h>
#include <tokenizer.h>
#include <tracked_stream.h>
#include <array.h>
#include <oop.h>

#define TYPENAME TokenStream

OBJECT (TrackedStream*, Tokenizer*)
  TrackedStream *ts;
  Tokenizer     *tokenizer;
  Array         *next;
END_OBJECT;

// RETURNS a token stream from the generic <stream> using a <tokenizer>
TokenStream *STATIC(open)(Stream *stream, Tokenizer *tokenizer) ALIAS (tkopen);

// Closes the token stream <tk>
void         _(close)() ALIAS (tkclose);

// RETURNS the next token on stream <tk>
Token       *_(get)() ALIAS (tkget);

// Ungets the <token> on the token stream <tk>
void         _(unget)(Token *token) ALIAS (tkunget);

#undef TYPENAME
#endif