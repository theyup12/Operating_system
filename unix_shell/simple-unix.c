#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
/*void parse(char* s, char** command) {
  const char break_chars[] = " \t";
  char* p;
  int i =0;
  p = strtok(s, break_chars);
  while (p != NULL) {
    //printf("token was: %s\n", p);
    command[i] = p; 
    i++;
    p = strtok(NULL, break_chars);
  }
  command[i] = NULL;
  //&
}*/


int main(int argc, const char * argv[]) {  
  char input[BUFSIZ];
  char last_command[BUFSIZ];
  char *command[BUFSIZ];
  char *p_command[BUFSIZ];
  memset(input, 0, BUFSIZ * sizeof(char));
  memset(input, 0, BUFSIZ * sizeof(char));
  bool finished = false;
  bool concurr = false;
  pid_t pid, pid2;
  while (!finished) {
      printf("osh> ");
      fflush(stdout);
    if ((fgets(input, BUFSIZ, stdin)) == NULL) {   // or gets(input, BUFSIZ);
      fprintf(stderr, "no command entered\n");
      exit(1);
    }
    int i;
    input[strlen(input) - 1] = '\0';          // wipe out newline at end of string

    // check for history (!!) command
    if (strncmp(input, "!!", 2) == 0) {
      if (strlen(last_command) == 0) {
        fprintf(stderr, "no commands in history\n");
      }
      strcpy(input, last_command);
      printf("last command was: %s\n", last_command);
    } else if (strncmp(input, "exit", 4) == 0) {   // only compare first 4 letters
      finished = true;
    } else {
      strcpy(last_command, input);
      printf("You entered: %s\n", input);   // you will call fork/exec
    }
      const char break_chars[] = " \t";
      char* p;
      i =0;
      p = strtok(input, break_chars);
      while (p != NULL) {
        //printf("token was: %s\n", p);
        command[i] = p; 
        i++;
        p = strtok(NULL, break_chars);
      }
      command[i] = NULL;
      //&
      concurr = (strncmp(command[i - 1], "&", 1) == 0);
    //input/output
    int j =0;
    //can be  -1
    int inputIdx = 0;
    int outputIdx = 0;
    int is_pipe = 0;
    while(command[j] != NULL){
      //check for <
      if(strcmp(command[j], "<") == 0){
        inputIdx = j;
      }
      //check for >
      if(strcmp(command[j], ">") == 0){
        outputIdx = j;
      }
      if(strcmp(command[j], "|")== 0){
        is_pipe = j;
      }
      j++;
    }
    //pipe 
    int fd[2];
    if(is_pipe){
      int k = 0;
      for(int t = j+1; t < i; ++t, ++k){
          p_command[k] = command[t];
      }
      
      command[j] = NULL;
      if(pipe(fd) != 0){
        fprintf(stderr, "Cannot create a pipe\n");
        exit(1);
      }
    }
    if( (pid=fork())== 0){     //child proccess
      if(inputIdx){
        command[inputIdx] = NULL;
        int inFileID = open(command[inputIdx + 1], O_RDONLY);           
        dup2(inFileID, STDIN_FILENO);
      }
      if(outputIdx){
        command[outputIdx] = NULL;
        int outFileID = open(command[outputIdx + 1], O_CREAT|O_WRONLY|O_TRUNC);             
        dup2(outFileID, STDOUT_FILENO); 
      }
      if(is_pipe){
        command[is_pipe] = NULL;
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
      }
      execvp(command[0], command);
      if(execvp(command[0], command) < 0){
        printf("Command not found.\n");
        exit(0);
      }
    }else{
      if(is_pipe){
        pid2 =fork();
        if(pid2 < 0){
          fprintf(stderr, "fork fail");
        }
        else if(pid2 == 0){
          close(fd[0]);
          dup2(fd[0], STDIN_FILENO);
          close(fd[1]);
          execvp(p_command[0], p_command);
        }else{
          wait(NULL);
          is_pipe = 0;
        }
      }else{
        if(!concurr){
          wait(NULL);
        }else{
        signal(SIGCHLD, SIG_IGN);
        }
      }
    }
    printf("\n");
  }
  
  printf("osh exited\n");
  printf("program finished\n");
  
  return 0;
}