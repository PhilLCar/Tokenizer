#include <token_stream.h>

#define TYPENAME TokenStream

// Compares the string until one ends
// RETURNS the match length, or 0 if no match
/********************************************************************************/
int _strcmps(char *s1, char *s2)
{
  int i;

  for (i = 0;; i++) {
    if (s1[i] != s2[i]) {
      if (s1[i] && s2[i]) return 0;
      else                break;
    }
    if (!s1[i] || !s2[i]) break;
  }
  return i;
}

// RETURNS a pointer to the <found> string
/********************************************************************************/
void *_in_list(TokenStream *tk, TokenizerCategory tcat)
{
  TokenizerArray *tokens = &tk->tokenizer->tokens[tcat];
  void           *found  = NULL;

  if (tokens->flags & TOKENIZER_FLAG_MULTI) {
    int    best = 0;

    for (int i = 0; i < tokens->base.size; i++) {
      String *str = Array_at(tokens, i);
      int     cmp = strcmps(str->base, tk->ts->buffer);

      if (cmp > best) {
        best  = cmp;
        found = str;
        break;
      }
    }
  } else {
    for (int i = 0; i < tokens->base.size; i++) {
      char *c   = at(tokens, i);

      if (*c == *tk->ts->buffer.base) {
        found = c;
        break;
      }
    }
  }
  
  return found;
}

// RETURNS 1 if the next character on the stream is a newline, 0 otherwise
/********************************************************************************/
int _is_newline(TrackedStream *ts) {
  return tspeek(ts, 0) == '\n';
}

// Records the current position of the stream <ts> into the <symbol>
/********************************************************************************/
void _snap(TrackedStream *ts, Token *token)
{
  token->line     = ts->line;
  token->position = ts->position;
}

////////////////////////////////////////////////////////////////////////////////
TokenStream *_(cons)(TrackedStream *ts, Tokenizer *tokenizer) {
  if (_this) {
    _this->ts        = ts;
    _this->tokenizer = tokenizer;
    _this->next      = NEW (Array) (sizeof(Token));
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)() {
  TrakcedStream_close(_this->ts);
  Tokenizer_free(_this->tokenizer);
  Array_free(_this->next);
}

////////////////////////////////////////////////////////////////////////////////
TokenStream *STATIC(open)(Stream *stream, Tokenizer *tokenizer)
{
  TrackedStream *ts = TrackedStream_open(stream, tokenizer);
  TokenStream   *tk = NEW (TokenStream) (ts, tokenizer);

  return tk;
}

////////////////////////////////////////////////////////////////////////////////
void _(close)()
{
  DELETE (_this);
}

////////////////////////////////////////////////////////////////////////////////
Token *_(get)()
{
  // Returns a symbol if one is on the outstack
  if (_this->next->size) return pop(_this->next);
  else {
    Tokenizer     *tokenizer = _this->tokenizer;
    TrackedStream *ts        = _this->ts;
    Token         *token     = NEW (Token) ("");
    String        *text      = &token->base;
    int            var       = 0;
    int            num       = 0;

    // Empties the whitespaces ahead of next symbol
    while (_in_list(_this, TOKENIZER_WHITESPACE)) tsgetc(ts);

    // BEGIN PARSING LOOP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    do {
      int               length =  0;
      TokenizerElement *match  = NULL;
      char              peek0  = tspeek(ts, 0);
      char              peek1  = tspeek(ts, 1);

      // Check if the next symbol will be a number
      if (!text->length) {
        _snap(ts, token); 
        num = (peek0 >= '0' && peek0 <= '9') || (peek1 >= '0' && peek1 <= '9' && peek0 == '.' );
      }

      // Should be smaller than the FIRST variable type so that they are not tested
      for (TokenizerCategory tcat = 0; tcat < TOKENIZER_NCAT; tcat++) {
        TokenizerArray *tokens = &_this->tokenizer->tokens[tcat];

        if ((tokens->flags & TOKENIZER_FLAG_BREAK) && (~tokens->flags & TOKENIZER_FLAG_IGNORE)) {
          TokenizerElement *elem = _in_list(_this, tcat);

          if (elem) {
            int len = tokens->flags & TOKENIZER_FLAG_MULTI ? elem->as_string.length : 1;

            /* NOTE:
            There is a special case with decimals. '.' has to be taken out of consideration
            when the parsing is in a number context.
            */
            if (len > length && !(num && len == 1 && peek0 == '.')) {
              length     = len;
              match      = elem;
              token->cat = tcat;
            }
          }
        }
      }

      // A break was found on the stream ++++++++++++++++++++++++++++++++++++++++++//
      if (match) {
        if (!text->length) {
          if (TOKENIZER_FLAGS[token->cat].second & TOKENIZER_FLAG_PAIR) {
            PAIR(TokenizerElement, TokenizerElement) *pair = (void*)match;

            token->delimiters = pair;

            for (int i = 0; i < pair->first.as_string.length; i++) tsgetc(ts);
            while (_strcmps(pair->second.as_string.base, ts->buffer.base) != 
                            pair->second.as_string.length && !ts->base.eos)
            {
              append(text, tsgetc(ts));
            }
            for (int i = 0; i < pair->second.as_string.length; i++) tsgetc(ts);
          } else {
            for (int i = 0; i < match->as_string.length; i++) append(text, tsgetc(ts));
          }
        }
        break;
      // A newline was reached on the stream ++++++++++++++++++++++++++++++++++++++//
      } else if (_is_newline(ts)) { // If '\n' is in the whitespace list: unreachable
        if (!text->length) {
          token->cat = TOKEN_CAT_NEWLINE;
        }
        break;
      // EOS was reached on the stream ++++++++++++++++++++++++++++++++++++++++++++//
      } else if (ts->base.eos) {
        if (!text->length) {
          token->cat = TOKEN_CAT_EOS;
        }
        break;
      // The symbol is of the variable/reserved type ++++++++++++++++++++++++++++++//
      } else {
        char c = TrackedStream_getc(ts);

        var = 1;
        if (c == tokenizer->escape) c = Stream_esc(ts);
        String_append(text, c);
      }
    } while (1);
    // END PARSING LOOP >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // If it's a variable type, distinguish further
    if (num) {
      token->cat = TOKEN_CAT_NUMBER;
    } else if (var) {
      token->cat = TOKEN_CAT_VARIABLE;

      // Should be smaller than the FIRST variable type so that they are not tested
      for (TokenizerCategory tcat = 0; tcat < TOKENIZER_NCAT; tcat++) {
        TokenizerArray *tokens = &_this->tokenizer->tokens[tcat];

        if (tokens->flags == TOKENIZER_FLAG_NONE) {
          for (int i = 0; i < tokens->base.size; i++) {
            if (String_equals(Array_at(tokens, i), text->base)) {
              token->cat = tcat;
            }
          }
        }
      }
    }

    return token;
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(unget)(Token *token)
{
  push(_this->next, &token);
}
