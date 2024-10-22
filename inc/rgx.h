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
END("");

int _(matches)(const char *text);

#undef TYPENAME
#endif