#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

char buf[512];

int head_f(int fd, char *name, int line)
{
    int i, n, l_no; 
    l_no = 0;
    printf("Head command is getting executed in user mode\n");
    while((n = read(fd, buf, sizeof(buf))) > 0 )
    {
      for(i=0;i<=n ;i++)
      {           
        if(buf[i]!='\n'){
          printf("%c",buf[i]);
      }         
      else if (l_no == (line-1)){
            printf("\n");
            return 0;
      }
      else{
          printf("\n");
          l_no++;
      }
    }
  }
  if(n < 0){
    printf("head: unable to read(user mode)\n");
    exit(1);
  }
  return 0;
}

int main(int argc, char *argv[]) {
    int i,n;
    int fd = 0;
    int fdk = 0;
    int flines = 14;    
    char *file;    
    char a;
    int f = 0;
    char buffer[512];
    file = "";
  
    if (argc <= 1) {
      head_f(0, "", flines); 
      while((n = read(0, buffer, sizeof(buffer))) > 0 )
    {
       head(0, buffer, flines);
    }
      if (n < 0) {
        printf("Error: Unable to read file(Kernel mode)\n");
    }        
        exit(0);
      }
      else {
            for (i = 1; i < argc; i++) {
                a = *argv[i];
                if(a != '-'){
                    if ((fd = open(argv[i], 0)) < 0){
                      printf("Error: Cannot Open File %s\n", argv[i]);
                      exit(1);
                     }
                }
                if (a == '-') {  
                    argv[i]++;
                    flines = atoi(argv[i]++);
                    f = 1;
                }
                if(f == 0){
                    printf("\nFile: %s\n", argv[i]);
                    head_f(fd, file, flines);
                    fdk = open(argv[i], 0);
                    while((n = read(fdk, buffer, sizeof(buffer))) > 0 )
                    {
                        printf("\nFile: %s\n", argv[i]);
                        head(n, buffer, flines);
                    }
                    if (n < 0) {
                        printf("Error: Unable to read file(kernel mode)\n");
                        close(fdk);
                    }
                }
                f = 0;
        }
        close(fd);
        exit(0);
      }            
}
