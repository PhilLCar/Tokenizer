#include <tkregex.h>

#define TYPENAME TokenizerRegex

////////////////////////////////////////////////////////////////////////////////
TokenizerRegex *_(cons)(int id, const Map *map)
{
  if (this) {
    Pair  *group = *(Pair**)map;
    Map   *info  = group->second.object;
    Array *post  = Map_vatkey(info, "post");

    this->id = id;
    

    this->name = NEW (String) (((String*)group->first.object)->base);
    
    regcomp(&this->init, ((String*)Map_vatkey(info, "init"))->base, 0);

    this->post = NEW (Map) (OBJECT_TYPE(String), NATIVE_TYPE(regex_t), (Comparer)String_cmp);

    for (int i = 0; i < post->size; i++) {
      Pair *current = Array_at(post, i);

      String *first  = current->first.object;
      String *second = current->second.object;

      regex_t regex;

      regcomp(&regex, second->base, 0);
      Map_setkey(this->post, first->base, &regex);
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  DELETE (this->name);
  DELETE (this->post);
}