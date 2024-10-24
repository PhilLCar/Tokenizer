#include <rgx.h>

#define TYPENAME Regex

////////////////////////////////////////////////////////////////////////////////
Regex *_(cons)(const char *regex)
{
  if (this) {    
    regcomp(&this->regex, regex, 0);
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  regfree(&this->regex);
}

////////////////////////////////////////////////////////////////////////////////
int _(matches)(const char *text)
{
  return !regexec(&this->regex, text, 0, NULL, 0);
}