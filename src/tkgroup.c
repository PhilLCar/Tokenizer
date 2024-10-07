#include <tkgroup.h>

#define TYPENAME TokenizerGroup

////////////////////////////////////////////////////////////////////////////////
TokenizerGroup *_(cons)(int id, const TokenizerContext *context)
{
  if (this) {
    this->id        = id;
    this->context   = context;
    this->intervals = NEW (Array) (sizeof(Interval));
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  DELETE (this->intervals);
}
