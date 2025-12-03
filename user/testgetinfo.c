#include "kernel/types.h"
#include "user/user.h"
#include "procinfo.h"

int
main()
{
  struct procinfo pi;
  
  printf("Testing getprocinfo system call...\n");
  
  if(getprocinfo(&pi) < 0) {
    printf("getprocinfo failed\n");
    exit(1);
  }
  
  printf("Process Information:\n");
  printf("  PID: %d\n", pi.pid);
  printf("  State: %d\n", pi.state);
  printf("  Priority: %d\n", pi.priority);
  printf("  Queue: %d\n", pi.queue);
  printf("  Ticks in queue: %d\n", pi.ticks);
  printf("  Total ticks: %d\n", pi.totalticks);
  
  exit(0);
}
