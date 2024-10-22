#include <jsonfile.h>
#include <tokenstream.h>
#include <diagnostic.h>

int main(void)
{
  CHECK_MEMORY

  Tokenizer   *tokenizer = Tokenizer_open("tst/config.json");
  TokenStream *stream    = TokenStream_open(tokenizer, "tst/file.tst");

  while (!((Stream*)stream)->eos) {
    Token *token = TokenStream_get(stream);

    if (token)
    {
      TokenizerGroup *group = Array_at((Array*)stream->tokenizer->groups, token->group);

      printf("%15s : %15s\n", token->base.base, group->name->base);
    }

    DELETE (token);
  }

  CHECK_MEMORY

  DELETE (stream);

  CHECK_MEMORY

  DELETE (tokenizer);

  CHECK_MEMORY

  STOP_WATCHING
}