#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define STR_LEN 100
char g_text[STR_LEN];

void print_reverse(void *arg) {

  int i;
  int beg_word = STR_LEN;

  char    tmp[STR_LEN];
  char *  orig_str = (char *) arg;

  for (i=STR_LEN - 1; i >= 0; i--)
  {
    /* COPY INDEX */
    tmp[i] = orig_str[i];

    if (' ' == orig_str[i])
    /* IF SPACE, PRINT */
    {
      printf ("%s", tmp[i]);
      memset (tmp, '\0', (int)STR_LEN);
    }
  }
}

void store_word_count(void *arg) {

}

void append_to_file(void *arg) {

}

int main() {

  pthread_t t1, t2;
  char str[STR_LEN];
  int i;

  printf( "Enter a line of text:\n");
  fgets(str, 100, stdin);

  printf("%s", str);

  if (strcmp("exit", str) == 0) {
    return 0;
  }

  print_reverse((void*)str);

  return 0;
}