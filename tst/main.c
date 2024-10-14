#include <diagnostic.h>
#include <jsonfile.h>
#include <filestream.h>
#include <tokenstream.h>

int main(void)
{
  TokenStream *stream = TokenStream_open("tst/file.tst", "tst/config.json");

  while (!((Stream*)stream)->eos) {
    Token *token = TokenStream_get(stream);

    printf("%s\n", ((String*)token)->base);
  }

  DELETE (stream);
}