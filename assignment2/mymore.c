#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  struct termios initial_settings, new_settings;

  tcgetattr(fileno(stdin), &initial_settings);
  new_settings=initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_cc[VMIN] = 1;
  new_settings.c_cc[VTIME] = 0;
  if (tcsetattr(fileno(stdin), TCSANOW, &new_settings)!=0)
  {
    fprintf(stderr, "could not set attributes\n");
    return 1;
  }

  int i;
  int file;
  char buffer[10];
  char c;
  for(i = 1; i < argc; i++)
  {
    printf("\nOpening file: %s\n", argv[i]);

    if((file = open(argv[i], O_RDONLY)) == -1)
    {
      printf("bad file\n");
      tcsetattr(fileno(stdin), TCSANOW, &initial_settings);
      return -1;
    }

    while((c = read(file, buffer, 10)) != 0)
    {
      write(1, buffer, c);
    }

    printf("Closing file: %s\n", argv[i]);
    close(file);
  }

  //Before the return 0, put this in the code to return the terminal
  tcsetattr(fileno(stdin), TCSANOW, &initial_settings);
  return 0;
}
