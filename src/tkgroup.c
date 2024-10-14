#include <tkgroup.h>

#define TYPENAME TokenizerGroup

////////////////////////////////////////////////////////////////////////////////
TokenizerGroup *_(cons)(const String *name)
{
  if (this) {
    this->name    = NEW (String) (name->base);
    this->context = NULL;
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  DELETE (this->name)
  DELETE (this->context);
}
