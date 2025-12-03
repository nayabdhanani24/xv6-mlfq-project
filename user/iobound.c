#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/procinfo.h"

int
main(int argc, char *argv[])
{
  struct procinfo info;
  int pid = getpid();
  
  printf("=== I/O-bound Test ===\n");
  printf("PID %d starting I/O-intensive work...\n\n", pid);
  
  // Get initial state
  if(getprocinfo(pid, &info) == 0) {
    printf("INITIAL STATE:\n");
    printf("  Queue Level: %d\n", info.current_level);
    printf("  Time Slice Budget: %d\n", info.time_slice_budget);
    printf("  Total Runtime: %d ticks\n\n", (int)info.total_runtime);
  }
  
  // I/O-intensive work - sleep to simulate I/O operations
  printf("Simulating I/O operations (with sleep)...\n");
  for(int i = 0; i < 15; i++) {
    printf("  I/O operation %d/15...", i+1);
    
    // Small amount of work
    volatile int x = 0;
    for(int j = 0; j < 100000; j++) {
      x += j;
    }
    
    // Sleep to simulate I/O wait
    pause(3);
    
    // Check status after I/O
    if(getprocinfo(pid, &info) == 0) {
      printf(" Level=%d Budget=%d Runtime=%d\n", 
             info.current_level, 
             info.time_slice_budget,
             (int)info.total_runtime);
    }
  }
  
  // Get final state
  printf("\nI/O work complete!\n");
  if(getprocinfo(pid, &info) == 0) {
    printf("\nFINAL STATE:\n");
    printf("  Queue Level: %d (should be similar to initial)\n", info.current_level);
    printf("  Time Slice Budget: %d\n", info.time_slice_budget);
    printf("  Total Runtime: %d ticks\n", (int)info.total_runtime);
    printf("\nTime spent in each queue:\n");
    printf("  Q0 (highest): %d ticks\n", (int)info.queue_runtime[0]);
    printf("  Q1:           %d ticks\n", (int)info.queue_runtime[1]);
    printf("  Q2:           %d ticks\n", (int)info.queue_runtime[2]);
    printf("  Q3 (lowest):  %d ticks\n", (int)info.queue_runtime[3]);
  }
  
  printf("\n=== Test Result: I/O-bound process should STAY at HIGH priority ===\n");
  printf("Expected: Final level ~= Initial level (process yields before exhausting slice)\n\n");
  
  exit(0);
}
