# Multilevel feedback queue scheduling

This C program simulates CPU scheduling using three different algorithms: Round Robin (with two different time slices) and First-Come-First-Serve (FCFS). The scheduling is performed on a set of processes with randomly generated burst times.

## Implementation Overview

The program defines a `Process` structure representing a process with attributes such as process ID, burst time, remaining time, and a flag indicating the final execution level. Three queues (`queueRR`, `queueRR2`, `queueFcfs`) and three waiting queues (`waitingQueueRR`, `waitingQueueRR2`, `waitingQueueFcfs`) are utilized for process scheduling.

## Data Structures

- **Process Structure:**
  - `pid`: Process ID
  - `burst`: Burst time
  - `remaining_time`: Remaining execution time
  - `finalLevel`: Flag indicating the final execution level

- **Node Structure:**
  - `process`: Pointer to a `Process` structure
  - `next`: Pointer to the next node in the queue

- **Queue Structure:**
  - `head`: Pointer to the head of the queue
  - `tail`: Pointer to the tail of the queue
  - `capacity`: Full capacity of the queue
  - `size`: Current size of the queue

## Queue Operations

- **enqueue:** Adds a process to the end of the queue.
- **dequeue:** Removes and returns the first process from the queue.
- **isEmpty:** Checks if the queue is empty.
- **isFull:** Checks if the queue is full.
- **freeQ:** Frees the memory occupied by the queue.

## Scheduling Algorithms

### Round Robin (RR) Algorithm

- Processes get a small unit of CPU time called a quantum.
- If the time expires before the process finishes, it is moved to another queue with a longer quantum.
- Two queues: `queueRR` (quantum = 8) and `queueRR2` (quantum = 16).

### First-Come-First-Serve (FCFS) Algorithm

- Non-preemptive algorithm with a queue of size 30.

## Main Simulation

- Generates a set of 100 processes with random burst times.
- Schedules processes using a CPU time-slicing approach:
  - Round Robin Q = 8 (50% of total time)
  - Round Robin Q = 16 (30% of total time)
  - FCFS (20% of total time)

## Execution

The program prints the details of each process upon completion, including the process ID, burst time, and remaining time.

## Cleanup

Memory allocated for queues and processes is freed at the end of the simulation.


## Notes

- The simulation follows the specified time-slicing ratios for each scheduling algorithm.
- The randomness in process scheduling decisions is achieved through the use of random seeds and probabilities.
- Adjust the quantum values, queue sizes, and time-slicing ratios based on specific requirements.
