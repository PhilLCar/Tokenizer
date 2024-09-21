#include <tokenizer.h>

#define TYPENAME Tokenizer

void readfile(const char *filename) {
	Stream *file = fromFile(filename, "r");

	String *line;

	while ((line = sreadline(file, '\\'))) {
		if (!String_starts_with(line, "#") && line->length > 0) {
			switch (line->base[0]) {
				case TOKENIZER_CATEGORY_ESCAPE:
					break;
				case TOKENIZER_CATEGORY_WHITESPACE:
					break;
				case TOKENIZER_CATEGORY_PUNCTUATION:
					break;
				case TOKENIZER_CATEGORY_EXCERPT:
					break;
				case TOKENIZER_CATEGORY_RESERVED:
					break;
				case TOKENIZER_CATEGORY_LOOKAHEAD_MATCH:
					break;
				case TOKENIZER_CATEGORY_POSTPARSE_MATCH:
					break;
				default:
					fprintf(stderr, "Unknown tokenizer category '%c'\n", line->base[0]);
					break;
			}
		}
	}

	sclose(file);
}

TYPENAME *_(cons)()
{
	if (_this) {
		_this->escape     = NEW (TokenizerGroup) ("", 0, 1);
		_this->whitespace = NEW (TokenizerGroup) ("", 0, 1);

		_this->excerpt  = NEW (ObjectArray) (sizeof(TokenizerGroup));
		_this->reserved = NEW (ObjectArray) (sizeof(TokenizerGroup));
		_this->regex1   = NEW (ObjectArray) (sizeof(TokenizerGroup));
		_this->regex2   = NEW (ObjectArray) (sizeof(TokenizerGroup));

		_this->groups = NEW (Array) (sizeof(String*));
	}
}

int _(escape)(const char *sequence)
{
	return TokenizerArray_indexof(_this->escape, sequence);
}

int _(whitespace)(const char *sequence)
{
	return TokenizerArray_indexof(_this->whitespace, sequence);
}

