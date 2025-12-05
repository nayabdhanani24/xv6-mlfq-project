#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

//system call declaration
uint64
sys_getprocinfo(void)
{
  uint64 addr;  //user pointer to struct procinfo
  struct procinfo pi;
  
  // Get pointer argument from user space
  if(argaddr(0, &addr) < 0)
    return -1;
  
  struct proc *p = myproc();
  
  // Fill in the process info structure
  pi.pid = p->pid;
  pi.state = p->state;
  pi.priority = p->priority;
  pi.queue = p->queue;
  pi.ticks = p->ticks_in_queue;
  pi.totalticks = p->total_ticks;
  
  // Copy to user space
  if(copyout(p->pagetable, addr, (char *)&pi, sizeof(pi)) < 0)
    return -1;
  
  return 0;
}
