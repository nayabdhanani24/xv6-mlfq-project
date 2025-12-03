#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/procinfo.h"

void
print_info(int pid, const char *label)
{
  struct procinfo info;
  if(getprocinfo(pid, &info) == 0) {
    printf("  %s: PID=%d Level=%d Budget=%d Runtime=%d\n", 
           label, pid, info.current_level, 
           info.time_slice_budget, (int)info.total_runtime);
  }
}

int
main(int argc, char *argv[])
{
  printf("=== MLFQ Scheduler Test ===\n");
  printf("Testing CPU-bound vs I/O-bound process scheduling\n\n");
  
  printf("Creating processes:\n");
  
  // Fork CPU-bound child
  int pid1 = fork();
  if(pid1 == 0) {
    // CPU-bound child
    printf("CPU-bound child (PID %d) starting...\n", getpid());
    volatile int sum = 0;
    for(int i = 0; i < 80000000; i++) {
      sum += i;
      if(i % 20000000 == 0 && i > 0) {
        print_info(getpid(), "CPU child");
      }
    }
    printf("CPU-bound child done (sum check: %d)\n", sum & 0xFF);
    exit(0);
  }
  
  // Fork I/O-bound child
  int pid2 = fork();
  if(pid2 == 0) {
    // I/O-bound child
    printf("I/O-bound child (PID %d) starting...\n", getpid());
    for(int i = 0; i < 12; i++) {
      print_info(getpid(), "I/O child");
      pause(5);
    }
    printf("I/O-bound child done\n");
    exit(0);
  }
  
  // Parent monitors both children
  printf("\nParent (PID %d) monitoring children:\n", getpid());
  printf("CPU-bound PID: %d\n", pid1);
  printf("I/O-bound PID: %d\n\n", pid2);
  
  for(int i = 0; i < 8; i++) {
    pause(10);
    printf("Status update #%d:\n", i+1);
    print_info(pid1, "CPU-bound");
    print_info(pid2, "I/O-bound");
    print_info(getpid(), "Parent   ");
    printf("\n");
  }
  
  // Wait for children to finish
  wait(0);
  wait(0);
  
  printf("=== MLFQ Test Complete ===\n");
  printf("\nExpected Results:\n");
  printf("  - CPU-bound process should be at LOWER priority (higher level number)\n");
  printf("  - I/O-bound process should stay at HIGHER priority (lower level number)\n");
  printf("  - This demonstrates MLFQ prioritizing interactive processes\n\n");
  
  exit(0);
}
