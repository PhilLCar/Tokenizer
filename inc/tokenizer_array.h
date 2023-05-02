#ifndef TOKENIZER_ARRAY_H
#define TOKENIZER_ARRAY_H

// CUT
#include <array.h>
#include <oop.h>
#include <str.h>

#define TYPENAME TokenizerArray

typedef union tokenizer_element {
	String as_string;
	char   as_char;
} TokenizerElement;

// The flags for a specific list
typedef enum tokenizer_flag {
  TOKENIZER_FLAG_NONE   = 0,
  TOKENIZER_FLAG_MULTI  = 1,
  TOKENIZER_FLAG_PAIR   = TOKENIZER_FLAG_MULTI << 1,
  TOKENIZER_FLAG_BREAK  = TOKENIZER_FLAG_PAIR  << 1,
  TOKENIZER_FLAG_IGNORE = TOKENIZER_FLAG_BREAK << 1
} TokenizerFlag;

OBJECT () INHERIT (Array)
  TokenizerFlag flags;
END_OBJECT;

#undef TYPENAME
#endif