#include <token.h>

#define TYPENAME Token

////////////////////////////////////////////////////////////////////////////////
Token *_(Construct)(const char *text) {
  if (String_Construct((String*)this, text)) {
    this->line       = 0;
    this->position   = 0;
    this->group      = 0;
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  String_Destruct(BASE(0));
}