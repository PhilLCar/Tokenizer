#include <tkcontext.h>

#define TYPENAME TokenizerContext

////////////////////////////////////////////////////////////////////////////////
TokenizerContext *_(Construct)(const Map *map)
{
  if (this) {
    Pair *esc_pair = Map_At((Map*)map, "escape");
    Pair *clo_pair = Map_At((Map*)map, "close");

    String *escape = esc_pair ? Pair_DerefS(esc_pair) : NULL;
    String *open   = Map_ValueAtDeref((Map*)map, "open");
    String *close  = clo_pair ? Pair_DerefS(clo_pair) : NULL;

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