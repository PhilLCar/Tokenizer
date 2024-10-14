#include <token.h>

#define TYPENAME Token

////////////////////////////////////////////////////////////////////////////////
Token *_(cons)(const char *text) {
  if (this) {
    String_cons((String*)this, text);
    this->line       = 0;
    this->position   = 0;
    this->group      = 0;
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)() {
  String_free(BASE(0));
}