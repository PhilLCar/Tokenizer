#include <tokenstream.h>

#define TYPENAME TokenStream

////////////////////////////////////////////////////////////////////////////////
TokenStream *_(Construct)(const Tokenizer *tokenizer, CharStream *stream)
{
  if (Stream_Construct(BASE(0), NEW (TrackedStream) (stream, tokenizer->lookahead))) {
    this->tokenizer = tokenizer;
    this->next      = NEW (ObjectArray) (TYPEOF (Token));
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  Stream_Destruct(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
void _(Close)()
{
  DELETE (*BASE(1));
  DELETE (this->next);
}

////////////////////////////////////////////////////////////////////////////////
Token *_(Peek)()
{
  if (!this->next->base.size) 
  {
    void *token = TokenStream_Get(this);

    if (token) TokenStream_Unget(this, token);
  }

  return Array_Last((Array*)this->next);
}

////////////////////////////////////////////////////////////////////////////////
Token *_(Get)()
{
  TrackedStream *ts    = *BASE(1);
  Token         *token = NULL;

  if (BASE(0)->eos) return token;

  if (this->next->base.size > 0) {
    token = ObjectArray_Pop(this->next, 1);
  } else {
    int group = -1;

    token = NEW (Token) ("");

    while (!((Stream*)ts)->eos) {
      int new  = !((String*)token)->length;
      int size = 0;

      if (Tokenizer_WhiteSpace(this->tokenizer, &ts->buffer) >= 0) {
        CharStream_Get((CharStream*)ts);
        if (new) continue;
        else     break;
      } else if ((group = Tokenizer_Symbol(this->tokenizer, &ts->buffer, &size)) >= 0) {
        TokenizerGroup *tkgroup = Array_At((Array*)this->tokenizer->groups, group);
        Token          *next;

        if (new) {
          next = token;
        } else {
          TokenStream_Unget(this, NEW (Token)(""));
          next = TokenStream_Peek(this);
        }

        next->line     = ts->line;
        next->position = ts->position;
        next->group    = group;
        
        for (int i = 0; i < size; i++) {
          String_Append((String*)next, TrackedStream_Get(ts));
        }

        if (tkgroup->context) {
          String_Destruct((String*)next);
          String_Construct((String*)next, "");

          while (!((Stream*)ts)->eos && !String_StartsWith(&ts->buffer, tkgroup->context->close->base)) {
            String_Append((String*)next, CharStream_ReadWith((CharStream*)ts, tkgroup->context->escape));
          }

          TrackedStream_Skip(ts, tkgroup->context->close->length);
        }
        
        break;
      } else {
        if (new) {
          token->line     = ts->line;
          token->position = ts->position;
        }

        String_Append((String*)token, CharStream_Get((CharStream*)ts));
      }
    }
    
    if ((group = Tokenizer_Keyword(this->tokenizer, (String*)token)) >= 0 
     || (group = Tokenizer_Regex  (this->tokenizer, (String*)token)) >= 0) {
      token->group = group;
    }
  }
  
  if (String_Eq((String*)token, "")) {
    DELETE (token);
    BASE(0)->eos = 1;
  }

  return token;
}

////////////////////////////////////////////////////////////////////////////////
void _(Unget)(Token *token)
{
  ObjectArray_Push(this->next, token);
  BASE(0)->eos = 0;
}

////////////////////////////////////////////////////////////////////////////////
void _(Put)(Token *token)
{
  TrackedStream *ts = *BASE(1);

  { // Attempt to the put the token back on stream exactly where it was
    while (ts->line < token->line) {
      CharStream_Put((CharStream*)ts, '\n');
    }

    while (ts->position < token->position) {
      CharStream_Put((CharStream*)ts, ' ');
    }
  }

  for (int i = 0, c; (c = token->base.base[i]); i++) {
    CharStream_Put((CharStream*)ts, c);
  }
}

TokenStream *STATIC (Open)(const Tokenizer *tokenizer, const char *filename)
{
  return NEW (TokenStream)(tokenizer, (CharStream*) NEW (FileStream) (fopen(filename, "r")));
}