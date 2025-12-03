#include "kernel/types.h"
#include "user/user.h"

void
cpu_intensive()
{
  int counter = 0;
  for(int i = 0; i < 1000000; i++) {
    counter++;
    if(i % 100000 == 0) {
      printf("Process %d: count = %d\n", getpid(), i);
    }
  }
}

int
main()
{
  printf("Time Slice Test - Creating 2 CPU-bound processes\n");
  
  int pid = fork();
  if(pid == 0) {
    printf("Child process starting CPU work\n");
    cpu_intensive();
    printf("Child process finished\n");
    exit(0);
  } else {
    printf("Parent process starting CPU work\n");
    cpu_intensive();
    printf("Parent process finished\n");
    wait(0);
  }
  
  printf("Test complete\n");
  exit(0);
}
