#include <tracked_stream.h>

#define TYPENAME TrackedStream

////////////////////////////////////////////////////////////////////////////////
TrackedStream *_(cons)(Stream *base)
{
  if (_this && base) {
    _this->base = *base;
    free(base);
  }
  
  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  Array_free(&_this->linestack);
  String_free(&_this->buffer);
  Stream_close(&_this->base);
}

////////////////////////////////////////////////////////////////////////////////
TrackedStream *STATIC(open)(Stream *stream, int lookahead)
{
  TrackedStream *ts = NEW (TrackedStream) (stream);
  
  if (ts) {
    ts->base.getc   = TrackedStream_getc;
    ts->base.ungetc = TrackedStream_ungetc;
    ts->base.peek   = TrackedStream_peek;
    ts->base.eos    = 0;

    String_cons(&ts->buffer, "");
    Array_cons(&ts->linestack, sizeof(int));

    ts->line      = 0;
    ts->position  = 0;

    for (int i = 0; i < lookahead; i++) String_append(&ts->buffer, Stream_getc(&ts->base));
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
  _this->buffer.base[_this->buffer.length - 1] = Stream_getc(&_this->base);

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
  Stream_ungetc(&_this->base, _this->buffer.base[_this->buffer.length - 1]);
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
