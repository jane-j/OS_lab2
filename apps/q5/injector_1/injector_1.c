#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q5.h"

void main (int argc, char *argv[])
{           
  sem_t s_procs_completed;
  sem_t s_N3; //semaphore associated with the molecule this process in injecting
  int num_N3 = 0; //number of molecules to inject
  int i = 0;

  //Usage of current program
  if (argc != 4) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_page_mapped_semaphore> <handle_to_num_molecules> <handle_to_sem_molecules> \n"); 
    Exit();
  } 

  // Convert the command-line strings into integers for use as handles
  s_procs_completed = dstrtol(argv[1], NULL, 10);
  num_N3 = dstrtol(argv[2], NULL, 10);
  s_N3 = dstrtol(argv[3], NULL, 10);
  
  //Injector Code
  while(i < num_N3) {
    //signal increase in number of molecules
    sem_signal(s_N3);
    Printf("An N3 molecule was injected\n");
    i++;
  }

  // // Signal the semaphore to tell the original process that we're done
  // Printf("Injector: PID %d is complete.\n", Getpid());

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
  }
}