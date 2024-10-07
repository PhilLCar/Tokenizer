#include <tokenstream.h>

#define TYPENAME TokenStream


////////////////////////////////////////////////////////////////////////////////
TokenStream *_(cons)(CharStream *stream, const Tokenizer *tokenizer)
{
  if (this) {
		this->ts        = NEW (TrackedStream) (stream, tokenizer->lookahead);
    this->tokenizer = tokenizer;
    this->next      = NEW (ObjectArray) (OBJECT_TYPE(Token));
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  Stream_free(this);
}

////////////////////////////////////////////////////////////////////////////////
void _(close)()
{
  DELETE (this->ts);
  DELETE (this->next);
}

////////////////////////////////////////////////////////////////////////////////
void *_(peek)()
{
  void *token = TokenStream_get(this);

  TokenStream_unget(this, token);

  return token;
}

////////////////////////////////////////////////////////////////////////////////
void *_(get)()
{
  if (this->next->base.size > 0) {
    return ObjectArray_pop(this, 1);
  } else {

  }
}

////////////////////////////////////////////////////////////////////////////////
void _(unget)(void *token)
{
  ObjectArray_push(this->next, token);
}

////////////////////////////////////////////////////////////////////////////////
void _(put)(void *token)
{
  Token *t = token;
  char   c;

  for (int i = 0; c = t->base.base[i]; i++) {
    TrackedStream_put(this->ts, (void*)(long)c);
  }

  TrackedStream_put(this->ts, (void*)(long)this->tokenizer->whitespaces->base[0]);
}