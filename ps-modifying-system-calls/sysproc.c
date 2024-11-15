#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "fcntl.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//Custom uniq system call
uint64
sys_uniq(void)
{
  int n,i;
  int j = 0, k = 0, f = 0, l = 0;
  int eq = 0, cnt = 1;
  char buffer[512];
  int var_icd;
  char temp_curr[512] = "";
  char temp_prev[512] = "";
  char var_ic = 'a';
  char var_ip = 'a';
  
  argint(0, &n); 
  argstr(1, buffer, sizeof(buffer)); 
  argint(2, &var_icd);  //-c or -d or -i ; by default dummy char o is passed
  
  printf("\nUniq command is getting executed in kernel mode\n");
  for(i=0;i<=n ;i++)
    {  
      if(buffer[i]!='\n'){
          temp_curr[j] = buffer[i];
          j+=1;
      }
      else{
          temp_curr[j]='\n';
          //compare previous and current statement
          while(temp_curr[k] != '\n' || temp_prev[k] != '\n'){          
              if(temp_curr[k] == temp_prev[k])
                  k+=1;
              else if(var_icd == 'i' && temp_curr[k] != '\n' && temp_prev[k] != '\n'){  
                  //convert string to lowercase to compare when -i
                  if(temp_curr[k] >= 'A' && temp_curr[k] <= 'Z'){
                      var_ic = temp_curr[k] + 32;
                  }else
                      var_ic = temp_curr[k];
                  if(temp_prev[k] >= 'A' && temp_prev[k] <= 'Z'){
                      var_ip = temp_prev[k] + 32;
                  }else
                      var_ip = temp_prev[k];
                  if(var_ic == var_ip){
                      k+=1;
                  }
                  else{
                      f = 1;
                      break;
                  }
              }else{
                f = 1;
                break;
              }
            }
        if(f == 0) //equal lines
            eq = 1; 
        if(var_icd != 'c' && var_icd != 'd'){  
            if(eq == 0){ //lines not equal
                for(l=0;temp_curr[l]!='\0';l++){
                  temp_prev[l] = temp_curr[l];
                }
                temp_prev[l]='\0';
                printf("%s",temp_curr);
            }
        }else if(var_icd == 'c'){
             if(temp_prev[0] != '\0'){
                  if(eq == 1)
                    cnt++;
                  else{
                    printf("%d %s", cnt, temp_prev);
                    cnt = 1;
                  }
              }
              //copy current line to a variable
              for(l=0;temp_curr[l]!='\0';l++){
                  temp_prev[l] = temp_curr[l];
              }
              temp_prev[l]='\0';
              if(i > n-2){ //print last line
                  printf("%d %s", cnt, temp_curr);
              }
        }else if(var_icd == 'd'){
              if(eq == 1)
                  cnt++;
              else{
                  if(cnt > 1)
                      printf("%s", temp_prev);
                  for(l=0;temp_curr[l]!='\0';l++){
                      temp_prev[l] = temp_curr[l];
                  }
                  temp_prev[l]='\0';
                  cnt = 1;
              }
              if(i > n-2 && cnt > 1){
                  printf("%s",temp_curr);
              }    
          }

        //clear current line variable
        memset(temp_curr, 0, sizeof(temp_curr));
        j=0;
        k=0;
        eq=0;
        f=0;
      }
    } 
     return 0;
} 

uint64
sys_head(void)
{
  int n,i;
  int l=0;
  char buffer[512];
  int line;
  char convStr[2];
  argint(0, &n); 
  argstr(1, buffer, sizeof(buffer)); 
  argint(2, &line); //lines to print - default 14
  printf("Head command is getting executed in kernel mode\n");
  for(i=0;i<=n ;i++)
    {  
      if(buffer[i]!='\n'){
        convStr[0] = buffer[i];
        convStr[1]='\0';
        printf("%s", convStr);
      }
      else if (l == (line-1)){
        printf("\n");
        return 0;
      }
      else{
        printf("\n");
        l++;
      }
    }
  return 0; 
} 

uint64
sys_waitext(void)
{
  uint64 wtime;
  uint64 rtime;
  uint64 p;
  
  argaddr(0, &wtime);

  argaddr(1, &rtime);

  argaddr(2, &p);
    
  return waitext(wtime, rtime, p);
}

int
sys_ps(void)
{
  return ps();
}