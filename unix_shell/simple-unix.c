#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
void parse(char* s, char** command) {
  const char break_chars[] = " \t";
  char* p;
  int i =0;
  p = strtok(s, break_chars);
  while (p != NULL) {
    printf("token was: %s\n", p);
    command[i] = p; 
    i++;
    p = strtok(NULL, break_chars);
  }
  command[i] = NULL;
  //&
}


int main(int argc, const char * argv[]) {  
  char input[BUFSIZ];
  char last_command[BUFSIZ];
  char *command[BUFSIZ];
  memset(input, 0, BUFSIZ * sizeof(char));
  memset(input, 0, BUFSIZ * sizeof(char));
  bool finished = false;
  pid_t pid;
  while (!finished) {
    printf("osh> ");
    fflush(stdout);


    if ((fgets(input, BUFSIZ, stdin)) == NULL) {   // or gets(input, BUFSIZ);
      fprintf(stderr, "no command entered\n");
      exit(1);
    }
    input[strlen(input) - 1] = '\0';          // wipe out newline at end of string
    printf("input was: \n'%s'\n", input);

    // check for history (!!) command
    if (strncmp(input, "!!", 2) == 0) {
      if (strlen(last_command) == 0) {
        fprintf(stderr, "no last command to execute\n");
      }
      strcpy(input, last_command);
      printf("last command was: %s\n", last_command);
    } else if (strncmp(input, "exit", 4) == 0) {   // only compare first 4 letters
      finished = true;
    } else {
      strcpy(last_command, input);
      printf("You entered: %s\n", input);   // you will call fork/exec
      parse(input, command);
			printf("after parse, what is input: %s\n", input);
      printf("\n");
    }
    //input/output
    int i = 0;
    //can be  -1
    int inputCharIdx = 0;
    int outputCharIdx = 0;
    //mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    while(argv[i] != NULL){
      //check for <
      if(strcmp(argv[i], "<") == 0){
        inputCharIdx = i;
      }
      //check for >
      if(strcmp(argv[i], ">") == 0){
        outputCharIdx = i;
      }
      i++;
    }
    if((pid = fork()) == 0){     //child proccess
      if(inputCharIdx){
        command[inputCharIdx] = NULL;
        //int inFileID = open(argv[inputCharIdx + 1], O_RDONLY, mode); 
        int inFileID = open(command[inputCharIdx + 1], O_RDONLY);           
        dup2(inFileID, STDIN_FILENO);
      }
      if(outputCharIdx){
        command[outputCharIdx] = NULL;
        //int outFileID = open(command[outputCharIdx + 1], O_CREAT|O_WRONLY|O_TRUNC, mode); 
        int outFileID = open(command[outputCharIdx + 1], O_CREAT|O_WRONLY|O_TRUNC);             
        dup2(outFileID, STDOUT_FILENO); 
        }
      if(execvp(command[0], command) < 0){
        printf("Command not found.\n");
        exit(0);
      }
    }
  }
  
  printf("osh exited\n");
  printf("program finished\n");
  
  return 0;
}