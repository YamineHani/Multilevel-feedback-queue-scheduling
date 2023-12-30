#include <stdio.h>
#include <stdlib.h>


int totalTime;  //sum of remaining time of all processes
int inQueue[100]; //used to mark if process is in any queue

typedef struct Process {
    int pid;
    int burst;
    int remaining_time;
    int finalLevel; // to ensure that if the process exits level 2 and
                    // returns to level 1 it stays there until it finishes
}Process;

Process* constructorProcess(int pid, int burst){
    Process *process = malloc(sizeof(Process));
    process->pid = pid;
    process->burst = process->remaining_time = burst;
    process->finalLevel = 0; //not final level
    return process;
}

typedef struct Node{
    Process *process;
    struct Node *next;
}Node;

Node* constructorNode(Process *process)
{
    Node *node = malloc(sizeof(Node));
    node->process = process;
    node->next = NULL;
    return node;
}

typedef struct Queue{
    Node *head;
    Node *tail;
    int capacity; //full capacity of the queue
    int size;     //to keep track of current size of queue
}Queue;

Queue* queueRR;
Queue* queueRR2;
Queue* queueFcfs;
Queue* waitingQueueRR;
Queue* waitingQueueRR2;
Queue* waitingQueueFcfs;

Queue* constructorQueue(int capacity){
    Queue *queue = malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}
//enqueue return 1 is enqueue is done successfully and 0 if full
int enqueue(Queue *queue,Process *process){
    if(queue->size < queue->capacity){
        Node *node = constructorNode(process);
        //if first element
        if(queue->size == 0){
            queue->head = queue->tail = node;
        } else{
            queue->tail->next = node;
            queue->tail= node;
        }
        queue->size += 1;
        return 1;
    }
    return 0;
}

//dequeue return process, will need to check first if it is empty
Process* dequeue(Queue *queue){
    Node *n = queue->head;          //get first element in queue
    Process *first = n->process;     //get the process
    queue->head = n->next;          //let the new head be the next element
    free(n);
    queue->size -= 1;
    if(queue->size == 0){            //if the queue is empty head and tail = NULL
        queue->head = queue->tail = NULL;
    }
    return first;
}

//returns 1 if queue is empty and 0 otherwise
int isEmpty(Queue *queue){
    if(queue->size == 0){
        return 1;
    }
    return 0;
}

int isFull(Queue *queue){
    if(queue->size == queue->capacity){
        return 1;
    }
    return 0;
}

void freeQ(Queue *queue) {
    while (queue->head != NULL) {
        Node* temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
    }
    queue->tail = NULL;
    free(queue);
}



void printProcess(Process* process) {
    printf("PID: %d, Burst Time: %d, Remaining Time: %d\n", process->pid, process->burst, process->remaining_time);
}

Process * generateProcesses(){
    Process *processes = malloc(100*sizeof(Process));
    for(int i = 0; i < 100; i++ ){
        processes[i].pid = i + 1; //process pid from 1 to 100
        processes[i].burst = rand() % 100 + 1; //generate burst random from 1 to 100
        processes[i].remaining_time = processes[i].burst;
        processes[i].finalLevel = 0;
        totalTime += processes[i].burst;
        enqueue(waitingQueueRR,&processes[i]); //enqueue all processes in main waiting queue
    }
    return processes;
}
/*          RoundRobin algorithms:
 *              *Each process gets small unit of CPU time called quantum = 8 with queue of size 10
 *              *If the time has expired before the process finishes, the process will be added
 *               to another queue working with Round Robin scheduling (quantum = 16) of size 20
 *              *If it finishes, the process will be printed
 */

void roundRobin(int quantum){
    while (1){

        if(isEmpty(queueRR)){
            while(!isEmpty(waitingQueueRR)&&!isFull(queueRR)){
                Process *process = dequeue(waitingQueueRR);
                enqueue(queueRR,process);
                inQueue[process->pid-1] = 1;
            }
        }
        //if queueRR and waiting queue are empty then there are no processes
        if (isEmpty(queueRR) && isEmpty(waitingQueueRR)) {
            break;
        }
        Process *p = dequeue(queueRR); //once you dequeue make it unvisited
        inQueue[p->pid - 1] = 0;
        if(p->remaining_time > quantum){
            int counter = quantum;
            while (counter != 0)
            { p->remaining_time--; counter--; totalTime--;}
            //if this is the final level for this process then enqueue it back in rr waitingQueueRR
            //else pass it to rr Q = 16 (level 2 waitingQueueRR2)
            if(p->finalLevel){
                enqueue(waitingQueueRR, p);
            } else{
               enqueue(waitingQueueRR2,p);
            }

        } else{
            //finished process
            while (p->remaining_time != 0)
            {p->remaining_time -= 1; totalTime--;}
            printProcess(p);
        }

    }
}

