#include <tokenizer.h>

#define TYPENAME Tokenizer

/******************************************************************************/
int STATIC (intcmp)(const void *a, const void *b)
{
	return *(int*)b - *(int*)a;
}

/******************************************************************************/
int STATIC (lookahead)(const Map *config)
{
	int lookahead = 1;

	const Array *symbols  = Map_ValueAtDeref(config, "symbol");
	const Array *contexts = Map_ValueAtDeref(config, "context");

	for (int i = 0; i < symbols->size; i++) {
		const Pair  *current = Array_At(symbols, i);
		const Array *list    = Pair_DerefS(current);

		for (int j = 0; j < list->size; j++) {
			const String *symbol = Array_AtDeref(list, j);

			if (symbol->length > lookahead) lookahead = symbol->length;
		}
	}

	for (int i = 0; i < contexts->size; i++) {
		const Pair   *current = Array_At(contexts, i);
		const Map    *context = Pair_DerefS(current);
		const String *symbol  = Map_ValueAtDeref(context, "open");
		
		if (symbol->length > lookahead) lookahead = symbol->length;
	}

	return lookahead;
}

/******************************************************************************/
int _(creategroup)(const String *name, const Map *context)
{
	int groupid = Tokenizer_Group(this, name->base);

	if (groupid < 0) {
		groupid = this->groups->base.size;
		ObjectArray_Push(this->groups, NEW (TokenizerGroup) (name));
	}

	return groupid;
}

/******************************************************************************/
void _(mapadd)(Map *map, int *key, void *element)
{
	Pair *pair = NULL;

	if (!(pair = Map_At(map, key))) {
		pair = NEW (Pair) (map->key, map->value);

		Pair_SetF(pair, key);
		Pair_SetS(pair, NEW (ObjectArray) (OBJECT_TYPE(String)));

		pair = ObjectArray_Push((ObjectArray*)map, pair);
	}

	ObjectArray_Push(pair->second.object, element);
}

/******************************************************************************/
void _(iterate)(const Map *config, const char *tag, const void (*addgroup)(Tokenizer*, const void*, int))
{
	const Array *section = Map_ValueAtDeref(config, tag);

	for (int i = 0; i < section->size; i++) {
		const Pair *current = Array_At(section, i);

		const String *name   = current->first.object;
		const void   *object = Pair_DerefS(current);

		int groupid = Tokenizer_creategroup(this, name, NULL);

		addgroup(this, object, groupid);
	}
}

/******************************************************************************/
void _(whitespace)(const Map *config)
{
	const Array *whitespaces = Map_ValueAtDeref(config, "whitespace");

	for (int i = 0; i < whitespaces->size; i++) {
		String_Append(this->whitespaces, ((const String*)Array_AtDeref(whitespaces, i))->base[0]);
	}
}

/******************************************************************************/
void _(symbol)(const void *symbols, int groupid)
{
	for (int j = 0; j < ((const Array*)symbols)->size; j++) {
		const String *symbol    = Array_AtDeref(symbols, j);
		Map          *dimension = Array_At((const Array*)this->symbols, symbol->length - 1);

		Tokenizer_mapadd(this, dimension, &groupid, NEW (String) (symbol->base));
	}
}

void _(keyword)(const void *keywords, int groupid)
{
	for (int j = 0; j < ((const Array*)keywords)->size; j++) {
		const String *keyword = Array_AtDeref(keywords, j);
		
		Tokenizer_mapadd(this, this->keywords, &groupid, NEW (String) (keyword->base));
	}
}

void _(context)(const void *context, int groupid)
{
	const String *symbol    = Map_ValueAtDeref(context, "open");
	Map          *dimension = Array_At((const Array*)this->symbols, symbol->length - 1);

	Tokenizer_mapadd(this, dimension, &groupid, NEW (String) (symbol->base));

	((TokenizerGroup*)Array_At((const Array*)this->groups, groupid))->context = NEW (TokenizerContext) (context);
}

