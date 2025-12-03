#ifndef _PROCINFO_H
#define _PROCINFO_H

struct procinfo {
  int pid;
  int state;
  int priority;    // MLFQ priority (0-3, 0 highest)
  int queue;       // MLFQ queue (0-3)
  int ticks;       // Ticks used in current queue
  int totalticks;  // Total ticks used
};

#endif
