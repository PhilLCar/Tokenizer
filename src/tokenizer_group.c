#include <tokenizer_group.h>

#define TYPENAME TokenizerGroup

// Compares the string until one ends
// RETURNS the match length, or 0 if no match
/********************************************************************************/
int _strcmps(const char *s1, const char *s2)
{
  int i;

  for (i = 0; s1[i] && s2[i]; i++) {
    if (s1[i] != s2[i]) {
      i = 0;
      break;
    }
  }

  return i;
}

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(cons)(const char *tag, int multiple, int dimension)
{
  if (_this) {
    if (multiple) ObjectArray_cons((ObjectArray*)_this, sizeof(String));
    else          Array_cons      ((Array*)      _this, sizeof(char));

    _this->tag       = NEW (String) (tag);
    _this->multiple  = multiple;
    _this->dimension = dimension;
  }
  
  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  DELETE (_this->tag);

  if (_this->multiple) ObjectArray_free((ObjectArray*)_this);
  else                 Array_free((Array*)_this);
}

////////////////////////////////////////////////////////////////////////////////
int _(add)(const char *sequence)
{
  if (_this->multiple) {
    int dim = 0;

    for (int i = 0, start = -1; sequence[i] && dim < _this->dimension; i++) {
      if (sequence[i] != ' ') {
        if (start < 0) start = i;
      } else if (start >= 0) {
        char buffer[256];

        memcpy(buffer, &sequence[start], i - start);
        buffer[i - start] = 0;

        ObjectArray_push((ObjectArray*)_this,  NEW (String) (buffer));
        ++dim;
        start = -1;
      }
    }

    while (++dim < _this->dimension) ObjectArray_push((ObjectArray*)_this, NEW (String) (""));
  } else {
    int  dim  = 0;
    char last = ' ';

    for (int i = 0; (last = sequence[i]) && dim < _this->dimension; i++) {
      if (last != ' ') {
        Array_push((Array*)_this, &sequence[i]);
        ++dim;
      }
    }

    while (dim++ < _this->dimension) Array_push((Array*)_this, &last);
  }
}

////////////////////////////////////////////////////////////////////////////////
int _(indexof)(const char *sequence)
{
  Array *base  = (Array*)_this;
  int    index = -1;

  if (_this->multiple) {
    int max = 0;

    for (int i = 0; i < base->size; i++){
      String *symbol = Array_at(base, i);

      if (_strcmps(symbol->base, sequence) > max) {
        index = i;
      }
    }
  } else {
    char *raw = base->base;
    char  c   = *sequence;

    for (int i = 0; i < base->size; i++) {
      if (c == raw[i]) {
        index = i;
        break;
      }
    }
  }

  return index;
}