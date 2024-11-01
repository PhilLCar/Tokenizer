#include <jsonfile.h>
#include <tokenizer.h>
#include <tokenstream.h>
#include <diagnostic.h>

int main(void)
{
  CHECK_MEMORY

  Tokenizer   *tokenizer = Tokenizer_Open("tst/config.json");
  TokenStream *stream    = TokenStream_Open(tokenizer, "tst/file.tst");

  while (!((Stream*)stream)->eos) {
    Token *token = TokenStream_Get(stream);

    if (token)
    {
      TokenizerGroup *group = Array_At((Array*)stream->tokenizer->groups, token->group);

      printf("%15s : %15s\n", token->base.base, group->base.base);
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