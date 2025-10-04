#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q2.h"

const char exp_str[] = "0123456789";

void main (int argc, char *argv[])
{
  circular_buf *cb;        
  uint32 h_mem;            
  sem_t s_procs_completed;
  lock_t buffer_lock;
  char get_char;
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
 
  Printf("Consumer: My PID is %d\n", Getpid());
  
  //Consumer Code
  while(num_chars < STRING_SIZE) {
    // Acquire lock
    if(lock_acquire(buffer_lock) != SYNC_SUCCESS){
        Printf("Bad lock buffer_lock (%d) acquire in ", buffer_lock); Printf(argv[0]); Printf("\n");
        Exit();
    } 

    //Check if buffer is not empty
    if(cb->head != cb->tail) {
      //if not empty, check if next item is the expected item
      get_char = cb->buffer[cb->tail];
      if(get_char == exp_str[num_chars]) {
          //if next item is expected item, remove item
          cb->tail = (cb->tail + 1) % BUF_SIZE;
          Printf("Consumer %d removed : %c\n", Getpid(), get_char); 
          num_chars++;
      }
    }

    //Release lock
    if(lock_release(buffer_lock) != SYNC_SUCCESS){
      Printf("Bad lock buffer_lock (%d) release in ", buffer_lock); Printf(argv[0]); Printf("\n");
      Exit();
    }
  }
  
  // Signal the semaphore to tell the original process that we're done
  Printf("Consumer: PID %d is complete.\n", Getpid());

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
    
  }
}