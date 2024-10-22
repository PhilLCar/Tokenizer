#include <tkcontext.h>

#define TYPENAME TokenizerContext

////////////////////////////////////////////////////////////////////////////////
TokenizerContext *_(cons)(const Map *map)
{
  if (this) {
    Pair *esc_pair = Map_atkey((Map*)map, "escape");
    Pair *clo_pair = Map_atkey((Map*)map, "close");

    String *escape = esc_pair ? Pair_sptr(esc_pair) : NULL;
    String *open   = Map_vatkeyptr((Map*)map, "open");
    String *close  = clo_pair ? Pair_sptr(clo_pair) : NULL;

    if (open) {
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
