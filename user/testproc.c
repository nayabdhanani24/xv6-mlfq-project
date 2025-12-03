#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("Testing getprocinfo system call...\n");
  struct procinfo info; getprocinfo(&info);
  exit(0);
}
