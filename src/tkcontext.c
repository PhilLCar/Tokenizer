#include <tkcontext.h>

#define TYPENAME TokenizerContext

////////////////////////////////////////////////////////////////////////////////
TokenizerContext *_(Construct)(const Map *map)
{
  if (this) {
    String *escape = Map_ValueAtKeyDeref((Map*)map, "escape");
    String *open   = Map_ValueAtKeyDeref((Map*)map, "open");
    String *close  = Map_ValueAtKeyDeref((Map*)map, "close");

    if (open) {
      this->escape = escape ? escape->base[0] : 0;
      this->open   = NEW (String) (open->base);
      this->close  = NEW (String) (close ? close->base : "\n");
    } else {
      THROW(NEW (Exception)("Can't infer the opening symbol of a given context."));
      DELETE (this);
    }
  } else {
    THROW(NEW (MemoryAllocationException)());
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    DELETE (this->open);
    DELETE (this->close);
  }
}

#undef TYPENAME