#include <tokenizer.h>

#define TYPENAME Tokenizer

// (!) This list should be defined in the same order as the enum!
const PAIR(const char*, TokenizerFlag) TOKENIZER_FLAGS[TOKENIZER_NCAT] = {
	{ "== Whitespace ==",  TOKENIZER_FLAG_IGNORE                       },
	{ "== Charseq ==",     TOKENIZER_FLAG_BREAK | TOKENIZER_FLAG_PAIR  },
	{ "== Comment ==",     TOKENIZER_FLAG_BREAK | TOKENIZER_FLAG_PAIR  },
	{ "== Punctuation ==", TOKENIZER_FLAG_BREAK                        },
	{ "== Reserved ==",    TOKENIZER_FLAG_NONE                         }
};

// Gets a char with escape '\\'
/******************************************************************************/
char _getchar(Stream *s) 
{
	char c = Stream_getc(s);

	if (c == '\\') c = Stream_esc(s);
	
	return c;
}

// Gets a line from the PRS file
/******************************************************************************/
String *_getline(Stream *s) 
{
	String *line = NEW (String) ("");

	for (char c = _getchar(s); c != '\n' && c != EOF; c = _getchar(s)) String_append(line, c);

	return line;
}

// Gets a line from the PRS file
/******************************************************************************/
void _skipline(Stream *s) 
{
	for (char c = _getchar(s); c != '\n' && c != EOF; c = _getchar(s));
}

// Gets an array from the PRS file
/******************************************************************************/
Array *_getarray(Stream *s) 
{
	Array  *list = NEW (Array) (sizeof(String));
	String *line;
	
	while (1) {
		line = _getline(s);
		if (line && line->length) {
			Array_pusho(list, line);
		} else {
			DELETE (line);
			break;
		}
	}

	return list;
}

// Gets the maximum string length from an array
/******************************************************************************/
int _maxlength(Array *tokens) 
{
	int max = 0;

	for (int i = 0; i < tokens->size; i++) {
		String *s = Array_at(tokens, i);

		if (s->length > max) max = s->length;
	}

	return max;
}

////////////////////////////////////////////////////////////////////////////////
Tokenizer *_(cons)()
{
	if (_this) {
		_this->lookahead = 1;
		_this->escape    = 0;

		for (int i = 0; i < TOKENIZER_NCAT; i++) TokenizerArray_cons(&_this->tokens[i]);
	}

	return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
	for (int i = 0; i < TOKENIZER_NCAT; i++) TokenizerArray_free(&_this->tokens[i]);
}

////////////////////////////////////////////////////////////////////////////////
void _(add_array)(TokenizerCategory tcat, Array *tokens)
{
	TokenizerArray *_tokens = &_this->tokens[tcat];
	int             length  = _maxlength(tokens);

	if (length > 1) {
		_tokens->flags |= TOKENIZER_FLAG_MULTI;
		if ((_tokens->flags & TOKENIZER_FLAG_BREAK) && (length > _this->lookahead)) {
			_this->lookahead = length;
		}
	}

	while (_tokens->base.size) {
		String *token = Array_pop(_tokens);

		if (length > 1) Array_pusho(_tokens, token);
		else {
			Array_push(_tokens, &token->base[0]);
			DELETE (token);
		}
	}
	DELETE (tokens);
}

////////////////////////////////////////////////////////////////////////////////
void _(load_file)(const char *filename)
{
	Stream *file  = fromFileStream(fopen(filename, "r"));

	if (file && file->base) {
		String *line;
		
		while (!file->eos) {
			line = Stream_getline(file);

			// Parse headers
			if (!strcmp(line->base, "== EOF ==")) {
				break;
			} else if (!strcmp(line->base, "== Escape Character ==")) {
				_this->escape = _getchar(file);
				_skipline(file);
			} else {
				for (int i = 0; i < TOKENIZER_NCAT; i++) {
					if (equals(line, TOKENIZER_FLAGS[i].first)) {
						Tokenizer_add_array(_this, i, _getlist(file));
						break;
					}
				}
			}
		}
	}
}