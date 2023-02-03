#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define MAX_CHAR 50
#define MAX_LEN 1000
#define NUM_THREADS 8

struct student
{
    char fname[MAX_CHAR];
    char lname[MAX_CHAR];
    double gpa;
};

int stdcmp(const void *std1, const void *std2){
    return (*(struct student*) std2).gpa > (*(struct student*) std1).gpa ;
}

int main(){
    struct student stu_array[8];

    omp_set_num_threads(NUM_THREADS);

    printf("Number of thread: %d\n", NUM_THREADS);

    #pragma omp parallel
    {
        for (int i=0; i<8; i++){
            int threadID = omp_get_thread_num();
            printf("Thread: %d processing file %d.\n", threadID, i+1);

            char file_name[25];
            sprintf(file_name, "CSCI4060U_Lab02_data/%d.csv", i+1);

            FILE *fp;
            fp = fopen(file_name, "r");

            if (fp == NULL) {
                printf("Unable to open file %d\n", i+1);
                exit(0);
            }

            size_t len = (size_t)MAX_CHAR * 3;
            char *line = (char *) malloc(sizeof(char) * len);
            char *ptr;

            struct student *stu_list = (struct student*) malloc(MAX_LEN * sizeof(struct student));
            int list_size = 0;
            while(getline(&line, &len, fp) != -1){
                char *fname_t = strtok_r(line, ",", &ptr);
                char *lname_t = strtok_r(NULL, ",", &ptr);
                char *gpa_str = strtok_r(NULL, ",", &ptr);
                double gpa;
                gpa = strtod(gpa_str, NULL);

                strcpy(stu_list[list_size].fname, fname_t);
                strcpy(stu_list[list_size].lname, lname_t);
                stu_list[list_size].gpa = gpa;
                list_size++;
            }

            #pragma omp barrier
            qsort(stu_list, list_size, sizeof(struct student), stdcmp);

            #pragma omp critical
            {
                strcpy(stu_array[i].fname, stu_list[0].fname);
                strcpy(stu_array[i].lname, stu_list[0].lname);
                stu_array[i].gpa = stu_list[0].gpa;
            }

            

            fclose(fp);

            free(stu_list);
            free(line);
        }
    }

    

    for (int i=0; i<8; i++){
        printf("fname:%s,lname:%s,gpa:%f\n", stu_array[i].fname, stu_array[i].lname, stu_array[i].gpa);
    }
    printf("Size of the array: %d\n", 8);
    
    return 0;
}


