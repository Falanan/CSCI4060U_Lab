#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#define MAX_CHAR 50
#define MAX_LEN 1000
#define NUM_THREADS

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

    // omp_set_num_threads(NUM_THREADS);

    

    for (int i=0; i<8; i++){
        char file_name[25];
        sprintf(file_name, "CSCI4060U_Lab02_data/%d.csv", i+1);

        FILE *fp;
        fp = fopen(file_name, "r");

        if (fp == NULL) {
            printf("Unable to open file %d\n", i+1);
            return 1;
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

        qsort(stu_list, list_size, sizeof(struct student), stdcmp);
        strcpy(stu_array[i].fname, stu_list[0].fname);
        strcpy(stu_array[i].lname, stu_list[0].lname);
        stu_array[i].gpa = stu_list[0].gpa;

        fclose(fp);

        free(stu_list);
        free(line);
    }

    for (int i=0; i<8; i++){
        printf("fname:%s,lname:%s,gpa:%f\n", stu_array[i].fname, stu_array[i].lname, stu_array[i].gpa);
    }
    printf("Size of the array: %d\n", 8);
    
    return 0;
}
