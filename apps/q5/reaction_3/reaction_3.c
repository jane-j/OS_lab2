#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q5.h"

void main (int argc, char *argv[])
{      
  sem_t s_procs_completed;
  sem_t s_N, s_O2, s_NO2;
  int num_N, num_O2; //Total number of expected N, O2 to be consumed in the process
  int num_reactions = 0; //Total number of reactions based on num of N and O2
  int i = 0;

  //Usage of current program
  if (argc != 7) { 
    Printf("Usage: "); Printf(argv[0]); Printf(" <handle_to_page_mapped_semaphore> <handle_to_num_N3> <handle_to_sem_N3> <handle_to_sem_N>\n"); 
    Exit();
  } 

  // Convert the command-line strings into integers for use as handles
  s_procs_completed = dstrtol(argv[1], NULL, 10);
  num_N = dstrtol(argv[2], NULL, 10);
  s_N = dstrtol(argv[3], NULL, 10);
  num_O2 = dstrtol(argv[4], NULL, 10);
  s_O2 = dstrtol(argv[5], NULL, 10);
  s_NO2 = dstrtol(argv[6], NULL, 10);

  if(num_N < num_O2) num_reactions = num_N;
  else num_reactions = num_O2;
  
  //Reaction_3 Code
  //Number of rections is the minimum of the number of N and O2 molecules generated
  while(i < num_reactions) {
    //Wait for number of N atoms to be non-zero 
    sem_wait(s_N);
    //Wait for number of O2 molecules to be non-zero 
    sem_wait(s_O2); 

    //Signal sem_NO2
    sem_signal(s_NO2);
    Printf("An NO2 molecule is created\n");

    i++;
  }
  
  // Signal the semaphore to tell the original process that we're done
  // Printf("Reaction_3: PID %d is complete.\n", Getpid());

  if(sem_signal(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf(", exiting...\n");
    Exit();
    
  }
}