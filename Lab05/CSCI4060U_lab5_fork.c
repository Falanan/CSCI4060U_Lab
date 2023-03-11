// ./a.out  1.80s user 16.71s system 80% cpu 23.114 total
// https://man7.org/linux/man-pages/man7/pthreads.7.html

//==============================================================================
//C Code for fork() creation test
//==============================================================================

// This Library is used for print the message
#include <stdio.h>
// This Library provides the exit() function
#include <stdlib.h>
// To use fork() function, need to include this function
#include <unistd.h>
// Probably provide the waitpid() function, if i comment this library, the program still work
#include <sys/types.h>


// define 100000 threads
#define NFORKS 100000

// In this function, just declear a variable and give the variable a value
void do_nothing() {
  int i;
  i= 0;
}

int main(int argc, char *argv[]) {
  int pid, j, status;
  pid_t processID;

  // Create 100000 child threads using for loop
  for (j=0; j<NFORKS; j++) {
    // Using fork() function to create a child thread, the fork() function returns a value
    // If the value != 0 means failed to create a child thread
    // If the value == 0 means succesfully created the child thread
    // If failed to create the threaad
    if ((processID = fork()) < 0 ) {
      printf ("fork failed with error code= %d\n", processID);
      exit(0);
    }
    // If successfully created the child thread, call do_nothing thread, then exit the function
    else if (processID ==0) {
      do_nothing();
      exit(0);
    }
    else { // The main thread keep waiting till the child thread terminated
      waitpid(processID, &status, 0);
    }
  }
}



