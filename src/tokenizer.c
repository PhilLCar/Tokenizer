#include <tokenizer.h>

#define TYPENAME Tokenizer

/******************************************************************************/
int STATIC (_lookahead)(Map *config)
{
	int lookahead = 1;

	Array *symbols  = Map_vatkey(config, "symbol");
	Array *contexts = Map_vatkey(config, "context");

	for (int i = 0; i < symbols->size; i++) {
		Pair  *current = Array_at(symbols, i);
		Array *list    = current->second.object;

		for (int j = 0; j < list->size; j++) {
			String *symbol = Array_at(list, j);

			if (symbol->length > lookahead) lookahead = symbol->length;
		}
	}

	for (int i = 0; i < contexts->size; i++) {
		Pair   *current = Array_at(contexts, i);
		Map    *context = current->second.object;
		String *symbol  = Map_vatkey(context, "open");
		
		if (symbol->length > lookahead) lookahead = symbol->length;
	}

	return lookahead;
}

/******************************************************************************/
int _(_creategroup)(const String *name, const Map *context)
{
	int groupid = Tokenizer_group(this, name->base);

	if (groupid < 0) {
		groupid = this->groups->base.size;
		ObjectArray_push(this->groups, NEW (TokenizerGroup) (name));
	}

	return groupid;
}

/******************************************************************************/
void _(_mapadd)(Map *map, void *key, void *element)
{
	Pair *pair = NULL;

	if (!(pair = Map_atkey(map, key))) {
		pair = NEW (Pair) (map->key, map->value);

		Pair_setf(pair, key);
		Pair_sets(pair, NEW (ObjectArray) (OBJECT_TYPE(String)));

		pair = ObjectArray_push((ObjectArray*)map, pair);
	}

	ObjectArray_push(pair->second.object, element);
}

/******************************************************************************/
void _(_iterate)(Map *config, const char *tag, const void (*addgroup)(Tokenizer*, void*, int))
{
	Array *section = Map_vatkey(config, tag);

	for (int i = 0; i < section->size; i++) {
		Pair *current = Array_at(section, i);

		String *name   = current->first.object;
		void   *object = current->second.object;

		int groupid = Tokenizer__creategroup(this, name, NULL);

		addgroup(this, object, groupid);
	}
}

/******************************************************************************/
void _(_whitespace)(Map *config)
{
	Array *whitespaces = Map_vatkey(config, "whitespace");

	for (int i = 0; i < whitespaces->size; i++) {
		String_append(this->whitespaces, ((String*)Array_atptr(whitespaces, i))->base[0]);
	}
}

/******************************************************************************/
void _(_symbol)(void *symbols, int groupid)
{
	for (int j = 0; j < ((Array*)symbols)->size; j++) {
		String *symbol    = Array_at(symbols, j);
		Map    *dimension = Array_at((Array*)this->symbols, symbol->length - 1);

		Tokenizer__mapadd(this, dimension, &groupid, symbol);
	}
}

void _(_keyword)(void *keywords, int groupid)
{
	for (int j = 0; j < ((Array*)keywords)->size; j++) {
		String *keyword = Array_at(keywords, j);
		
		Tokenizer__mapadd(this, this->keywords, &groupid, keyword);
	}
}

void _(_context)(void *context, int groupid)
{
	String *symbol    = Map_vatkey(context, "open");
	Map    *dimension = Array_at((Array*)this->symbols, symbol->length - 1);

	Tokenizer__mapadd(this, dimension, &groupid, symbol);

	((TokenizerGroup*)Array_at((Array*)this->groups, groupid))->context = NEW (TokenizerContext) (context);
}

void _(_regex)(void *regex, int groupid)
{
	Map_setkey(this->regexes, &groupid, ((String*)regex)->base);
}

////////////////////////////////////////////////////////////////////////////////
Tokenizer *_(cons)(Map *config)
{
  if (this) {
		this->lookahead   = Tokenizer__lookahead(config);
		this->whitespaces = NEW (String) ("");
		this->symbols     = NEW (ObjectArray) (OBJECT_TYPE(Map));
		this->keywords    = NEW (Map) (NATIVE_TYPE(int), OBJECT_TYPE(String), NULL);
		this->regexes     = NEW (Map) (NATIVE_TYPE(int), OBJECT_TYPE(String), NULL);
		

		if (this->whitespaces 
		 && this->symbols
		 && this->keywords
		 && this->regexes
		 && this->groups) {
			for (int i = 0; i < this->lookahead; i++) {
				ObjectArray_push(this->symbols, NEW (Map) (NATIVE_TYPE(int), OBJECT_TYPE(ObjectArray), NULL));
			}

			Tokenizer__whitespace(this, config);
			Tokenizer__iterate(this, config, "symbol",  Tokenizer__symbol);
			Tokenizer__iterate(this, config, "keyword", Tokenizer__keyword);
			Tokenizer__iterate(this, config, "context", Tokenizer__context);
			Tokenizer__iterate(this, config, "regex",   Tokenizer__regex);
		} else {
			Tokenizer_free(this);
			this = NULL;
		}
  }

	DELETE (config);

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
	DELETE (this->whitespaces);
	DELETE (this->symbols);
	DELETE (this->keywords);
	DELETE (this->regexes);
	DELETE (this->regexes);
}

////////////////////////////////////////////////////////////////////////////////
int _(group)(const char *name)
{
	int index = -1;

	for (int i = 0; i < this->groups->base.size; i++) {
		TokenizerGroup *group = Array_at((Array*)this->groups, i);

		if (String_eq(group->name, name)) {
			index = i;
			break;
		}
	}

	return index;
}

////////////////////////////////////////////////////////////////////////////////
int _(whitespace)(String *search)
{
	for (int i = 0; i < this->whitespaces->length; i++) {
		if (search->base[0] == this->whitespaces->base[i]) {
			return i;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
int _(symbol)(String *search, int *size)
{
	for (int i = this->lookahead - 1; i>= 0; i--) {
		Array* dimension = Array_at((Array*)this->symbols, i);

		for (int j = 0; j < dimension->size; j++) {
			Pair *current = Array_at(dimension, j);

			int   *id      = current->first.object;
			Array *symbols = current->second.object;

			for (int k = 0; k < symbols->size; k++) {
				String *symbol = Array_at(symbols, k);

				if(String_strw(search, symbol->base)) {
					*size = symbol->length;
					return *id;
				}
			}
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
int _(regex)(String *search)
{
	for (int i = 0; i < ((Array*)this->regexes)->size; i++) {
		Pair *current = Array_at((Array*)this->regexes, i);

		int   *id    = current->first.object;
		Regex *regex = current->second.object;

		if(Regex_matches(regex, search->base)) {
			return *id;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
int _(keyword)(String *search)
{
	for (int i = 0; i < search->length; i++) {
		Pair *current = Array_at((Array*)this->keywords, i);

		int   *id       = current->first.object;
		Array *keywords = current->second.object;

		for (int j = 0; j < keywords->size; j++) {
			String *keyword = Array_at(keywords, j);

			if(String_strw(search, keyword->base)) {
				return *id;
			}
		}
	}

	return -1;
}
