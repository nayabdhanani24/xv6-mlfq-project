#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/procinfo.h"

void
show_queue_stats(int pid)
{
  struct procinfo info;
  if(getprocinfo(pid, &info) == 0) {
    printf("PID %d (%s):\n", pid, info.name);
    printf("  Current Level: %d\n", info.current_level);
    printf("  Base Priority: %d\n", info.base_priority);
    printf("  Time Slice Budget: %d ticks\n", info.time_slice_budget);
    printf("  Total Runtime: %d ticks\n", (int)info.total_runtime);
    printf("  Queue Distribution:\n");
    int total = 0;
    for(int i = 0; i < MLFQ_LEVELS; i++) {
      total += info.queue_runtime[i];
      printf("    Q%d: %d ticks", i, (int)info.queue_runtime[i]);
      if(total > 0) {
        printf(" (%d%%)", (int)((info.queue_runtime[i] * 100) / total));
      }
      printf("\n");
    }
  }
}

int
main(int argc, char *argv[])
{
  printf("=== Scheduler Statistics Test ===\n\n");
  
  int pid = getpid();
  printf("Testing process: PID %d\n\n", pid);
  
  printf("Phase 1: Initial state\n");
  show_queue_stats(pid);
  
  printf("\nPhase 2: CPU work (should cause demotion)\n");
  volatile int sum = 0;
  for(int i = 0; i < 50000000; i++) {
    sum += i;
  }
  show_queue_stats(pid);
  
  printf("\nPhase 3: I/O work (voluntary yields)\n");
  for(int i = 0; i < 5; i++) {
    pause(5);
  }
  show_queue_stats(pid);
  
  printf("\nPhase 4: More CPU work\n");
  for(int i = 0; i < 50000000; i++) {
    sum += i;
  }
  show_queue_stats(pid);
  
  printf("\n=== Test Complete ===\n");
  printf("Checksum: %d\n", sum & 0xFFFF);
  
  exit(0);
}
