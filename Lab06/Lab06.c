// MacOS native environment
// ./a.out 5 100 10  0.00s user 0.00s system 107% cpu 0.006 total
// ./a.out 5 2500 25  0.00s user 0.03s system 41% cpu 0.071 total
// ./a.out 50 2500 25  0.01s user 0.06s system 87% cpu 0.074 total

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int num_agents;
int num_seats;
int overselling_percentage;
int total_num_tickets;
int tickets_sold = 0;
pthread_mutex_t ts_mutex;
// srand(time(NULL));


struct thread_data {
    int id;
};

void *ticket_agent(void *agent_data);

int main(int argc, char *argv[]) {

    num_agents = atoi(argv[1]);
    num_seats = atoi(argv[2]);
    overselling_percentage = atoi(argv[3]);

    total_num_tickets = num_seats + (num_seats * overselling_percentage / 100);

    pthread_t agents[num_agents];

    srand(time(NULL));

    for (int i = 0; i < num_agents; i++) {
        struct thread_data *data = malloc(sizeof(struct thread_data));
        data->id = i + 1;
        int error = pthread_create(&agents[i], NULL, ticket_agent, data);
        if (error)
        {
            printf("Failed to create thread NUM %d", i+1);
            exit(-1);
        }
        
    }

    for (int i = 0; i < num_agents; i++) {
        int error = pthread_join(agents[i], NULL);
        if (error)
        {
            printf("Error to join thread NUM %d", i+1);
            exit(-1);
        }
        
    }

    printf("Total tickets sold: %d/%d\n", tickets_sold, total_num_tickets);
    printf("Percentage of tickets sold: %.2f%%\n", (float)tickets_sold / total_num_tickets * 100);

    return 0;
}


void *ticket_agent(void *agent_data) {
    struct thread_data *agent_data_ptr = (struct thread_data *) agent_data;

    while (tickets_sold < total_num_tickets) {

        int success_rate;
        if (agent_data_ptr->id % 2 == 1)
        {
            success_rate = 30;
        }else{
            success_rate = 45;
        }
        

        if (rand() % 100 < success_rate) {
            // Transaction successed 
            int num_tickets = rand() % 4 + 1;
            pthread_mutex_lock(&ts_mutex);
            
            // Check whether the number of tickets will sell exceeds the total number of tickets.
            // if exceed the total number of tickets, generate the random number again till meet the requirement
            while (1)
            {
                if (tickets_sold + num_tickets > total_num_tickets) {
                    num_tickets = rand() % 4 + 1;
                }else{
                    break;
                }
                
            }

            tickets_sold += num_tickets;
            pthread_mutex_unlock(&ts_mutex);
            printf("Ticket agent %d: Successful transaction - %d tickets sold\n", agent_data_ptr->id, num_tickets);
        } else {
            // Transaction failed
            printf("Ticket agent %d: Unsuccessful transaction\n", agent_data_ptr->id);
        }
    }

    free(agent_data_ptr);
    pthread_exit(NULL);
}
