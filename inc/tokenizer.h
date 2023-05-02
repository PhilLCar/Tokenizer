#ifndef TOKENIZER_H
#define TOKENIZER_H

// C
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// CUT
#include <array.h>
#include <tokenizer_array.h>
#include <diagnostic.h>
#include <oop.h>
#include <stream.h>
#include <pair.h>
#include <str.h>
#include <filestream.h>

#define TYPENAME Tokenizer

// The lists of symbols to be parsed
typedef enum tokenizer_category {
  TOKENIZER_WHITESPACE,  // Separating characters (for instance add '\t' and '\r' to ignore)
  TOKENIZER_CHARSEQ,     // PAIR: Where text is lifted verbatim (except for escapes)
  TOKENIZER_COMMENT,     // PAIR: Where format is completely ignored
  TOKENIZER_PUNCTUATION, // On which the parser breaks no matter what
  TOKENIZER_RESERVED,    // Reserved by language
  TOKENIZER_NCAT         // Size of enum
} TokenizerCategory;

OBJECT ()
  TokenizerArray tokens[TOKENIZER_NCAT];
  char           escape;
  int            lookahead;
END_OBJECT;

// First: Header (in the PRS), Second: flags
extern const PAIR(const char*, TokenizerFlag) TOKENIZER_FLAGS[TOKENIZER_NCAT];

// Adds the <tokens> to the <tokenizer>'s list specified by <listID>
void _(add_array)(TokenizerCategory tcat, Array *tokens);

// Loads the specified <filename> into the <tokenizer>
void _(load_file)(const char *filename);

#undef TYPENAME
#endif
