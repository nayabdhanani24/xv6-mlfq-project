#include "kernel/types.h"
#include "user/user.h"
#include "procinfo.h"

int
main()
{
  struct procinfo pi;
  int pid = getpid();
  
  printf("Testing getprocinfo system call...\n");
  
  if(getprocinfo(pid, &pi) < 0) {
    printf("getprocinfo failed\n");
    exit(1);
  }
  
  printf("Process Information:\n");
  printf("  PID: %d\n", pi.pid);
  printf("  State: %d\n", pi.state);
  printf("  Base Priority: %d\n", pi.base_priority);
  printf("  Current Level: %d\n", pi.current_level);
  printf("  Time Slice Budget: %d\n", pi.time_slice_budget);
  printf("  Total Runtime: %d\n", (int)pi.total_runtime);
  
  exit(0);
}
