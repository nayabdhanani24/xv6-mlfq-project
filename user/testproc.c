#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("Testing getprocinfo system call...\n");
  struct procinfo info;
  int pid = getpid();
  
  if(getprocinfo(pid, &info) < 0) {
    printf("getprocinfo failed\n");
    exit(1);
  }
  
  printf("getprocinfo succeeded!\n");
  printf("PID: %d\n", info.pid);
  printf("State: %d\n", info.state);
  printf("Base Priority: %d\n", info.base_priority);
  printf("Current Level: %d\n", info.current_level);
  
  exit(0);
}
