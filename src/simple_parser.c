#include <simple_parser.h>

#define TYPENAME SimpleParser

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(cons)(const char *filename)
{
  Stream *file = fromFile(filename, "r");

  if (_this && file) {
    Map         *subsection = NULL;
    ObjectArray *list       = NULL;

    _this->content = NEW (Map) (sizeof(String), sizeof(String), (Comparer)String_equals);

    if (_this->content) {
      Map_setkey(_this->content, "", NEW (ObjectArray) (sizeof(String)));

      do {
        String *line = sgetline(file);

        if (line && line->length) {
          if (list) {
            // Add the line to the current list
            ObjectArray_push(list, line);
          } else {
            int split = String_ccontains(line, ":");
            String *sectname, *subname;
            

            if (split > -1) {
              sectname = String_substring(NEW (String) (line->base), 0, split++);
              subname  = String_substring(line, split, line->length - split);
            } else {
              sectname = NEW (String) ("");
              subname  = line;
            }

            if (!(subsection = Map_vatkey(_this->content, sectname))) {
              subsection = Map_setkey(_this->content, sectname, NEW (Map) (sizeof(String), sizeof(ObjectArray), (Comparer)String_equals))->second;
            }

            if (!(list = Map_vatkey(subsection, subname))) {
              list = Map_setkey(subsection, subname, NEW (ObjectArray) (sizeof(String)))->second;
            }

            DELETE (sectname);
            DELETE (subname);
          }
        } else {
          subsection = NULL;
          list       = NULL;
        }
      } while (!file->eos);
    } else {
      _this = NULL;
    }

    sclose(file);
  } else {
    _this = NULL;
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  DELETE (_this->content);
}

