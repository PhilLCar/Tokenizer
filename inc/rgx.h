#ifndef REGEX_H
#define REGEX_H

// C
#include <regex.h>

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#define TYPENAME Regex

OBJECT (const char *regex) INHERIT (regex_t)
END_OBJECT("");

int CONST (Matches)(const char *text);

// TODO: (low): Improvement: Implement more features

#undef TYPENAME
#endif