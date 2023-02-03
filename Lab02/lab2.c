#include <stdio.h>
// #include <iostream>
#include <stdlib.h>
// #include <omp.h>
#include <string.h>

#define MAX_CHAR 50
#define MAX_LEN 1000

#define NUM_THREADS 1

struct student
{
    char *fname;
    char *lname;
    double gpa;
};

int stdcmp(const void *std1, const void *std2){
    // return std2.gpa > std1.gpa
    return (*(struct student*) std2).gpa > (*(struct student*) std1).gpa ;
}



int main(){
    char file_name[] = "CSCI4060U_Lab02_data/1.csv";
    int num_files = 8;

    // struct student *top_students;
    // top_students = (struct student *)malloc(sizeof(struct student) * 12);

    FILE *fp;
    fp = fopen(file_name, "r");

    if (fp == NULL) {
        printf("Unable to open file\n");
        return 1;
    }

    size_t len = (size_t)MAX_CHAR * 3;
    char *line = (char *) malloc(sizeof(char) * len);
    char *ptr;


    // struct student *stu_list = NULL;
    struct student *stu_list = (struct student*) malloc(MAX_LEN * sizeof(struct student));
    // struct student *temp_stu = NULL;
    int list_size = 0;
    // handle the file read
    while(getline(&line, &len, fp) != -1){
    // while(fgets(&line, &len, fp) != -1){
        // read in a line in the file, then seperate then by ","
        char *fname_t = strtok_r(line, ",", &ptr);
        char *lname_t = strtok_r(NULL, ",", &ptr);
        char *gpa_str = strtok_r(NULL, ",", &ptr);
        double gpa;
        gpa = strtod(gpa_str, NULL);
        stu_list[list_size].fname = fname_t;
        stu_list[list_size].lname = lname_t;
        stu_list[list_size].gpa = gpa;
        list_size++;

  
        // stu_list[list_size].fname = strtok_r(line, ",", &ptr);
        // stu_list[list_size].lname = strtok_r(NULL, ",", &ptr);
        // char *gpa_str = strtok_r(NULL, ",", &ptr);
        // double gpa;
        // gpa = strtod(gpa_str, NULL);
        // stu_list[list_size].gpa = gpa;
        // list_size++;

    }
    

    printf("Size of the list: %d\n", list_size);

    for (int i=0; i< list_size; i++){
        printf("fname:%s,lname:%s,gpa:%f\n", stu_list[i].fname, stu_list[i].lname, stu_list[i].gpa);
    }
    printf("fname:%s,lname:%s,gpa:%f\n", stu_list[0].fname, stu_list[0].lname, stu_list[0].gpa);
    fclose(fp);
    // sort list
    // qsort(top_students, num_files, sizeof(struct student), stdcmp);
    // display list of students

    // free(top_students);

    return 0;
}