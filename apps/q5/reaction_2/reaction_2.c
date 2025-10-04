#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q5.h"

void main (int argc, char *argv[])
{      
  sem_t s_procs_completed;
  sem_t s_H2O, s_H2, s_O2;
  int num_H2O; //Original number of H2O injected
  int i = 0;

  //Usage of current program
  if (argc != 6) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_page_mapped_semaphore> <handle_to_num_H2O> <handle_to_sem_H2O> <handle_to_sem_H2> <handle_to_sem_O2>\n"); 
    Exit();
  } 

  // Convert the command-line strings into integers for use as handles
  s_procs_completed = dstrtol(argv[1], NULL, 10);
  num_H2O = dstrtol(argv[2], NULL, 10);
  s_H2O = dstrtol(argv[3], NULL, 10);
  s_H2 = dstrtol(argv[4], NULL, 10);
  s_O2 = dstrtol(argv[5], NULL, 10);
  
  //Reaction_2 Code
  //Number of reactions is equal to half the number of H2O molecules originally injected
  while(i < num_H2O/2) {
    //Wait for number of H2O molecules to be atleast 2
    sem_wait(s_H2O);
    sem_wait(s_H2O); 

    //Signal sem_H2 2 times
    sem_signal(s_H2);
    Printf("An H2 molecule is created\n");
    sem_signal(s_H2);
    Printf("An H2 molecule is created\n");

    //Signal sem_O2
    sem_signal(s_O2);
    Printf("An O2 molecule is created\n");

    i++;
  }
  
  // Signal the semaphore to tell the original process that we're done
  // Printf("Reaction_2: PID %d is complete.\n", Getpid());

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
    
  }
}