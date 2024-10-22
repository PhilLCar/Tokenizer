#include <tokenstream.h>

#define TYPENAME TokenStream

////////////////////////////////////////////////////////////////////////////////
TokenStream *_(cons)(CharStream *stream, Map *config)
{
  if (this) {
    this->tokenizer = NEW (Tokenizer)     (config);
		this->ts        = NEW (TrackedStream) (stream, this->tokenizer->lookahead);
    this->next      = NEW (ObjectArray)   (OBJECT_TYPE(Token));
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
  DELETE (this->ts);
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
  Token *token = NULL;

  if (this->next->base.size > 0) {
    token = ObjectArray_pop(this->next, 1);
  } else {
    int group = -1;

    token = NEW (Token) ("");

    while (!((Stream*)this->ts)->eos) {
      int new = !((String*)token)->length;

      if (Tokenizer_whitespace(this->tokenizer, &this->ts->buffer) >= 0) {
        CharStream_get((CharStream*)this->ts);
        if (new) continue;
        else     break;
      } else {
        int size = 0;

        if ((group = Tokenizer_symbol(this->tokenizer, &this->ts->buffer, &size)) >= 0) {
          Token *next;

          if (new) {
            next = token;
          } else {
            TokenStream_unget(this, NEW (Token)(""));
            next = TokenStream_peek(this);
          }

          next->line     = this->ts->line;
          next->position = this->ts->position;
          next->group    = group;
          TrackedStream_skip(this->ts, size);

          {
            TokenizerGroup *tkgroup = Array_at((Array*)this->tokenizer->groups, group);

            if (tkgroup->context) {
              String_free((String*)next);
              String_cons((String*)next, "");

              while (!((Stream*)this->ts)->eos && !String_strw(&this->ts->buffer, tkgroup->context->close->base)) {
                String_append((String*)next, CharStream_readwith((CharStream*)this->ts, tkgroup->context->escape));
              }

              TrackedStream_skip(this->ts, tkgroup->context->close->length);
            }
          }
          break;
        } else {
          if (new) {
            token->line     = this->ts->line;
            token->position = this->ts->position;
          }

          String_append((String*)token, CharStream_get((CharStream*)this->ts));
        }
      }
    }
    
    if ((group = Tokenizer_keyword(this->tokenizer, (String*)token)) >= 0 
     || (group = Tokenizer_regex(this->tokenizer, (String*)token)) >= 0) {
      token->group = group;
    }
  }

  return token;
}

////////////////////////////////////////////////////////////////////////////////
void _(unget)(Token *token)
{
  ObjectArray_push(this->next, token);
}

////////////////////////////////////////////////////////////////////////////////
void _(put)(Token *token)
{
  { // Attempt to the put the token back on stream exactly where it was
    while (this->ts->line < token->line) {
      CharStream_put((CharStream*)this->ts, '\n');
    }

    while (this->ts->position < token->position) {
      CharStream_put((CharStream*)this->ts, ' ');
    }
  }

  for (int i = 0, c; (c = token->base.base[i]); i++) {
    CharStream_put((CharStream*)this->ts, c);
  }
}

TokenStream *STATIC (open)(const char *filename, const char *config)
{
  return NEW (TokenStream)(
    (CharStream*) NEW (FileStream) (fopen(filename, "r")), 
    (Map*)        NEW (JSONFile)   (config));
}