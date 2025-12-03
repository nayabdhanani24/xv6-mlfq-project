uint64
sys_getprocinfo(void)
{
    uint64 addr;  // user pointer to struct procinfo
    struct proc *p = myproc();
    struct procinfo info;
    
    if(argaddr(0, &addr) < 0)
        return -1;
    
    // Fill the info structure
    info.pid = p->pid;
    info.state = p->state;
    info.priority = p->priority;
    info.queue = p->queue;
    info.ticks = p->ticks_in_queue;
    info.totalticks = p->total_ticks;
    
    // Copy to user space
    if(copyout(p->pagetable, addr, (char*)&info, sizeof(info)) < 0)
        return -1;
    
    return 0;
}
