# Week 2 MLFQ Implementation Summary

## Changes Made

### 1. Core Scheduler Rewrite (`kernel/proc.c`)

**Function: `scheduler()`**
- ✅ Replaced round-robin with MLFQ queue priority logic
- ✅ Scans queues Q0 → Q3 (highest to lowest priority)
- ✅ Runs first RUNNABLE process found in highest non-empty queue
- ✅ Integrated `check_boost()` for Week 3 starvation prevention

**Key Implementation:**
```c
// Scan queues from highest priority (Q0) to lowest (Q3)
for(int q = 0; q < NQUEUES && !found; q++) {
    for(p = proc; p < &proc[NPROC]; p++) {
        if(p->state == RUNNABLE && p->queue == q) {
            // Run this process
        }
    }
}
```

### 2. Timer Interrupt Handling (`kernel/trap.c`)

**Function: `usertrap()`** - Timer interrupt (which_dev == 2)
- ✅ Tracks `ticks_in_queue` for each running process
- ✅ Increments `total_ticks` and `boost_ticks`
- ✅ Enforces time slice limits per queue:
  - Q0: 1 tick
  - Q1: 2 ticks
  - Q2: 4 ticks
  - Q3: 8 ticks
- ✅ Demotes processes when time slice expires
- ✅ Resets `ticks_in_queue` after demotion

**Key Implementation:**
```c
if(p->ticks_in_queue >= slice) {
    if(p->queue < 3) {
        p->queue++;  // Demote
    }
    p->ticks_in_queue = 0;
}
```

### 3. Process Initialization

**Function: `userinit()`**
- ✅ First process (init) starts in Q0
- ✅ Initializes all MLFQ fields

**Function: `kfork()`**
- ✅ Child processes start in Q0 (fresh priority)
- ✅ Inherit parent's name but not queue position

### 4. I/O Handling

**Function: `wakeup()`**
- ✅ Resets `ticks_in_queue` when waking from SLEEPING
- ✅ Keeps process in current queue (rewards I/O behavior)
- ✅ I/O-bound processes stay in high-priority queues

### 5. Queue Management (`kernel/proc.c`)

**Function: `add_to_queue()`**
- ✅ Adds process to tail of specified queue (FIFO within level)
- ✅ Maintains linked list using `mlfq_next` pointer
- ✅ Resets `ticks_in_queue`

**Function: `remove_from_queue()`**
- ✅ Removes process from current queue's linked list
- ✅ Updates head/tail pointers correctly
- ✅ Thread-safe with `mlfq_lock`

**Function: `get_time_slice()`**
- ✅ Returns appropriate time quantum for each queue level

### 6. Data Structure Updates (`kernel/proc.h`)

**Added to `struct proc`:**
- ✅ `struct proc *mlfq_next` - For linked list queue management

## MLFQ Behavior Summary

### Time Quantum Progression
- **Q0**: 1 tick (highest priority, shortest quantum)
- **Q1**: 2 ticks
- **Q2**: 4 ticks
- **Q3**: 8 ticks (lowest priority, longest quantum)

### Process Lifecycle
1. **New process** → Starts in Q0
2. **CPU-bound** → Uses full time slice → Demotes to Q1 → Q2 → Q3
3. **I/O-bound** → Yields before time slice → Stays in current queue
4. **Priority boost** (every 1000 ticks) → All processes reset to Q0

### Scheduling Policy
- **Queue Priority**: Q0 > Q1 > Q2 > Q3
- **Within Queue**: Round-robin (FIFO)
- **Preemption**: Time-slice based (enforced by timer interrupt)

## Testing Instructions

### 1. Compile xv6
```bash
make clean
make qemu
```

### 2. Test Programs Available
```bash
# Test basic MLFQ behavior
$ mlfqtest

# CPU-bound process (should demote to Q3)
$ cpubound

# I/O-bound process (should stay in Q0/Q1)
$ iobound

# Starvation test (Week 3)
$ starvtest

# Test getprocinfo syscall
$ testgetinfo
```

### 3. Expected Behavior

**CPU-bound Process:**
```
PID 3: queue=0, ticks=1, total=1    # Just started
PID 3: queue=1, ticks=1, total=2    # Demoted after 1 tick
PID 3: queue=2, ticks=2, total=6    # Demoted after 2 ticks
PID 3: queue=3, ticks=5, total=15   # At lowest priority
```

**I/O-bound Process:**
```
PID 4: queue=0, ticks=0, total=5    # Stays in Q0
PID 4: queue=0, ticks=0, total=10   # Still in Q0 (yields early)
```

## Known Limitations

1. **Single-core only** (CPUS := 1 in Makefile) - as recommended by project guide
2. **Queue operations** - Currently O(n) scan; could optimize with better data structures
3. **No gaming prevention** - Process can game by yielding just before time slice expires

## Week 3 TODO

- [x] Priority boost implemented (`priority_boost()`, `check_boost()`)
- [ ] Test starvation prevention with CPU-bound and low-priority processes
- [ ] Fine-tune BOOST_INTERVAL (currently 1000 ticks)
- [ ] Add gaming prevention (track cumulative time in level)
- [ ] Performance testing and documentation

## Files Modified

1. `kernel/proc.c` - Scheduler, process initialization, queue management
2. `kernel/trap.c` - Timer interrupt handling and time slice enforcement
3. `kernel/proc.h` - Added `mlfq_next` field
4. `kernel/getprocinfo.c` - Already implemented (Week 1)

## Deliverables Status

- [x] Week 1: getprocinfo system call
- [x] Week 2: MLFQ scheduler with 4 queues
- [x] Week 2: Time-slice enforcement and demotion
- [x] Week 2: Round-robin within each level
- [x] Week 3: Priority boosting (implemented, needs testing)
