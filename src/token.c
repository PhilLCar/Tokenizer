#include <token.h>

#define TYPENAME Token

////////////////////////////////////////////////////////////////////////////////
Token *_(cons)(const char *text) {
  if (_this) {
    String_cons((String*)_this, text);
    _this->line       = 0;
    _this->position   = 0;
    _this->cat        = TOKEN_CAT_VOID;
    _this->delimiters = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(free)() {
  String_free(_this);
}