/*
Number of thread: 1
Thread: 0 processing file 1.
Thread: 0 processing file 2.
Thread: 0 processing file 3.
Thread: 0 processing file 4.
Thread: 0 processing file 5.
Thread: 0 processing file 6.
Thread: 0 processing file 7.
Thread: 0 processing file 8.
fname:Juan,lname:Little,gpa:4.290000
fname:Janice,lname:Powell,gpa:4.230000
fname:Peter,lname:Chapman,gpa:4.290000
fname:Joe,lname:Dixon,gpa:4.300000
fname:Margaret,lname:Carr,gpa:4.280000
fname:Gerald,lname:Cunningham,gpa:4.130000
fname:Charles,lname:Austin,gpa:3.230000
fname:Aaron,lname:Moreno,gpa:4.290000
Size of the array: 8

real    0m0.032s
user    0m0.000s
sys     0m0.008s


Number of thread: 2
Thread: 0 processing file 1.
Thread: 1 processing file 1.
Thread: 1 processing file 2.
Thread: 0 processing file 2.
Thread: 0 processing file 3.
Thread: 1 processing file 3.
Thread: 1 processing file 4.
Thread: 0 processing file 4.
Thread: 1 processing file 5.
Thread: 0 processing file 5.
Thread: 0 processing file 6.
Thread: 1 processing file 6.
Thread: 0 processing file 7.
Thread: 1 processing file 7.
Thread: 1 processing file 8.
Thread: 0 processing file 8.
fname:Juan,lname:Little,gpa:4.290000
fname:Janice,lname:Powell,gpa:4.230000
fname:Peter,lname:Chapman,gpa:4.290000
fname:Joe,lname:Dixon,gpa:4.300000
fname:Margaret,lname:Carr,gpa:4.280000
fname:Gerald,lname:Cunningham,gpa:4.130000
fname:Charles,lname:Austin,gpa:3.230000
fname:Aaron,lname:Moreno,gpa:4.290000
Size of the array: 8

real    0m0.032s
user    0m0.017s
sys     0m0.000s




Number of thread: 4
Thread: 0 processing file 1.
Thread: 1 processing file 1.
Thread: 3 processing file 1.
Thread: 2 processing file 1.
Thread: 0 processing file 2.
Thread: 2 processing file 2.
Thread: 1 processing file 2.
Thread: 3 processing file 2.
Thread: 2 processing file 3.
Thread: 1 processing file 3.
Thread: 0 processing file 3.
Thread: 3 processing file 3.
Thread: 0 processing file 4.
Thread: 2 processing file 4.
Thread: 1 processing file 4.
Thread: 3 processing file 4.
Thread: 3 processing file 5.
Thread: 1 processing file 5.
Thread: 2 processing file 5.
Thread: 0 processing file 5.
Thread: 2 processing file 6.
Thread: 0 processing file 6.
Thread: 3 processing file 6.
Thread: 1 processing file 6.
Thread: 2 processing file 7.
Thread: 0 processing file 7.
Thread: 3 processing file 7.
Thread: 1 processing file 7.
Thread: 3 processing file 8.
Thread: 0 processing file 8.
Thread: 1 processing file 8.
Thread: 2 processing file 8.
fname:Juan,lname:Little,gpa:4.290000
fname:Janice,lname:Powell,gpa:4.230000
fname:Peter,lname:Chapman,gpa:4.290000
fname:Joe,lname:Dixon,gpa:4.300000
fname:Margaret,lname:Carr,gpa:4.280000
fname:Gerald,lname:Cunningham,gpa:4.130000
fname:Charles,lname:Austin,gpa:3.230000
fname:Aaron,lname:Moreno,gpa:4.290000
Size of the array: 8

real    0m0.044s
user    0m0.043s
sys     0m0.000s

Number of thread: 8
Thread: 5 processing file 1.
Thread: 1 processing file 1.
Thread: 7 processing file 1.
Thread: 4 processing file 1.
Thread: 6 processing file 1.
Thread: 3 processing file 1.
Thread: 0 processing file 1.
Thread: 2 processing file 1.
Thread: 6 processing file 2.
Thread: 5 processing file 2.
Thread: 1 processing file 2.
Thread: 2 processing file 2.
Thread: 3 processing file 2.
Thread: 7 processing file 2.
Thread: 4 processing file 2.
Thread: 0 processing file 2.
Thread: 4 processing file 3.
Thread: 6 processing file 3.
Thread: 7 processing file 3.
Thread: 3 processing file 3.
Thread: 2 processing file 3.
Thread: 5 processing file 3.
Thread: 0 processing file 3.
Thread: 1 processing file 3.
Thread: 3 processing file 4.
Thread: 2 processing file 4.
Thread: 0 processing file 4.
Thread: 4 processing file 4.
Thread: 5 processing file 4.
Thread: 7 processing file 4.
Thread: 6 processing file 4.
Thread: 1 processing file 4.
Thread: 7 processing file 5.
Thread: 3 processing file 5.
Thread: 4 processing file 5.
Thread: 5 processing file 5.
Thread: 6 processing file 5.
Thread: 2 processing file 5.
Thread: 1 processing file 5.
Thread: 0 processing file 5.
Thread: 2 processing file 6.
Thread: 1 processing file 6.
Thread: 3 processing file 6.
Thread: 0 processing file 6.
Thread: 6 processing file 6.
Thread: 4 processing file 6.
Thread: 5 processing file 6.
Thread: 7 processing file 6.
Thread: 0 processing file 7.
Thread: 5 processing file 7.
Thread: 1 processing file 7.
Thread: 3 processing file 7.
Thread: 2 processing file 7.
Thread: 7 processing file 7.
Thread: 6 processing file 7.
Thread: 4 processing file 7.
Thread: 4 processing file 8.
Thread: 1 processing file 8.
Thread: 7 processing file 8.
Thread: 3 processing file 8.
Thread: 0 processing file 8.
Thread: 5 processing file 8.
Thread: 6 processing file 8.
Thread: 2 processing file 8.
fname:Juan,lname:Little,gpa:4.290000
fname:Janice,lname:Powell,gpa:4.230000
fname:Peter,lname:Chapman,gpa:4.290000
fname:Joe,lname:Dixon,gpa:4.300000
fname:Margaret,lname:Carr,gpa:4.280000
fname:Gerald,lname:Cunningham,gpa:4.130000
fname:Charles,lname:Austin,gpa:3.230000
fname:Aaron,lname:Moreno,gpa:4.290000
Size of the array: 8

real    0m0.083s
user    0m0.157s
sys     0m0.049s


*/