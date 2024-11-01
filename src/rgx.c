#include <rgx.h>

#define TYPENAME Regex

////////////////////////////////////////////////////////////////////////////////
Regex *_(Construct)(const char *regex)
{
  if (this) {    
    regcomp(BASE(0), regex, 0);
  } else {
    THROW(NEW (MemoryAllocationException)());
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    regfree(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Matches)(const char *text)
{
  return !regexec(BASE(0), text, 0, NULL, 0);
}

#undef TYPENAME
