#include <jsonfile.h>
#include <tokenstream.h>
#include <diagnostic.h>

int main(void)
{
  TokenStream *stream = TokenStream_open("tst/file.tst", "tst/config.json");

  while (!((Stream*)stream)->eos) {
    Token *token = TokenStream_get(stream);

    printf("%s\n", ((String*)token)->base);
  }

  DELETE (stream);
}