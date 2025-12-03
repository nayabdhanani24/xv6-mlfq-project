# WEEK 1 & 2 TESTING GUIDE
# xv6 MLFQ Scheduler Project

## Build and Run
```bash
cd /home/nukhba/xv6-mlfq-project
make qemu
```

## Week 1 Tests - System Call Functionality

### Test 1: Basic getprocinfo
```
testgetinfo
```
**Expected Output:**
- Shows PID, state, priority levels
- Displays time slice budget
- Shows total runtime
- All fields should have valid values (not garbage)

### Test 2: Process information
```
testproc
```
**Expected Output:**
- Process information successfully retrieved
- Shows base priority, current level
- Displays scheduling statistics

## Week 2 Tests - MLFQ Scheduler Behavior

### Test 3: CPU-bound Process (Demotion)
```
cpubound
```
**Expected Behavior:**
- Process starts at Queue Level 0 (highest priority)
- As it consumes CPU, it gets DEMOTED to lower queues (Level 1, 2, 3)
- Final queue level should be HIGHER than initial (e.g., ends at Level 3)
- Most runtime should be in Q2 and Q3 (lower priority queues)

**Success Criteria:**
✅ Initial Level: 0
✅ Final Level: 2 or 3
✅ Shows progressive demotion over time

### Test 4: I/O-bound Process (Stays High Priority)
```
iobound
```
**Expected Behavior:**
- Process starts at Queue Level 0
- Frequently yields CPU (simulates I/O waits with pause)
- Should STAY at higher priority (Level 0 or 1)
- Final queue level should be similar to initial

**Success Criteria:**
✅ Initial Level: 0
✅ Final Level: 0 or 1 (not demoted)
✅ Most runtime in Q0 or Q1

### Test 5: MLFQ Fairness Test
```
mlfqtest
```
**Expected Behavior:**
- Creates 2 children: CPU-bound and I/O-bound
- CPU-bound child gets demoted (lower priority)
- I/O-bound child stays at high priority
- Demonstrates scheduler favors interactive processes

**Success Criteria:**
✅ CPU-bound ends at Level 2-3
✅ I/O-bound stays at Level 0-1
✅ Different processes get different treatment based on behavior

### Test 6: Scheduler Statistics
```
schedtest
```
**Expected Behavior:**
- Shows detailed queue statistics
- Demonstrates queue transitions
- Shows percentage of time in each queue

## Understanding the Output

### Queue Levels (Priority):
- **Q0**: Highest priority, shortest time slice (4 ticks)
- **Q1**: Medium-high priority (8 ticks)
- **Q2**: Medium-low priority (16 ticks)
- **Q3**: Lowest priority, longest time slice (32 ticks)

### Key Metrics:
- **Current Level**: Which queue the process is in NOW
- **Time Slice Budget**: Remaining ticks before demotion
- **Total Runtime**: Total CPU time consumed
- **Queue Runtime**: Time spent in each specific queue

## Troubleshooting

### If tests don't show expected behavior:
1. Check that MLFQ scheduler is implemented in kernel/proc.c
2. Verify time quantum values match spec
3. Ensure demotion happens when time slice expires
4. Check that I/O operations (pause) reset time slice

### If getprocinfo returns errors:
1. Verify system call is registered in syscall.c
2. Check procinfo struct matches between kernel and user
3. Ensure copyout is working correctly

## Exit QEMU
Press: Ctrl-A then X

## Clean and Rebuild
```bash
make clean
make qemu
```
