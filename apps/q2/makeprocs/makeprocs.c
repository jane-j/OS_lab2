#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q2.h"

void main (int argc, char *argv[])
{
  int numprocs = 0;               // Used to store number of processes to create
  int i;                          // Loop index variable
  circular_buf *cb;               // Used to get address of shared memory page
  uint32 h_mem;                   // Used to hold handle to shared memory page
  sem_t s_procs_completed;        // Semaphore used to wait until all spawned processes have completed
  char h_mem_str[10];             // Used as command-line argument to pass mem_handle to new processes
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
  lock_t buffer_lock;
  char buffer_lock_str[10];

  //Usage of current program
  if (argc != 2) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of processes to create>\n");
    Exit();
  }

  //Number of producers/consumers to create
  numprocs = dstrtol(argv[1], NULL, 10);
  Printf("Creating %d producers and %d consumers\n", numprocs, numprocs);

  //Allocate shared memory
  if ((h_mem = shmget()) == 0) {
    Printf("ERROR: could not allocate shared memory page in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }

  //Map shared memory to current process' memory space
  if ((cb = (circular_buf *)shmat(h_mem)) == NULL) {
    Printf("Could not map the shared page to virtual address in "); Printf(argv[0]); Printf(", exiting..\n");
    Exit();
  }

  //Initialize circular buffer
  cb->head = 0;
  cb->tail = 0;

  //Create lock for circular buffer
  if ((buffer_lock = lock_create()) == SYNC_FAIL) {
    Printf("Bad lock_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  //Create semaphore to wait for all child proccesses to complete
  if ((s_procs_completed = sem_create(-(2*numprocs-1))) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  //Create argument strings for child processes
  ditoa(h_mem, h_mem_str);
  ditoa(s_procs_completed, s_procs_completed_str);
  ditoa(buffer_lock, buffer_lock_str);

  //Create child processes : Producers and Consumers
  for(i=0; i<numprocs; i++) {
    process_create(PRODUCER_FILE_TO_RUN, h_mem_str, s_procs_completed_str, buffer_lock_str, NULL);
    //Printf("Producer %d created\n", i);
    process_create(CONSUMER_FILE_TO_RUN, h_mem_str, s_procs_completed_str, buffer_lock_str, NULL);
    //Printf("Consumer %d created\n", i);
  }

  //Wait until all spawned processes have finished.
  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("All other processes completed, exiting main process.\n");
}
