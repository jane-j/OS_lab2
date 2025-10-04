#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q2.h"

const char put_char[] = "0123456789";

void main (int argc, char *argv[])
{
  circular_buf *cb;        
  uint32 h_mem;            
  sem_t s_procs_completed;
  lock_t buffer_lock;
  int num_chars = 0;
  int i;

  //Usage of current program
  if (argc != 4) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_shared_memory_page> <handle_to_page_mapped_semaphore> <handle_to_buffer_lock>\n"); 
    Exit();
  } 

  // Convert the command-line strings into integers for use as handles
  h_mem = dstrtol(argv[1], NULL, 10); // The "10" means base 10
  s_procs_completed = dstrtol(argv[2], NULL, 10);
  buffer_lock = dstrtol(argv[3], NULL, 10);
  
  // Map shared memory page into this process's memory space
  if ((cb = (circular_buf *)shmat(h_mem)) == NULL) {
    Printf("Could not map the virtual address to the memory in "); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
  
  Printf("Producer: My PID is %d\n", Getpid());

  //Producer Code
  while(num_chars < STRING_SIZE) {
    // Acquire lock
    if(lock_acquire(buffer_lock) != SYNC_SUCCESS){
        Printf("Bad lock buffer_lock (%d) acquire in ", buffer_lock); Printf(argv[0]); Printf("\n");
        Exit();
    }

    //Check if buffer is not full
    if((cb->head + 1) % BUF_SIZE != cb->tail) {
      //if not full, insert item
        cb->buffer[cb->head] = put_char[num_chars];
        cb->head = (cb->head + 1) % BUF_SIZE;
        Printf("Producer %d inserted : %c \n", Getpid(), put_char[num_chars]);
        num_chars = num_chars + 1;
    }

    //Release lock
    if(lock_release(buffer_lock) != SYNC_SUCCESS){
      Printf("Bad lock buffer_lock (%d) release in ", buffer_lock); Printf(argv[0]); Printf("\n");
      Exit();
    }
  }

  // Signal the semaphore to tell the original process that we're done
  Printf("Producer: PID %d is complete.\n", Getpid());

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}