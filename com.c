#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
  pid_t pid;
  int i, j, k, fd[5][2];
  int err;
  char input[256];
  char * ptr, temp;
  char * arg[10], * proc[5];
  char * arg_t;
  while(1)
  {
    memset(&input[0], 0, sizeof(input));
    memset(&arg[0], 0, sizeof(arg));
    memset(&proc[0], 0, sizeof(proc));
    arg_t = malloc(1024 * sizeof(char));
    printf("> ");
    if(!fgets(input, 256, stdin))
    {
      exit(-1);
    }
    for (i = 0; i < 256; i++)
    {
      if (input[i] == '\n') input[i] = '\0';
    }
    if (input[0] == 'q') exit(0);
    ptr = strtok(input, "|");
    i = -1;
    while(ptr)
    {
      i++;
      proc[i] = ptr;
      ptr = strtok(NULL, "|");
    }
    for (i = 0; i < 5; i++)
      pipe(fd[i]);
    ptr = strtok(proc[0], " ");
    i = -1;
    while(ptr)
    {
      i++;
      arg[i] = ptr;
      ptr = strtok(NULL, " ");
    }
    ptr = arg[0];
    pid = fork();
    if(!pid)
    {
      dup2(fd[0][1], 1);
      execvp(ptr, arg);
      exit(-2);
    }
    wait(NULL);
    j = 1;
    while (proc[j])
    {
      memset(&arg[0], 0, sizeof(arg));
      memset(arg_t, 0, 1024 * sizeof(char));
      ptr = strtok(proc[j], " ");
      i = 0;
      while(ptr)
      {
        arg[i] = ptr;
        ptr = strtok(NULL, " ");
        i++;
      }
      k = 0;
      close(fd[j - 1][1]);
      read(fd[j - 1][0], arg_t, 1024 * sizeof(char));
      arg[i] = arg_t;
      pid = fork();
      if(!pid)
      {
        ptr = arg[0];
        dup2(fd[j][1], 1);
        execvp(ptr, arg);
        exit(-2);
      }
      wait(NULL);
      j++;
    }
    close(fd[j - 1][1]);
    err = 1;
    while(err)
    {
      err = read(fd[j - 1][0], &temp, sizeof(char));
      printf("%c", temp);
    }
  }
  exit(0);
}
