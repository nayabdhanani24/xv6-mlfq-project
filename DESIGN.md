# MLFQ Scheduler Design - Week 1
## Student: nayabdhanani24
## Date: December 2, 2024

## 1. Project Overview
Implement Multi-Level Feedback Queue scheduler in xv6-RISC-V.

## 2. Queue Structure
- 4 priority queues: Q0 (highest) to Q3 (lowest)
- Time slices: Q0=10ms, Q1=20ms, Q2=40ms, Q3=80ms
- Round-robin within each queue

## 3. Data Structure Changes
Add to struct proc in proc.h:
  int priority;      // Current queue (0-3)
  int ticks_used;    // Ticks used in current slice
  int total_ticks;   // Total CPU time

## 4. Week 1 Goals
- [x] Setup GitHub repository
- [x] Test xv6 compilation
- [ ] Implement getprocinfo system call
- [ ] Create queue scaffolding
- [ ] Test basic scheduling

## 5. Implementation Plan
1. Modify scheduler() in proc.c
2. Add queue management functions
3. Implement getprocinfo syscall
4. Test with user programs
