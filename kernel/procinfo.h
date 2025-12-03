#ifndef PROCINFO_H
#define PROCINFO_H

#include "types.h"

#define MLFQ_LEVELS 4
#define PROCINFO_NAME_MAX 16

struct procinfo {
  int pid;
  int state;
  int base_priority;
  int current_level;
  int time_slice_budget;
  uint64 total_runtime;
  uint64 queue_runtime[MLFQ_LEVELS];
  char name[PROCINFO_NAME_MAX];
};

#endif // PROCINFO_H