void _(regex)(const void *regex, int groupid)
{
	Map_Set(this->regexes, &groupid, NEW (Regex) (((const String*)regex)->base));
}

////////////////////////////////////////////////////////////////////////////////
Tokenizer *_(Construct)(Map *config)
{
  if (this) {
		this->lookahead   = Tokenizer_lookahead(config);
		this->whitespaces = NEW (String) ("");
		this->symbols     = NEW (ObjectArray) (OBJECT_TYPE(Map));
		this->keywords    = NEW (Map) (NATIVE_TYPE(int), OBJECT_TYPE(ObjectArray), Tokenizer_intcmp);
		this->regexes     = NEW (Map) (NATIVE_TYPE(int), OBJECT_TYPE(Regex),       Tokenizer_intcmp);
		this->groups      = NEW (ObjectArray) (OBJECT_TYPE(TokenizerGroup));
		

		if (this->whitespaces 
		 && this->symbols
		 && this->keywords
		 && this->regexes
		 && this->groups) {
			for (int i = 0; i < this->lookahead; i++) {
				ObjectArray_Push(this->symbols, NEW (Map) (NATIVE_TYPE(int), OBJECT_TYPE(ObjectArray), Tokenizer_intcmp));
			}

			Tokenizer_whitespace(this, config);
			Tokenizer_iterate(this, config, "symbol",  Tokenizer_symbol);
			Tokenizer_iterate(this, config, "keyword", Tokenizer_keyword);
			Tokenizer_iterate(this, config, "context", Tokenizer_context);
			Tokenizer_iterate(this, config, "regex",   Tokenizer_regex);
		} else {
			THROW (NEW (MemoryAllocationException)());
			DELETE(this);
		}

		DELETE (config);
  } else {
		THROW (NEW (MemoryAllocationException)());
	}

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
	if (this) {
		DELETE (this->whitespaces);
		DELETE (this->symbols);
		DELETE (this->keywords);
		DELETE (this->regexes);
		DELETE (this->groups);
	}
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Group)(const char *name)
{
	int index = -1;

	for (int i = 0; i < this->groups->base.size; i++) {
		const String* group = Array_At((const Array*)this->groups, i);

		if (String_Eq(group, name)) {
			index = i;
			break;
		}
	}

	return index;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (WhiteSpace)(const String *search)
{
	for (int i = 0; i < this->whitespaces->length; i++) {
		if (search->base[0] == this->whitespaces->base[i]) {
			return i;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Symbol)(const String *search, int *size)
{
	for (int i = this->lookahead - 1; i>= 0; i--) {
		const Array* dimension = Array_At((const Array*)this->symbols, i);

		for (int j = 0; j < dimension->size; j++) {
			const Pair *current = Array_At(dimension, j);

			const int   *id      = current->first.object;
			const Array *symbols = current->second.object;

			for (int k = 0; k < symbols->size; k++) {
				const String *symbol = Array_At(symbols, k);

				if(String_StartsWith(search, symbol->base)) {
					*size = symbol->length;
					return *id;
				}
			}
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Regex)(const String *search)
{
	for (int i = 0; i < ((const Array*)this->regexes)->size; i++) {
		const Pair *current = Array_At((const Array*)this->regexes, i);

		const int   *id    = current->first.object;
		const Regex *regex = current->second.object;

		if(Regex_Matches(regex, search->base)) {
			return *id;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Keyword)(const String *search)
{
	for (int i = 0; i < ((const Array*)this->keywords)->size; i++) {
		const Pair *current = Array_At((const Array*)this->keywords, i);

		const int   *id       = current->first.object;
		const Array *keywords = current->second.object;

		for (int j = 0; j < keywords->size; j++) {
			const String *keyword = Array_At(keywords, j);

			if (String_Equals(search, keyword)) {
				return *id;
			}
		}
	}

	return -1;
}

Tokenizer *STATIC (Open)(const char *filename)
{
	return NEW (Tokenizer)((Map*) NEW (JSONFile) (filename, 1));
}

#undef TYPENAME
