#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int wt, rt, st;
    int pid;
    char *argv2[argc-1];

    //arguments to be passed in exec() from index 1
    for(int i=1; i<argc; i++){
      argv2[i-1] = argv[i];
    }
     
    pid = fork();
    if (pid == 0) {
      // Child process
      exec(argv[1], argv2);
      printf("Failed to execute %s\n", argv[1]);
      exit(1);
    } else if (pid > 0) {
      printf("%s pid %d \n", argv[1], pid);
      // Parent process
       waitext(&wt, &rt, &st);
    } else {
      printf("Failed to fork %s\n", argv[1]);
    }    
  
  exit(0);
}
