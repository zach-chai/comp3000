#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#define STR_LEN 16 * 1024
#define WORD_LEN 128

typedef struct
{
  char word[WORD_LEN];
  int count;
} WORD;

typedef struct _NODE NODE;

struct _NODE
{
  NODE* next;
  WORD* data;
};

char g_text[STR_LEN];
NODE* head;

void insert (char* buff)
{
  NODE* curr_node = head;

  while(1)
  {
    if (curr_node->data != NULL && strcmp(curr_node->data->word, buff) == 0)
    {
      ++(curr_node->data->count);
      return;
    }
    if(curr_node->next == NULL)
      break;
    curr_node = curr_node->next;
  }


  curr_node->next = (NODE *) malloc(sizeof(NODE));
  curr_node = curr_node->next;

  curr_node->data = (WORD *) malloc(sizeof(WORD));
  strcpy(curr_node->data->word, buff);
  curr_node->data->count = 0;
  curr_node->next = NULL;
}

void print_reverse ()
{
  int i;
  int size = 0;
  int str_size = 0;
  char tmp[WORD_LEN];
  char* orig_str = g_text;

  for(i=0; i < STR_LEN; i++)
  {
    if('\0' == orig_str[i])
      break;
    ++str_size;
  }

  int end_word = str_size - 2;

  for (i=str_size - 1; i >= 0; i--)
  {
    if (' ' == orig_str[i])
    {
      // buff_size = 0;
      // memset (tmp, '\0', (int)WORD_LEN);

      size = end_word - i;
      printf("%.*s", size, &orig_str[i+1]);
      if(end_word == str_size - 2)
        printf("%s", " ");
      end_word = i;
    }

    // tmp[buff_size] = orig_str[i];
    // ++buff_size;
  }
  printf("%.*s\n", end_word, orig_str);
}

void store_word_count ()
{
  int i;
  char* orig_str = g_text;
  char buffer[WORD_LEN];
  int buff_size = 0;

  memset (buffer, '\0', (int)WORD_LEN);

  for (i=0; i < STR_LEN; i++)
  {
    if (' ' == orig_str[i] || '\0' == orig_str[i])
    {
      if(buff_size != 0)
        insert(buffer);

      buff_size = 0;
      memset (buffer, '\0', (int)WORD_LEN);

      if('\0' == orig_str[i])
      {
        break;
      }
      continue;
    }

    buffer[buff_size] = orig_str[i];
    ++buff_size;
  }
}

void append_to_file (void *arg, int len)
{

}

int main()
{

  pthread_t t1, t2, t3;
  int i;

  head = (NODE *) malloc(sizeof(NODE));
  head->data = NULL;

  printf( "Enter a line of text:\n");
  fgets(g_text, STR_LEN, stdin);

  if (strcmp("exit", g_text) == 0) {
    return 0;
  }

  print_reverse();

  // store_word_count();

  // NODE* curr_node = head->next;
  // while(1)
  // {
  //   printf("%s\n", curr_node->data->word);
  //   if(curr_node->next == NULL)
  //     break;
  //   curr_node = curr_node->next;
  // }

  return 0;
}