/*          RoundRobin Q = 16:
 *              *If the process finishes in 16 unit of time print it out
 *              *Else there are two equiprobable scenarios: 1. it returns to RoundRobin Q = 8 and stays until it finishes
 *                                                          2. goes to fcfs queue
 */

void roundRobin2(int quantum){

    while (1){
        //if queues are empty you are done
        if (isEmpty(queueRR2) && isEmpty(waitingQueueRR2)) {
            break;
        }
        if(isEmpty(queueRR2)){
            while(!isEmpty(waitingQueueRR2)&&!isFull(queueRR2)){
                Process *process = dequeue(waitingQueueRR2);
                enqueue(queueRR2,process);
                inQueue[process->pid-1] = 1;
            }
        }

        Process *p = dequeue(queueRR2);
        inQueue[p->pid - 1] = 0; //mark unvisited
        if(p->remaining_time > quantum){
            int counter = quantum;
            while (counter != 0)
            { p->remaining_time--; counter--; totalTime--;}
            //50% goes to level 1 50% to level 3
            srand(p->burst); //use diff seeds to ensure randomness
            int prob = rand() % 100;
            if(prob > 50){
                p->finalLevel = 1;
                // Ensure that once it returns to level one it stays there until it finishes its execution
                enqueue(waitingQueueRR,p);
            } else{
                p->finalLevel = 1;
                enqueue(waitingQueueFcfs,p);
            }
        } else{
            while (p->remaining_time != 0){
                p->remaining_time -= 1;
                totalTime -= 1;
            }
            printProcess(p);
        }

    }
}
/*          FCFS algorithms:
 *              *Queue of size 30
 *              *Non-preemptive algorithm
 */

void fcfsScheduling(){
    while (1){

        if(isEmpty(queueFcfs)){
            while(!isEmpty(waitingQueueFcfs)&&!isFull(queueFcfs)){
                Process *process = dequeue(waitingQueueFcfs);
                enqueue(queueFcfs,process);
                inQueue[process->pid-1] = 1;
            }
        }
        if (isEmpty(queueFcfs) && isEmpty(waitingQueueFcfs)) {
            break;
        }

        Process *p = dequeue(queueFcfs);
        while (p->remaining_time != 0) //non-preemptive
        { p->remaining_time -= 1; totalTime--;}
        printProcess(p);
    }
}

int main() {

    queueRR = constructorQueue(10);
    queueRR2 = constructorQueue(20);
    queueFcfs = constructorQueue(30);
    waitingQueueFcfs = constructorQueue(100);
    waitingQueueRR2 = constructorQueue(100);
    waitingQueueRR = constructorQueue(100);

    Process *processes = generateProcesses();

/*          CPU time slicing:
                *RoundRobin Q = 8 ---> 50%
                *RoundRobin Q = 16---> 30%
                *FCFS             ---> 20%
*/
    while (!isEmpty(queueRR)||!isEmpty(queueRR2)||!isEmpty(queueFcfs)
    ||!isEmpty(waitingQueueRR)||!isEmpty(waitingQueueRR2)||!isEmpty(waitingQueueFcfs)){
        int rr1TimeSlice = (int)(totalTime * 0.5);
        int rr2TimeSlice = (int)(totalTime * 0.3)+rr1TimeSlice;
        for(int i = 0; i < totalTime; i++){
            if(i < rr1TimeSlice){
                roundRobin(8);
            }
            else if(i < rr2TimeSlice){
                roundRobin2(16);
            }
            else{
                fcfsScheduling();
            }
        }
    }

    freeQ(queueRR);
    freeQ(queueRR2);
    freeQ(queueFcfs);
    freeQ(waitingQueueRR);
    freeQ(waitingQueueFcfs);
    freeQ(waitingQueueRR2);
    free(processes);

    return 0;
}
