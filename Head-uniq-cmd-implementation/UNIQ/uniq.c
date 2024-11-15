#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[1024];

int uniq_f(int fd, char var_icd) {
    int n, i, j=0, f=0 , cnt = 1;
    char var_ic='a', var_ip='a';
    char temp_curr[1024] = "";
    char temp_prev[1024] = "";
     
    n = read(fd, buf, sizeof(buf));
    printf("Uniq command is getting executed in user mode.\n");
  
    for (i = 0; i < n; i++) {
        if (buf[i] != '\n') {
            temp_curr[j] = buf[i];
            j += 1;
            }
        else {
            temp_curr[j]='\n';
            if(var_icd != 'c' && var_icd != 'd'){  
                 if (strcmp(temp_curr, temp_prev) != 0){
                     if(var_icd == 'i'){
                         int k = 0;
                         //to convert line to lowercase to compare when -i is given
                         while(temp_curr[k] != '\n' && temp_prev[k] != '\n'){   
                             if(temp_curr[k] >= 'A' && temp_curr[k] <= 'Z')
                                 var_ic = temp_curr[k] + 32;
                             else
                                 var_ic = temp_curr[k];
                             if(temp_prev[k] >= 'A' && temp_prev[k] <= 'Z')
                                 var_ip = temp_prev[k] + 32;
                             else
                                 var_ip = temp_prev[k];
                             if(var_ic == var_ip){
                                 k+=1;
                             }
                            else{
                              f = 1;
                              break;
                            }
                        }
                        if(f==1){ //lines are not equal
                            strcpy(temp_prev, temp_curr);
                            printf(temp_curr);
                        }
                        f=0;
                    }else{ //when -i is not given
                        strcpy(temp_prev, temp_curr);
                        printf(temp_curr);
                    }
              
                } 
            }
            if(var_icd == 'c'){ 
                if(temp_prev[0] != '\0'){
                    if(strcmp(temp_curr, temp_prev) == 0)
                        cnt++;
                    else{
                        printf("%d %s", cnt, temp_prev);
                        cnt = 1;
                    }
                  }
                  strcpy(temp_prev, temp_curr);
                  if(i > n-2){ //check last line
                      printf("%d %s", cnt, temp_curr);
                  }
            }else if(var_icd == 'd'){
                if(strcmp(temp_curr, temp_prev) == 0)
                    cnt++;
                else{
                    if(cnt > 1)
                    printf("%s", temp_prev);
                    strcpy(temp_prev, temp_curr);
                    cnt = 1;
                }
                if(i > n-2 && cnt > 1){ //check last line
                    printf("%s",temp_curr);
                }       
            }
            
            memset(temp_curr, 0, sizeof(temp_curr));
            j=0;
        }
    }
    if (n < 0) {
        printf("uniq: read error\n");
        exit(1);
    }
     return 0;
}

int main(int argc, char *argv[]) {
    char buffer[512];
    int n;
    int fdk = 0, fd=0;
    char a;
    char var_icd = 'o';
    int f=0;
    
    if (argc <= 1) {
        uniq_f(0,var_icd);

        while((n = read(0, buffer, sizeof(buffer))) > 0 )
        {
           uniq(0, buffer, var_icd);
        }
        if (n < 0) {
            printf("Error: Unable to read file(Kernel mode)\n");
        }        
        exit(0);   
    } else {
          for (int i = 1; i < argc; i++) {
            a=*argv[i];
            if(a!='-'){
                fd = open(argv[i], 0);
                if (fd < 0) {
                  printf("uniq: cannot open %s\n", argv[i]);
                  exit(1);
                }
            }
            
          if(a=='-'){
              argv[i]++;
              var_icd = *argv[i];
              f=1;
          }
          // when -c/i/d is not given
          if(f==0){
              uniq_f(fd,var_icd);
              fdk = open(argv[i], 0);
              while((n = read(fdk, buffer, sizeof(buffer))) > 0 )
              {
                  uniq(n, buffer, var_icd);
              }
              if (n < 0) {
                  printf("Error: Unable to read file(kernel mode)\n");
              } 
                  close(fdk);
                  close(fd);
          }
          f=0;
        }
    }
    exit(0);
}