#include <tokenizer.h>

#define TYPENAME Tokenizer

/******************************************************************************/
int STATIC (lookahead)(const Map *config)
{
	int lookahead = 1;

	const List *symbols  = Map_ValueAtKey(config, "symbol");
	const List *contexts = Map_ValueAtKey(config, "context");

	for (const List *s = symbols; !List_Empty(s); s = List_Next(s)) {
		const Pair *current = List_Head(s);

		for (const List *l = current->second; !List_Empty(l); l = List_Next(l)) {
			const String *symbol = List_Head(l);

			if (symbol->length > lookahead) lookahead = symbol->length;
		}
	}

	for (const List *c = contexts; !List_Empty(c); c = List_Next(c)) {
		const Pair   *current = List_Head(c);
		const Map    *context = current->second;
		const String *symbol  = Map_ValueAtKey(context, "open");
		
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
void _(mapadd)(Map *map, int key, void *element)
{
	Pair *pair = Map_At(map, &key);

	if (!pair) {
		pair = Map_Set(map, NEW (int) (key), NEW (ObjectArray) (TYPEOF(String)));
	}

	ObjectArray_Push(pair->second, element);
}

/******************************************************************************/
void _(iterate)(const Map *config, const char *tag, const void (*addgroup)(Tokenizer*, const void*, int))
{
	const List *section = Map_ValueAtKey(config, tag);

	for (const List *l = section; !List_Empty(l); l = List_Next(l)) {
		const Pair *current = List_Head(l);

		const String *name   = current->first;
		const void   *object = current->second;

		int groupid = Tokenizer_creategroup(this, name, NULL);

		addgroup(this, object, groupid);
	}
}

/******************************************************************************/
void _(whitespace)(const Map *config)
{
	for (const List *l = Map_ValueAtKey(config, "whitespace"); !List_Empty(l); l = List_Next(l)) {
		String_Append(this->whitespaces, ((const String*)List_Head(l))->base[0]);
	}
}

/******************************************************************************/
void _(symbol)(const void *symbols, int groupid)
{
	for (const List *l = symbols; !List_Empty(l); l = List_Next(l)) {
		const String *symbol    = List_Head(l);
		Map          *dimension = Array_At((const Array*)this->symbols, symbol->length - 1);

		Tokenizer_mapadd(this, dimension, groupid, String_Copy(symbol));
	}
}

void _(keyword)(const void *keywords, int groupid)
{
	for (const List *l = keywords; !List_Empty(l); l = List_Next(l)) {
		const String *keyword = List_Head(l);
		
		Tokenizer_mapadd(this, this->keywords, groupid, String_Copy(keyword));
	}
}

void _(context)(const void *context, int groupid)
{
	const String *symbol    = Map_ValueAtKey(context, "open");
	Map          *dimension = Array_At((const Array*)this->symbols, symbol->length - 1);

	Tokenizer_mapadd(this, dimension, groupid, String_Copy(symbol));

	((TokenizerGroup*)Array_At((const Array*)this->groups, groupid))->context = NEW (TokenizerContext) (context);
}

void _(regex)(const void *regex, int groupid)
{
	Map_Set(this->regexes, NEW (int)(groupid), NEW (Regex) (((const String*)regex)->base));
}

////////////////////////////////////////////////////////////////////////////////
Tokenizer *_(Construct)(Map *config)
{
  if (this) {
		this->lookahead   = Tokenizer_lookahead(config);
		this->whitespaces = NEW (String) ("");
		this->symbols     = NEW (ObjectArray) (TYPEOF (Map));
		this->keywords    = NEW (Map) (TYPEOF (int));
		this->regexes     = NEW (Map) (TYPEOF (int));
		this->groups      = NEW (ObjectArray) (TYPEOF (TokenizerGroup));
		

		if (this->whitespaces 
		 && this->symbols
		 && this->keywords
		 && this->regexes
		 && this->groups) {
			for (int i = 0; i < this->lookahead; i++) {
				ObjectArray_Push(this->symbols, NEW (Map) (TYPEOF (int)));
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
	return ObjectArray_IndexOfKey(this->groups, name);
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
		const List* dimension = Array_At((const Array*)this->symbols, i);

		for (const List *l = dimension; !List_Empty(l); l = List_Next(l)) {
			const Pair *current = List_Head(l);

			const int   *id      = current->first;
			const Array *symbols = current->second;

			for (int k = 0; k < symbols->size; k++) {
				const String *symbol = Array_At(symbols, k);

				if (String_StartsWith(search, symbol->base)) {
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
	for (const List *l = (const List*)this->regexes; !List_Empty(l); l = List_Next(l)) {
		const Pair *current = List_Head(l);

		const int   *id    = current->first;
		const Regex *regex = current->second;

		if(Regex_Matches(regex, search->base)) {
			return *id;
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Keyword)(const String *search)
{
	for (const List *l = (const List*)this->keywords; !List_Empty(l); l = List_Next(l)) {
		const Pair *current = List_Head(l);

		const int   *id       = current->first;
		const Array *keywords = current->second;

		for (int j = 0; j < keywords->size; j++) {
			const String *keyword = Array_At(keywords, j);

			if (String_Equals(search, keyword)) {
				return *id;
			}
		}
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////////////
Tokenizer *STATIC (Open)(const char *filename)
{
	return NEW (Tokenizer)((Map*) NEW (JSONFile) (filename, ACCESS_READ));
}

#undef TYPENAME
