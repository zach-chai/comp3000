#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define PRINT_ADDRESS(ID, I) printf("%s is at: %8X\n", ID, &I)

extern int etext, edata, end;

int kb1[256];
int kb2[512];

void fun2()
{
  int automatic = 10;
  int *dynamic = malloc(100 * sizeof(int));

  PRINT_ADDRESS("fun2 automatic", automatic);
  PRINT_ADDRESS("fun2 dynamic", dynamic);

  free(dynamic);
}

void fun()
{
  int automatic = 10;
  int *dynamic = malloc(100 * sizeof(int));

  PRINT_ADDRESS("fun automatic", automatic);
  PRINT_ADDRESS("fun dynamic", dynamic);

  fun2();

  free(dynamic);
}

int main()
{
  static int static_int = 5;
  static char static_char = 'c';
  static char* static_str = "I am a string";

  PRINT_ADDRESS("etext", etext);

  // function addresses
  PRINT_ADDRESS("main", main);
  PRINT_ADDRESS("fun", fun);
  PRINT_ADDRESS("fun2", fun2);

  PRINT_ADDRESS("edata", edata);

  // static variable addresses
  PRINT_ADDRESS("static_int", static_int);
  PRINT_ADDRESS("static_char", static_char);
  PRINT_ADDRESS("static_str", static_str);

  // global addresses
  PRINT_ADDRESS("kb1", kb1);
  PRINT_ADDRESS("kb2", kb2);

  PRINT_ADDRESS("end", end);

  fun();

  return 0;
}
