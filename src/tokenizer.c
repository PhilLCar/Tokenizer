#include <tokenizer.h>

#define TYPENAME Tokenizer

////////////////////////////////////////////////////////////////////////////////
Tokenizer *_(cons)(const JSON *config)
{
  if (this) {
		this->whitespaces = NEW (String) ("");

		if (this->whitespaces) {
			Array *whitespaces = Map_vatkey((Map*)config, "whitespaces");

			for (int i = 0; i < whitespaces->size; i++) {
				String_append(this->whitespaces, ((String*)Array_atptr(whitespaces, i))->base[0]);
			}
			

		} else {
			this = NULL;
		}
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
}
