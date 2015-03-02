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
volatile int g_input;
NODE* head;

void add_in_order(NODE* new)
{
  NODE* curr_node = head;
  while(1)
  {
    if(curr_node->next == NULL)
    {
      new->next = NULL;
      curr_node->next = new;
      break;
    }else if(curr_node->next->data->count >= new->data->count)
    {
      new->next = curr_node->next;
      curr_node->next = new;
      break;
    }
    curr_node = curr_node->next;
  }
}

void insert (char* buff)
{
  NODE* curr_node = head;
  NODE* prev_node = NULL;

  // printf("%c", buff[strlen(buff) - 1]);

  while(1)
  {
    if(curr_node->data != NULL && strcmp(curr_node->data->word, buff) == 0)
    {

      NODE* node = curr_node;
      ++(node->data->count);

      prev_node->next = curr_node->next;

      add_in_order(node);

      return;
    }
    if(curr_node->next == NULL)
      break;
    prev_node = curr_node;
    curr_node = curr_node->next;
  }

  NODE* new = (NODE *) malloc(sizeof(NODE));
  new->data = (WORD *) malloc(sizeof(WORD));
  strcpy(new->data->word, buff);
  new->data->count = 1;

  new->next = head->next;
  head->next = new;
}

void *print_reverse (void *m)
{
  int i;
  int size = 0;
  int str_size;
  char* orig_str;
  int input = g_input;

  while(1)
  {
    if(input == g_input)
    {
      sleep(1);
    } else {

      if (strcmp("exit", g_text) == 0)
      {
        break;
      }
      input = g_input;

      orig_str = g_text;
      str_size = 0;

      for(i=0; i < STR_LEN; i++)
      {
        if('\0' == orig_str[i])
          break;
        ++str_size;
      }

      int end_word = str_size - 1;

      for (i=str_size - 1; i >= 0; i--)
      {
        if (' ' == orig_str[i])
        {
          size = end_word - i;
          printf("%.*s", size, &orig_str[i+1]);
          if(end_word == str_size - 1)
            printf("%s", " ");
          end_word = i;
        }
      }
      printf("%.*s\n", end_word, orig_str);
    }
  }
}

void *store_word_count (void *m)
{
  int i;
  char* orig_str;
  char buffer[WORD_LEN];
  int buff_size = 0;
  int input = g_input;

  while(1)
  {
    if(input == g_input)
    {
      sleep(1);
    }
    else {

      if (strcmp("exit", g_text) == 0)
      {
        break;
      }
      input = g_input;

      orig_str = g_text;
      buff_size = 0;
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
  }

  NODE* curr_node = head->next;
  while(curr_node != NULL)
  {
    printf("%s occured %d times\n", curr_node->data->word, curr_node->data->count);
    if(curr_node->next == NULL)
      break;
    curr_node = curr_node->next;
  }
}

void *append_to_file (void *m)
{
  int input = g_input;
  FILE* file =fopen("as3part1input.txt", "a");
      if (file == NULL)
      {
        printf("Error opening file!\n");
        exit(1);
      }

  while(1)
  {
    if(input == g_input)
    {
      sleep(1);
    }
    else {
      if (strcmp("exit", g_text) == 0)
      {
        break;
      }

      input = g_input;
      fprintf(file, "%s\n", g_text);
    }
  }
  fclose(file);
}

int main()
{

  pthread_t t1, t2, t3;
  const char *m1 = "Thread 1";
  const char *m2 = "Thread 2";
  const char *m3 = "Thread 3";
  int i;
  g_input = 0;
  volatile char u_input[STR_LEN];

  head = (NODE *) malloc(sizeof(NODE));
  head->data = NULL;

  // create threads
  pthread_create(&t1, NULL, print_reverse, (void*) m1);
  pthread_create(&t2, NULL, store_word_count, (void*) m2);
  pthread_create(&t3, NULL, append_to_file, (void*) m2);

  for(;;){

    printf( "Enter a line of text:\n");
    fgets(g_text, STR_LEN, stdin);
    g_text[strlen(g_text) - 1] = '\0';

    ++g_input;

    if(strcmp("exit", g_text) == 0)
    {
      pthread_join(t1, NULL);
      pthread_join(t2, NULL);
      pthread_join(t3, NULL);
      break;
    }
  }

  return 0;
}
