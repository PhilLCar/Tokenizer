#include <trackedstream.h>

#define TYPENAME TrackedStream

////////////////////////////////////////////////////////////////////////////////
TrackedStream *_(cons)(CharStream *stream, int lookahead)
{
  CharStream_cons(BASE(0), stream);

  if (this) {
    String_cons(&this->buffer, "");
    Array_cons(&this->linestack, sizeof(int));

    this->line      = 0;
    this->position  = 0;

    for (int i = 0; i < lookahead; i++) {
      String_append(&this->buffer, CharStream_get(stream));
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  CharStream_free(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
void _(close)()
{
  String_free(&this->buffer);
  Array_free(&this->linestack);
  DELETE (BASE(1)->base);
}

////////////////////////////////////////////////////////////////////////////////
void *_(get)()
{
  long c = this->buffer.base[0];

  for (int i = 0; i < this->buffer.length - 1; i++) this->buffer.base[i] = this->buffer.base[i + 1];
  this->buffer.base[this->buffer.length - 1] = CharStream_get(BASE(1)->base);

  if (c == '\n') {
    ++this->line;
    Array_push(&this->linestack, &this->position);
    this->position = 0;
  } else ++this->position;

  return (void*)c;
}

////////////////////////////////////////////////////////////////////////////////
void _(unget)(void *token)
{
  char c = (long)token;

  CharStream_unget(BASE(1)->base, this->buffer.base[this->buffer.length - 1]);
  for (int i = this->buffer.length - 1; i > 0; i--) this->buffer.base[i] = this->buffer.base[i - 1];
  
  if (c == '\n') {
    --this->line;
    this->position = *(int*)Array_pop(&this->linestack);
  } else --this->position;
  
  this->buffer.base[0] = c;
}

////////////////////////////////////////////////////////////////////////////////
void _(put)(void *token)
{
  char c = (long)token;

  CharStream_put(BASE(1)->base, c);
  if (c == '\n') {
    ++this->line;
    this->position = 0;
  } else ++this->position;
}

////////////////////////////////////////////////////////////////////////////////
void _(skip)(int distance)
{
  for (int i = 0; i < distance; i++) {
    TrackedStream_get(this);
  }
}

////////////////////////////////////////////////////////////////////////////////
int _(peek)(int distance)
{
  int peek;

  if (distance < this->buffer.length) peek = this->buffer.base[distance];
  else {
    String *s = NEW (String) ("");
    
    for (int i = distance - this->buffer.length; i >= 0; i--) String_append(s, CharStream_get(BASE(1)->base));
    peek = this->buffer.base[distance - this->buffer.length];
    for (int i = distance - this->buffer.length; i >= 0; i--) CharStream_unget(BASE(1)->base, s->base[i]);
    
    DELETE (s);
  }

  return peek;
}
