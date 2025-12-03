#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/procinfo.h"

int
main(int argc, char *argv[])
{
  printf("=== Starvation Prevention Test ===\n");
  printf("This test verifies that priority boosting prevents starvation\n\n");
  
  // Create a CPU-intensive process that will get demoted
  int cpu_pid = fork();
  if(cpu_pid == 0) {
    printf("CPU-hog process (PID %d) starting...\n", getpid());
    // Infinite CPU work to stay in low priority
    volatile int sum = 0;
    for(;;) {
      for(int i = 0; i < 1000000; i++) {
        sum += i;
      }
    }
    exit(0);
  }
  
  // Let CPU-hog run and get demoted
  printf("Letting CPU-hog run for a bit to get demoted...\n");
  pause(50);
  
  // Check its priority
  struct procinfo info;
  if(getprocinfo(cpu_pid, &info) == 0) {
    printf("CPU-hog status: Level=%d Runtime=%d\n", 
           info.current_level, (int)info.total_runtime);
  }
  
  // Now create a low-priority process
  int low_pid = fork();
  if(low_pid == 0) {
    int my_pid = getpid();
    printf("\nLow-priority process (PID %d) starting...\n", my_pid);
    printf("This process should eventually get CPU time due to boosting\n\n");
    
    int iterations = 0;
    for(int round = 0; round < 10; round++) {
      // Do some work
      volatile int x = 0;
      for(int i = 0; i < 500000; i++) {
        x += i;
        iterations++;
      }
      
      // Report progress
      if(getprocinfo(my_pid, &info) == 0) {
        printf("Low-priority: Round %d, Level=%d, Iterations=%d, Runtime=%d\n",
               round+1, info.current_level, iterations, (int)info.total_runtime);
      }
      
      pause(10);
    }
    
    printf("\nLow-priority process COMPLETED - NO STARVATION!\n");
    exit(0);
  }
  
  // Parent monitors
  printf("\nParent monitoring (will trigger manual boost after some time)...\n\n");
  
  for(int i = 0; i < 15; i++) {
    pause(20);
    
    printf("=== Status Update %d ===\n", i+1);
    if(getprocinfo(cpu_pid, &info) == 0) {
      printf("  CPU-hog:      Level=%d Runtime=%d\n", 
             info.current_level, (int)info.total_runtime);
    }
    if(getprocinfo(low_pid, &info) == 0) {
      printf("  Low-priority: Level=%d Runtime=%d\n", 
             info.current_level, (int)info.total_runtime);
    }
    
    // Manually trigger boost halfway through
    if(i == 7) {
      printf("\n*** MANUALLY TRIGGERING PRIORITY BOOST ***\n\n");
      boostproc();
    }
  }
  
  // Kill CPU-hog
  kill(cpu_pid);
  wait(0);
  wait(0);
  
  printf("\n=== Starvation Test Complete ===\n");
  printf("If low-priority process completed, boosting works!\n");
  
  exit(0);
}
