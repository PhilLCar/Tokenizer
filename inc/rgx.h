#ifndef REGEX_H
#define REGEX_H

// C
#include <regex.h>

// CUT
#include <diagnostic.h>
#include <oop.h>

#define TYPENAME Regex

OBJECT (const char *regex)
  regex_t regex;
END(NULL);

int _(matches)(const char *text);

#undef TYPENAME
#endif