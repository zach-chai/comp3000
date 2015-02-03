#include <sys/types.h>
#include <unistd.h>
#include <string.h>

char mynum='0';
int main(void) {
   int i;
   pid_t fork_return;
   static char buffer[10];
   fork_return = fork();
   if (fork_return == 0) {
      strcpy(buffer, "CHILD"); /*in the child process*/
      for (i=0; i<5; ++i) {/*both processes do this*/
         mynum=i + '0';
         sleep(1); /*5 times each*/
         write(1, buffer, sizeof(buffer));
         write(1, &mynum, 1);
         write(1, "\n", 1);
      }
      return 0;
   }
   else{
      strcpy(buffer, "PARENT"); /*in the parent process*/
      for (i=0; i<5; ++i) { /*both processes do this*/
         sleep(1); /*5 times each*/
         write(1, buffer, sizeof(buffer));
         write(1, &mynum, 1);
         write(1, "\n", 1);
      }
      return 0;
   }
}
