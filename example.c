#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stringprep_nameprep.h>

/*
 * Compiling using libtool and pkg-config is recommended:
 *
 * $ libtool cc -o example example.c `pkg-config --cflags --libs libstringprep`
 * $ ./example
 * Input string encoded as `ISO-8859-1': ª
 * Before locale2utf (length 2): aa 0a 
 * Before stringprep (length 3): c2 aa 0a 
 * After stringprep (length 2): 61 0a 
 * $ 
 *
 */

int main(int argc, char *argv[])
{
  char buf[BUFSIZ];
  char *p;
  int rc, i;

  printf("Input string encoded as `%s': ",
	 stringprep_locale_charset ());
  fflush(stdout);
  fgets(buf, BUFSIZ, stdin);

  printf("Before locale2utf (length %d): ", strlen(buf));
  for (i=0; i < strlen(buf); i++)
    printf("%02x ", buf[i] & 0xFF);
  printf("\n");

  p = stringprep_locale_to_utf8 (buf);
  if (p)
    {
      strcpy(buf, p);
      free(p);
    }
  else
    printf("Could not convert string to UTF-8, continuing anyway...\n");

  printf("Before stringprep (length %d): ", strlen(buf));
  for (i=0; i < strlen(buf); i++)
    printf("%02x ", buf[i] & 0xFF);
  printf("\n");

  rc = stringprep(buf, BUFSIZ, 0, stringprep_nameprep);
  if (rc != STRINGPREP_OK)
    printf("Stringprep failed with rc %d...\n", rc);
  else
    {
      printf("After stringprep (length %d): ", strlen(buf));
      for (i=0; i < strlen(buf); i++)
        printf("%02x ", buf[i] & 0xFF);
      printf("\n");
    }

  return 0;
}
