#include <tkgroup.h>

#define TYPENAME TokenizerGroup

////////////////////////////////////////////////////////////////////////////////
TokenizerGroup *_(Construct)(const String *name)
{
  if (String_Construct(BASE(0), name->base)) {
    this->context = NULL;
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this)
  {
    String_Destruct(BASE(0));
    DELETE (this->context);
  }
}

#undef TYPENAME
