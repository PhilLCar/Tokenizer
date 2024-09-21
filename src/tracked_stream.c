#include <tracked_stream.h>

#define TYPENAME TrackedStream

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(cons)(Stream *base)
{
  if (_this && base) {
    Stream_cons((Stream*)_this, base, STRINGIZE(TYPENAME));

    String_cons(&_this->buffer, "");
    Array_cons(&_this->linestack, sizeof(int));
  }
  
  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  Array_free(&_this->linestack);
  String_free(&_this->buffer);

  sclose(_this->base.base);
}

////////////////////////////////////////////////////////////////////////////////
TrackedStream *STATIC(open)(Stream *stream, int lookahead)
{
  TrackedStream *ts = NEW (TrackedStream) (stream);
  
  if (ts) {

    ts->line      = 0;
    ts->position  = 0;

    for (int i = 0; i < lookahead; i++) String_append(&ts->buffer, sgetc(ts->base.base));
  } else {
    ts = NULL;
  }

  return ts;
}

////////////////////////////////////////////////////////////////////////////////
void _(close)()
{
  DELETE (_this);
}

////////////////////////////////////////////////////////////////////////////////
int _(getc)()
{
  char c = _this->buffer.base[0];

  for (int i = 0; i < _this->buffer.length - 1; i++) _this->buffer.base[i] = _this->buffer.base[i + 1];
  _this->buffer.base[_this->buffer.length - 1] = sgetc(_this->base.base);

  if (c == '\n') {
    ++_this->line;
    Array_push(&_this->linestack, _this->position);
    _this->position = 0;
  } else ++_this->position;
  
  _this->base.eos = c == EOF;

  return c;
}

////////////////////////////////////////////////////////////////////////////////
void _(ungetc)(int c)
{
  sungetc(_this->base.base, _this->buffer.base[_this->buffer.length - 1]);
  for (int i = _this->buffer.length - 1; i > 0; i--) _this->buffer.base[i] = _this->buffer.base[i - 1];
  
  if (c == '\n') {
    --_this->line;
    _this->position = *(int*)pop(_this->linestack);
  } else --_this->position;
  
  _this->buffer.base[0] = c;
}

////////////////////////////////////////////////////////////////////////////////
int _(peek)(int distance)
{
  int peek;

  if (distance < _this->buffer.length) peek = _this->buffer.base[distance];
  else {
    String *s = NEW (String) ("");
    
    for (int i = distance - _this->buffer.length; i >= 0; i--) String_append(s, TrackedStream_getc(_this));
    peek = _this->buffer.base[distance - _this->buffer.length];
    for (int i = distance - _this->buffer.length; i >= 0; i--) TrackedStream_ungetc(_this, s->base[i]);
    
    DELETE (s);
  }

  return peek;
}
