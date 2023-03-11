// ./a.out  0.20s user 0.91s system 88% cpu 1.261 total



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 100000


// In this function, just declear a variable and give the variable a value
void* do_nothing(void* arg) {
    int i;
    i= 0;
}

// int main() {
//     // void *status; // used to join

//     // Create a 100000 threads pool
//     pthread_t threads[NTHREADS];

//     // Initialize the threads
//     for (int i = 0; i < NTHREADS; i++) {
//         // creat the thread and let the thread call do_nothing function
//          int error = pthread_create(&threads[i], NULL, do_nothing, NULL);
//          // if failed to create the threads, exit
//          if (error){
//             printf("ERROR: create\n");
//             exit(-1);
//         }
//     }

//     // Wait for all threads to finish
//     for (int i = 0; i < NTHREADS; i++) {
//         // int error = pthread_join(threads[i], &status);

//         // the main thread keep waiting all child threads finished using pthread_join() function
//         int error = pthread_join(threads[i], NULL);
//         // If failed to join
//         if (error){
//             printf("ERROR: Join\n");
//             // exit(-1);
//         }
//     }

//     return 0;
// }


int main() {
    // void *status; // used to join
    
    // Create a 100000 threads pool
    pthread_t threads[NTHREADS];

    // Initialize the threads
    for (int i = 0; i < NTHREADS; i++) {
        // creat the thread and let the thread call do_nothing function
         int error = pthread_create(&threads[i], NULL, do_nothing, NULL);
         // if failed to create the threads, exit
         if (error){
            printf("ERROR: create\n");
            exit(-1);
        }

        // wait till the child thread finish the job
        error = pthread_join(threads[i], NULL);
        // If failed to join
        if (error){
            printf("ERROR: Join\n");
            // exit(-1);
        }

    }

    return 0;
}


