#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "procinfo.h"

// System call wrapper for getprocinfo
// User calls: getprocinfo(pid, &info)
// This extracts the arguments and calls the kernel getprocinfo() helper
uint64
sys_getprocinfo(void)
{
  int pid;
  uint64 uaddr;  // user pointer to struct procinfo
  struct procinfo info;
  
  // Get arguments: pid and pointer to procinfo struct
  argint(0, &pid);
  argaddr(1, &uaddr);
  
  // Call kernel helper function (defined in proc.c)
  if(getprocinfo(pid, &info) < 0)
    return -1;
  
  // Copy result to user space
  if(copyout(myproc()->pagetable, uaddr, (char*)&info, sizeof(info)) < 0)
    return -1;
  
  return 0;
}
