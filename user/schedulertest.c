#include "kernel/types.h"
#include "user/user.h"

void
child_process(int id)
{
  // CPU-intensive work
  int x = 0;
  for(int i = 0; i < 100000; i++) {
    x = x + i;
    if(i % 20000 == 0) {
      printf("Process %d: at %d (x=%d)\n", id, i, x);
    }
  }
  exit(0);
}

int
main()
{
  printf("Scheduler Test Start\n");
  
  // Create 3 child processes
  for(int i = 0; i < 3; i++) {
    int pid = fork();
    if(pid == 0) {
      child_process(i);
    }
  }
  
  // Parent also does work
  int y = 0;
  for(int i = 0; i < 100000; i++) {
    y = y + i;
    if(i % 20000 == 0) {
      printf("Parent: at %d (y=%d)\n", i, y);
    }
  }
  
  // Wait for children
  for(int i = 0; i < 3; i++) {
    wait(0);
  }
  
  printf("Scheduler Test Complete\n");
  exit(0);
}
