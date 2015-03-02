#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

#define BUFFER "buffer"
#define PARENT 1
#define CHILD 0

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
};

int critical_section(int process)
{
  int num;
  char* modifier;
  char* buf;
  FILE* fptr;

  if(PARENT == process)
  {
    modifier = "parent";
  } else {
    modifier = "child";
  }
  fptr = fopen(BUFFER, "r+");
  if(fptr == NULL)
  {
    perror("Error opening the file.\n");
    return 1;
  }

  fgets(buf, 3, fptr);

  num = atoi(buf);

  ++num;
  printf("The number [%d] modified by %s\n", num, modifier);

  rewind(fptr);
  fprintf(fptr, "%d", num);

  fclose(fptr);
}

int main(void) {
  pid_t pid;

  FILE* fptr = fopen(BUFFER, "w");
  fprintf(fptr, "%d", 0);
  fclose(fptr);

  int sem_id;
  key_t ipc_key = ftok(".", 'S');
  int semflg = IPC_CREAT | 0666;
  int nsems = 2;
  int nsops;
  struct semid_ds sem_buf;
  static ushort sem_array[2] = {1, 0};
  union semun arg;
  arg.buf = &sem_buf;

  struct sembuf acquire = {0, -1, SEM_UNDO},
                release = {0,  1, SEM_UNDO};

  // setup semaphore
  if ((sem_id = semget(ipc_key, nsems, semflg)) == -1)
  {
    perror("semget failed");
    return 1;
  }

  if (semctl(sem_id, 0, IPC_STAT, arg) == -1)
  {
    perror("semctl failed");
    return 1;
  }

  // set semaphore values
  arg.array = sem_array;
  if ((semctl(sem_id, 0, SETALL, arg)) == -1)
  {
    perror("semctl failed");
  }

  pid = fork();
  if(pid == -1)
    {
      perror("Failed to Fork");
      return 1;
    }

    if(pid == 0)
    {
      int i;
      for(i=0; i < 5; i++)
      {
        // acquire resource
        acquire.sem_num = CHILD;
        if (semop(sem_id, &acquire, 1) == -1)
        {
          perror("semop -- child -- acquire failed");
          return 1;
        }

        // critical section code
        critical_section(CHILD);

        // release resource
        release.sem_num = PARENT;
        if (semop(sem_id, &release, 1) == -1)
        {
          perror("semop -- child -- release failed");
          return 1;
        }
      }
      if(semctl(sem_id,0,IPC_RMID,0) == -1)
      {
        perror("semctl failed removing semaphore");
        return 1;
      }
    }
    else
    {

      while(1)
      {
        // acquire resource
        acquire.sem_num = PARENT;
        if (semop(sem_id, &acquire, 1) == -1)
        {
          perror("semop -- parent -- acquire failed");
          return 1;
        }

        // critical section code
        critical_section(PARENT);

        // release resource
        release.sem_num = CHILD;
        if (semop(sem_id, &release, 1) == -1)
        {
          perror("semop -- parent -- release failed");
          return 1;
        }
      }
    }

  return 0;
}
