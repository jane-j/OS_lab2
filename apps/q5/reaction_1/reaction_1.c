#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q5.h"

void main (int argc, char *argv[])
{      
  sem_t s_procs_completed;
  sem_t s_N3, s_N;
  int num_N3; //Original number of N3 injected
  int i = 0;

  //Usage of current program
  if (argc != 5) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_page_mapped_semaphore> <handle_to_num_N3> <handle_to_sem_N3> <handle_to_sem_N>\n"); 
    Exit();
  } 

  // Convert the command-line strings into integers for use as handles
  s_procs_completed = dstrtol(argv[1], NULL, 10);
  num_N3 = dstrtol(argv[2], NULL, 10);
  s_N3 = dstrtol(argv[3], NULL, 10);
  s_N = dstrtol(argv[4], NULL, 10);
  
  //Reaction_1 Code
  //Number of reactions same as number of N3 molecules originally injected
  while(i < num_N3) {
    //Wait for number of N3 molecules to be non-zero
    sem_wait(s_N3); 

    //Signal sem_N 3 times
    sem_signal(s_N);
    Printf("An N molecule is created\n");
    sem_signal(s_N);
    Printf("An N molecule is created\n");
    sem_signal(s_N);
    Printf("An N molecule is created\n");

    i++;
  }
  
  // Signal the semaphore to tell the original process that we're done
  // Printf("Reaction_1: PID %d is complete.\n", Getpid());

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
    
  }
}