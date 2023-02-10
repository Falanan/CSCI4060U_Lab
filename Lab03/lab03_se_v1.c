#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <math.h>

#define ARRAY_LEN 10
#define NUM_THREADS 1


void decimalToDMS(double decimal, int *d, int *m, int *s, char *dir) {
    *dir = (decimal < 0) ? 'S' : 'N';

    decimal = fabs(decimal);
    *d = (int)decimal;
    *m = (int)((decimal - *d) * 60);
    *s = (int)((((decimal - *d) * 60) - *m) * 60);
}

int enqueue(double *latitude, double num, int *size){
    // return 0 means the array is full and cannot add value into it
    // return 1 means success

    if (*size == ARRAY_LEN)
    {
        return 0;
    }else
    {
        latitude[*size] = num;
        *size += 1;
        return 1;
    }
}

double dequeue(double *latitude, int *size){
    double temp;
    if (*size == 0)
    {
        return -999;
    }else
    {
        temp = latitude[0];
        if (*size != 1)
        {
            for (int i = 0; i < *size; i++)
            {
                latitude[i] = latitude[i+1];
            }
        }
        *size -= 1;
    }
    return temp;
}




int main(){

    double latitude[ARRAY_LEN];
    int la_size = 0;
    double longitude[ARRAY_LEN];
    int lo_size = 0;

    char latitude_filename[14] = "latitude_t.csv";
    // char longitude_filename[13] = "longitude.csv";

    // printf("%s\n", longitude_filename);

    FILE *fp;
    fp = fopen(latitude_filename, "r");
    size_t len = (size_t)15;
    char *line = (char *) malloc(sizeof(char) * len);

    if (fp == NULL) {
        printf("Unable to open file\n");
        exit(0);
    }


    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        while(getline(&line, &len, fp) != -1){
            double la = strtod(line, NULL);
            // printf("%f \n", la);

            #pragma omp critical
            {
                enqueue(latitude, la, &la_size);
                printf("%d Thread Enqueue: %f\n", thread_id, la);
                // for (int i = 0; i < la_size; i++)
                // {
                //     printf("%f ", latitude[i]);
                // }
                // printf("\n");
                
            }
            #pragma omp critical
            {
                double de = dequeue(latitude, &la_size);
                printf("%d Thread Dequeue: %f\n", thread_id, de);
                int d, m, s;
                char dir;
                
                decimalToDMS(de, &d, &m, &s, &dir);
                printf("%.4f° converted to %d° %d' %d'' %c\n", de, d, m, s, dir);
                // for (int i = 0; i < la_size; i++)
                // {
                //     printf("%f ", latitude[i]);
                // }
                printf("\n");
            } 
        }
    }
    
    fclose(fp);
    printf("The file is closed\n");
    if (fp == NULL) {
        printf("Unable to open file\n");
        exit(0);
    }


    return 0;
}