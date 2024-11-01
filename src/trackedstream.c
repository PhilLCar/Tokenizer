#include <trackedstream.h>

#define TYPENAME TrackedStream

////////////////////////////////////////////////////////////////////////////////
TrackedStream *_(Construct)(CharStream *stream, int lookahead)
{
  if (CharStream_Construct(BASE(0), stream)) {
    String_Construct(&this->buffer, "");
    Array_Construct(&this->linestack, sizeof(int));

    this->line     = 0;
    this->position = 0;

    for (int i = 0; i < lookahead; i++) {
      String_Append(&this->buffer, CharStream_Get(stream));
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  CharStream_Destruct(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
void _(Close)()
{
  if (this)
  {
    String_Destruct(&this->buffer);
    Array_Destruct(&this->linestack);
    DELETE (BASE(1)->base);
  }
}

////////////////////////////////////////////////////////////////////////////////
int _(Get)()
{
  int c = this->buffer.base[0];

  for (int i = 0; i < this->buffer.length - 1; i++) this->buffer.base[i] = this->buffer.base[i + 1];
  this->buffer.base[this->buffer.length - 1] = CharStream_Get(BASE(1)->base);

  if (((Stream*)BASE(1)->base)->eos) {
    this->buffer.base[this->buffer.length - 1] = 0;
  }

  if (c == '\n') {
    ++this->line;
    Array_Push(&this->linestack, &this->position);
    this->position = 0;
  } else ++this->position;

  BASE(1)->eos = !c;

  return c;
}

////////////////////////////////////////////////////////////////////////////////
void _(Unget)(int c)
{
  CharStream_Unget(BASE(1)->base, this->buffer.base[this->buffer.length - 1]);
  for (int i = this->buffer.length - 1; i > 0; i--) this->buffer.base[i] = this->buffer.base[i - 1];
  
  if (c == '\n') {
    --this->line;
    this->position = *(int*)Array_Pop(&this->linestack);
  } else --this->position;
  
  this->buffer.base[0] = c;

  BASE(1)->eos = 0;
}

////////////////////////////////////////////////////////////////////////////////
void _(Put)(int c)
{
  CharStream_Put(BASE(1)->base, c);
  if (c == '\n') {
    ++this->line;
    this->position = 0;
  } else ++this->position;
}

////////////////////////////////////////////////////////////////////////////////
void _(Skip)(int distance)
{
  for (int i = 0; i < distance; i++) {
    TrackedStream_Get(this);
  }
}

////////////////////////////////////////////////////////////////////////////////
int _(Peek)(int distance)
{
  int peek;

  if (distance < this->buffer.length) peek = this->buffer.base[distance];
  else {
    String *s = NEW (String) ("");
    
    for (int i = distance - this->buffer.length; i >= 0; i--) String_Append(s, CharStream_Get(BASE(1)->base));
    peek = this->buffer.base[distance - this->buffer.length];
    for (int i = distance - this->buffer.length; i >= 0; i--) CharStream_Unget(BASE(1)->base, s->base[i]);
    
    DELETE (s);
  }

  return peek;
}
