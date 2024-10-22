#include <tokenstream.h>

#define TYPENAME TokenStream

////////////////////////////////////////////////////////////////////////////////
TokenStream *_(cons)(const Tokenizer *tokenizer, CharStream *stream)
{
  if (this) {
    this->tokenizer = (Tokenizer*)tokenizer;
    this->next      = NEW (ObjectArray)   (OBJECT_TYPE(Token));

    Stream_cons(BASE(0), NEW (TrackedStream) (stream, this->tokenizer->lookahead));
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  Stream_free(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
void _(close)()
{
  DELETE (*BASE(1));
  DELETE (this->next);
}

////////////////////////////////////////////////////////////////////////////////
Token *_(peek)()
{
  if (!this->next->base.size) 
  {
    void *token = TokenStream_get(this);

    if (token) TokenStream_unget(this, token);
  }

  return Array_last((Array*)this->next);
}

////////////////////////////////////////////////////////////////////////////////
Token *_(get)()
{
  TrackedStream *ts    = *BASE(1);
  Token         *token = NULL;

  if (this->next->base.size > 0) {
    token = ObjectArray_pop(this->next, 1);
  } else {
    int group = -1;

    token = NEW (Token) ("");

    while (!((Stream*)ts)->eos) {
      int new = !((String*)token)->length;

      if (Tokenizer_whitespace(this->tokenizer, &ts->buffer) >= 0) {
        CharStream_get((CharStream*)ts);
        if (new) continue;
        else     break;
      } else {
        int size = 0;

        if ((group = Tokenizer_symbol(this->tokenizer, &ts->buffer, &size)) >= 0) {
          Token *next;

          if (new) {
            next = token;
          } else {
            TokenStream_unget(this, NEW (Token)(""));
            next = TokenStream_peek(this);
          }

          next->line     = ts->line;
          next->position = ts->position;
          next->group    = group;
          
          for (int i = 0; i < size; i++) {
            String_append((String*)next, TrackedStream_get(ts));
          }

          {
            TokenizerGroup *tkgroup = Array_at((Array*)this->tokenizer->groups, group);

            if (tkgroup->context) {
              String_free((String*)next);
              String_cons((String*)next, "");

              while (!BASE(0)->eos && !String_strw(&ts->buffer, tkgroup->context->close->base)) {
                String_append((String*)next, CharStream_readwith((CharStream*)ts, tkgroup->context->escape));
              }

              TrackedStream_skip(ts, tkgroup->context->close->length);
            }
          }
          break;
        } else {
          if (new) {
            token->line     = ts->line;
            token->position = ts->position;
          }

          String_append((String*)token, CharStream_get((CharStream*)ts));
        }
      }
    }
    
    if ((group = Tokenizer_keyword(this->tokenizer, (String*)token)) >= 0 
     || (group = Tokenizer_regex  (this->tokenizer, (String*)token)) >= 0) {
      token->group = group;
    }
  }
  
  if (String_eq((String*)token, "")) {
    DELETE (token);
    BASE(0)->eos = 1;
  }

  return token;
}

////////////////////////////////////////////////////////////////////////////////
void _(unget)(Token *token)
{
  ObjectArray_push(this->next, token);
  BASE(0)->eos = 0;
}

////////////////////////////////////////////////////////////////////////////////
void _(put)(Token *token)
{
  TrackedStream *ts = *BASE(1);

  { // Attempt to the put the token back on stream exactly where it was
    while (ts->line < token->line) {
      CharStream_put((CharStream*)ts, '\n');
    }

    while (ts->position < token->position) {
      CharStream_put((CharStream*)ts, ' ');
    }
  }

  for (int i = 0, c; (c = token->base.base[i]); i++) {
    CharStream_put((CharStream*)ts, c);
  }
}

TokenStream *STATIC (open)(const Tokenizer *tokenizer, const char *filename)
{
  return NEW (TokenStream)(tokenizer, (CharStream*) NEW (FileStream) (fopen(filename, "r")));
}