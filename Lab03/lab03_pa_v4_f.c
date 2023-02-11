#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <omp.h>
#include <math.h>

#define ARRAY_LEN 10
#define NUM_THREADS 4

void laDecimalToDMS(double decimal, int *d, int *m, int *s, char *dir) {
    *dir = (decimal < 0) ? 'S' : 'N';

    decimal = fabs(decimal);
    *d = (int)decimal;
    *m = (int)((decimal - *d) * 60);
    *s = (int)((((decimal - *d) * 60) - *m) * 60);
}



void loDecimalToDMS(double decimal, int *d, int *m, double *s, char *dir) {
    *dir = (decimal < 0) ? 'W' : 'E';

    *d = (int) decimal;
    double remainder = fabs(decimal - *d);
    *m = (int) (remainder * 60);
    *s = (remainder * 3600 - *m * 60);
}





int enqueue(double *array, double num, int *size){
    // return 0 means the array is full and cannot add value into it
    // return 1 means success

    if (*size == ARRAY_LEN)
    {
        return 0;
    }else
    {
        array[*size] = num;
        *size += 1;
        return 1;
    }
}

double dequeue(double *array, int *size){
    double temp;
    if (*size == 0)
    {
        return -999.0;
    }else
    {
        temp = array[0];
        if (*size != 1)
        {
            for (int i = 0; i < *size; i++)
            {
                array[i] = array[i+1];
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

    // char latitude_filename[15] = "latitude_t.csv";
    // char longitude_filename[17] = "longitude_t.csv";
    char latitude_filename[13] = "latitude.csv";
    char longitude_filename[14] = "longitude.csv";

    FILE *fp_la, *fp_lo;
    fp_la = fopen(latitude_filename, "r");
    fp_lo = fopen(longitude_filename, "r");
    size_t len = (size_t)15;
    // char *line = (char *) malloc(sizeof(char) * len);

    if (fp_la == NULL | fp_lo == NULL) {
        printf("Unable to open file\n");
        exit(0);
    }
    // if (fp_la == NULL) {
    //     printf("Unable to open file\n");
    //     exit(0);
    // }

    // flag for end of file
    int eof_la = 0;
    int eof_lo = 0;
    // Parallel region
    omp_set_num_threads(NUM_THREADS);
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

        if (thread_id == 0)
        {
            int la_status = 0;
            while(!eof_la){
                char *line = (char *) malloc(sizeof(char) * len);
                // char *line;
                la_status = getline(&line, &len, fp_la);
                if (la_status == -1)
                {
                    // la_file_result = fclose(fp_la);
                    fclose(fp_la);
                    #pragma omp atomic write
                    eof_la = 1;
                    break;
                }
                
                double la = strtod(line, NULL);
                #pragma omp critical (critical_section1)
                {
                    int status = 0;
                    while (status == 0)
                    {
                        status = enqueue(latitude, la, &la_size);
                    }
                    // printf("Thread %d Enqueue: %f\n",thread_id, la);
                }
                free(line);
            }
        }else if (thread_id == 1)
        {
            while (!eof_la || la_size != 0)
            {
                if (la_size != 0)
                {
                    #pragma omp critical (critical_section2)
                    {
                        double de = dequeue(latitude, &la_size);
                        if (de == -999.0){
                            de = dequeue(latitude, &la_size);
                        }
                        
                        int d, m, s;
                        char dir;
                        
                        laDecimalToDMS(de, &d, &m, &s, &dir);
                        // printf("Thread %d ",thread_id);
                        #pragma omp flush
                        printf("Thread %d, %.4f° converted to %d° %d' %d'' %c\n",thread_id, de, d, m, s, dir);
                    }
                    
                } 
                
            }
        }else if (thread_id == 2)
        {
            int lo_status = 0;
            while(!eof_lo){
                char *line = (char *) malloc(sizeof(char) * len);
                // char *line;
                lo_status = getline(&line, &len, fp_lo);
                if (lo_status == -1)
                {
                    // la_file_result = fclose(fp_lo);
                    fclose(fp_lo);
                    #pragma omp atomic write
                    eof_lo = 1;
                    break;
                }
                
                double lo = strtod(line, NULL);
                #pragma omp critical (critical_section3)
                {
                    int status = 0;
                    while (status == 0)
                    {
                        status = enqueue(longitude, lo, &lo_size);
                    }
                    // printf("Thread %d Enqueue: %f\n",thread_id, la);
                }
                free(line);
            }
        }else if (thread_id == 3)
        {
            while (!eof_lo || lo_size != 0)
            {
                if (lo_size != 0)
                {
                    #pragma omp critical (critical_section4)
                    {
                        double de = dequeue(longitude, &lo_size);
                        if (de == -999.0)
                        {
                            de = dequeue(longitude, &lo_size);
                        }
                        
                        // int d, m, s;
                        int d, m;
                        double s;
                        char dir;
                        
                        loDecimalToDMS(de, &d, &m, &s, &dir);
                        #pragma omp flush
                        printf("Thread %d %.4f° converted to %d° %d' %f'' %c\n",thread_id, de, d, m, s, dir);
                    }
                    
                } 
                
            }

        }
        
        

    }// end parallel tag
    return 0;
}


// wenbo@DESKTOP-AK0LUOF:/mnt/e/OntarioTech/CSCI4060U_Lab/Lab03$ time ./a.out | wc
//    2000   18000  104126

// real    0m0.039s
// user    0m0.070s
// sys     0m0.024s
