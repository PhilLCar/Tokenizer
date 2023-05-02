#include <tokenizer_array.h>

#define TYPENAME TokenizerArray

////////////////////////////////////////////////////////////////////////////////
TokenizerArray *_(cons)()
{
  if (_this) {
    Array_cons((Array*)_this, sizeof(TokenizerElement));
    _this->flags = TOKENIZER_FLAG_NONE;
  }
  
  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  Array_free((Array*)_this);
}

