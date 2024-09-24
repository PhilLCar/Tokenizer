#ifndef SIMPLE_PARSER_H
#define SIMPLE_PARSER_H

// C
#include <stdlib.h>
#include <string.h>

// CUT
#include <array.h>
#include <map.h>
#include <diagnostic.h>
#include <oop.h>
#include <filestream.h>
#include <str.h>

#define TYPENAME SimpleParser

/*
The idea of the simple parser is to provide a quick and easy way to get
information from a file in the following form

[section:]sub-section
value1
value2
...

As such, the resulting object if of the following form

Map
<
  String section-name,
  Map
  <
    String sub-section-name, 
    List
    <
      String value
    > sub-section
  > section
> content

The functions of this object serve to facilitate navigation through this object


*/

OBJECT (const char* filename) INHERIT (Map)
  const char *filename;
END_OBJECT;

ObjectArray *_(slist)(const char *subsection);

ObjectArray *_(list)(const char *section, const char *subsection);

#undef TYPENAME
#endif