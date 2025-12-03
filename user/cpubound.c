#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/procinfo.h"

int
main(int argc, char *argv[])
{
  struct procinfo info;
  int pid = getpid();
  
  printf("=== CPU-bound Test ===\n");
  printf("PID %d starting CPU-intensive work...\n\n", pid);
  
  // Get initial state
  if(getprocinfo(pid, &info) == 0) {
    printf("INITIAL STATE:\n");
    printf("  Queue Level: %d\n", info.current_level);
    printf("  Time Slice Budget: %d\n", info.time_slice_budget);
    printf("  Total Runtime: %d ticks\n\n", (int)info.total_runtime);
  }
  
  // CPU-intensive work - consume time slices
  printf("Performing CPU-intensive calculations...\n");
  volatile int sum = 0;
  for(int i = 0; i < 100000000; i++) {
    sum += i;
    
    // Check status periodically
    if(i % 20000000 == 0 && i > 0) {
      if(getprocinfo(pid, &info) == 0) {
        printf("  Progress %d%%: Level=%d Budget=%d Runtime=%d\n", 
               (i/20000000)*20, 
               info.current_level, 
               info.time_slice_budget, 
               (int)info.total_runtime);
      }
    }
  }
  
  // Get final state
  printf("\nCPU work complete!\n");
  if(getprocinfo(pid, &info) == 0) {
    printf("\nFINAL STATE:\n");
    printf("  Queue Level: %d (should be higher than initial)\n", info.current_level);
    printf("  Time Slice Budget: %d\n", info.time_slice_budget);
    printf("  Total Runtime: %d ticks\n", (int)info.total_runtime);
    printf("\nTime spent in each queue:\n");
    printf("  Q0 (highest): %d ticks\n", (int)info.queue_runtime[0]);
    printf("  Q1:           %d ticks\n", (int)info.queue_runtime[1]);
    printf("  Q2:           %d ticks\n", (int)info.queue_runtime[2]);
    printf("  Q3 (lowest):  %d ticks\n", (int)info.queue_runtime[3]);
  }
  
  printf("\n=== Test Result: CPU-bound process should get DEMOTED ===\n");
  printf("Expected: Final level > Initial level (process moved to lower priority)\n\n");
  
  exit(0);
}
