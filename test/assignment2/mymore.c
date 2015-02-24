#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

  if(argc < 2)
  {
    printf("Expected at least 1 argument");
    return 1;
  }

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
  int size = 1;
  char buffer[size];
  char userBuffer[size];
  char c;
  int lines;
  for(i = 1; i < argc; i++)
  {
    printf("\nOpening file: %s\n", argv[i]);
    lines = 0;
    if((file = open(argv[i], O_RDONLY)) == -1)
    {
      printf("bad file\n");
      tcsetattr(fileno(stdin), TCSANOW, &initial_settings);
      return -1;
    }

    while((c = read(file, buffer, size)) != 0)
    {
      if(buffer[0] == 10)
        ++lines;
      if(lines == 20)
      {
        while(1)
        {
          read(0, userBuffer, size);
          if(userBuffer[0] == 32)
          {
            lines = 0;
            break;
          }
        }
      }
      write(1, buffer, size);
    }

    printf("Closing file: %s\n", argv[i]);
    close(file);
  }

  tcsetattr(fileno(stdin), TCSANOW, &initial_settings);
  return 0;
}
