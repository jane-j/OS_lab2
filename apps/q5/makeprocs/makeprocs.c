#include "lab2-api.h"
#include "usertraps.h"
#include "misc.h"

#include "q5.h"

void main (int argc, char *argv[])
{
  int num_inj = 0, num_reac = 0; //Number of proccesses carrying out injections and reactions
  int num_N3 = 0, num_H2O = 0, num_N = 0, num_O2 = 0;
  sem_t s_procs_completed;        // Semaphore used to wait until all spawned processes have completed
  sem_t s_N3, s_H2O, s_N, s_H2, s_O2, s_NO2;
  char s_procs_completed_str[10]; // Used as command-line argument to pass page_mapped handle to new processes
  char num_N3_str[10], num_H2O_str[10], num_N_str[10], num_O2_str[10];
  char s_N3_str[10], s_H2O_str[10], s_N_str[10], s_H2_str[10], s_O2_str[10], s_NO2_str[10];

  //Usage of current program
  if (argc != 3) {
    Printf("Usage: "); Printf(argv[0]); Printf(" <number of N3 molecules> <number of H2O molecules>\n");
    Exit();
  }

  //Number of proccesses carrying out injections and reactions
  num_inj = 2; num_reac = 3; 
 
  // Original number of N3, H2O molecules to be injected
  num_N3 = dstrtol(argv[1], NULL, 10);
  num_H2O = dstrtol(argv[2], NULL, 10);
  num_N = 3*num_N3;
  num_O2 = num_H2O/2;

  Printf("Number of molecules to be injected: N3: %d, H2O: %d\n", num_N3, num_H2O);

  //Create semaphore to wait for all child proccesses to complete
  if ((s_procs_completed = sem_create(-(num_inj + num_reac - 1))) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  if ((s_N3 = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  if ((s_H2O = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  if ((s_N = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  if ((s_H2 = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  if ((s_O2 = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  if ((s_NO2 = sem_create(0)) == SYNC_FAIL) {
    Printf("Bad sem_create in "); Printf(argv[0]); Printf("\n");
    Exit();
  }

  //Create argument strings for child processes
  ditoa(s_procs_completed, s_procs_completed_str);
  ditoa(s_N3, s_N3_str);
  ditoa(s_H2O, s_H2O_str);
  ditoa(s_H2, s_H2_str);
  ditoa(s_N, s_N_str);
  ditoa(s_O2, s_O2_str);
  ditoa(s_NO2, s_NO2_str);
  ditoa(num_N3, num_N3_str);
  ditoa(num_H2O, num_H2O_str);
  ditoa(num_N, num_N_str);
  ditoa(num_O2, num_O2_str);

  //Create child processes : Injectors and Reactions
  process_create(INJECTOR_1_FILE_TO_RUN, s_procs_completed_str, num_N3_str, s_N3_str, NULL);
  process_create(INJECTOR_2_FILE_TO_RUN, s_procs_completed_str, num_H2O_str, s_H2O_str, NULL);

  process_create(REACTION_1_FILE_TO_RUN, s_procs_completed_str, num_N3_str, s_N3_str, s_N_str, NULL);
  process_create(REACTION_2_FILE_TO_RUN, s_procs_completed_str, num_H2O_str, s_H2O_str, s_H2_str, s_O2_str, NULL);
  process_create(REACTION_3_FILE_TO_RUN, s_procs_completed_str, num_N_str, s_N_str, num_O2_str, s_O2_str, s_NO2_str, NULL);
  

  //Wait until all spawned processes have finished.
  if (sem_wait(s_procs_completed) != SYNC_SUCCESS) {
    Printf("Bad semaphore s_procs_completed (%d) in ", s_procs_completed); Printf(argv[0]); Printf("\n");
    Exit();
  }
  Printf("All other processes completed, exiting main process.\n");
}
