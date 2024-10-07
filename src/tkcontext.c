#include <tkcontext.h>

#define TYPENAME TokenizerContext

////////////////////////////////////////////////////////////////////////////////
TokenizerContext *_(cons)(int id, const Map *map)
{
  if (this) {
    String *escape = Map_vatkey((Map*)map, "escape");
    String *open   = Map_vatkey((Map*)map, "open");
    String *close  = Map_vatkey((Map*)map, "close");

    if (open) {
      this->id     = id;
      this->escape = escape ? escape->base[0] : 0;
      this->open   = NEW (String) (open->base);
      this->close  = NEW (String) (close ? close->base : "\n");
    } else {
      fprintf(stderr, "Can't infer the opening symbol of a given context.");
      this = NULL;
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  DELETE (this->open);
  DELETE (this->close);
}
