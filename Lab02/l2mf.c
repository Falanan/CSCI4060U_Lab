// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_CHAR 50
// #define MAX_LEN 1000

// struct student
// {
//     char fname[MAX_CHAR];
//     char lname[MAX_CHAR];
//     double gpa;
// };

// int stdcmp(const void *std1, const void *std2){
//     return (*(struct student*) std2).gpa > (*(struct student*) std1).gpa ;
// }

// int main(){
//     char file_prefix[] = "CSCI4060U_Lab02_data/";
//     char file_suffix[] = ".csv";
//     int file_count = 8;
//     struct student highest_stu_list[file_count];

// for(int i=0; i<file_count; i++){
//     char file_name[MAX_CHAR];
//     snprintf(file_name, MAX_CHAR, "%s%d%s", file_prefix, i+1, file_suffix);

//     FILE *fp;
//     fp = fopen(file_name, "r");

//     if (fp == NULL) {
//         printf("Unable to open file\n");
//         return 1;
//     }

//     size_t len = (size_t)MAX_CHAR * 3;
//     char *line = (char *) malloc(sizeof(char) * len);
//     char *ptr;

//     struct student *stu_list = (struct student*) malloc(MAX_LEN * sizeof(struct student));
//     int list_size = 0;
//     while(getline(&line, &len, fp) != -1){
//         char *fname_t = strtok_r(line, ",", &ptr);
//         char *lname_t = strtok_r(NULL, ",", &ptr);
//         char *gpa_str = strtok_r(NULL, ",", &ptr);
//         double gpa;
//         gpa = strtod(gpa_str, NULL);

//         strcpy(stu_list[list_size].fname, fname_t);
//         strcpy(stu_list[list_size].lname, lname_t);
//         stu_list[list_size].gpa = gpa;
//         list_size++;
//     }

//     qsort(stu_list, list_size, sizeof(struct student), stdcmp);
//     strcpy(highest_stu_list[i].fname, stu_list[0].fname);
//     strcpy(highest_stu_list[i].lname, stu_list[0].lname);
//     highest_stu_list[i].gpa = stu_list[0].gpa;

//     fclose(fp);

//     free(stu_list);
//     free(line);
// }

// printf("\nThe highest GPA students from each file are:\n");
// for(int i=


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// #define MAX_CHAR 50
// #define MAX_LEN 1000

// struct student
// {
//     char fname[MAX_CHAR];
//     char lname[MAX_CHAR];
//     double gpa;
// };

// int stdcmp(const void *std1, const void *std2){
//     return (*(struct student*) std2).gpa > (*(struct student*) std1).gpa ;
// }

// int main(){
//     char base_file_name[] = "CSCI4060U_Lab02_data/";
//     struct student highest_gpa_stu[8];

//     for (int i = 1; i <= 8; i++) {
//         char file_name[30];
//         sprintf(file_name, "%s%d.csv", base_file_name, i);

//         FILE *fp;
//         fp = fopen(file_name, "r");

//         if (fp == NULL) {
//             printf("Unable to open file\n");
//             return 1;
//         }

//         size_t len = (size_t)MAX_CHAR * 3;
//         char *line = (char *) malloc(sizeof(char) * len);
//         char *ptr;

//         struct student *stu_list = (struct student*) malloc(MAX_LEN * sizeof(struct student));
//         int list_size = 0;
//         while(getline(&line, &len, fp) != -1){
//             char *fname_t = strtok_r(line, ",", &ptr);
//             char *lname_t = strtok_r(NULL, ",", &ptr);
//             char *gpa_str = strtok_r(NULL, ",", &ptr);
//             double gpa;
//             gpa = strtod(gpa_str, NULL);

//             strcpy(stu_list[list_size].fname, fname_t);
//             strcpy(stu_list[list_size].lname, lname_t);
//             stu_list[list_size].gpa = gpa;
//             list_size++;
//         }

//         qsort(stu_list, list_size, sizeof(struct student), stdcmp);
//         highest_gpa_stu[i-1] = stu_list[0];

//         fclose(fp);

//         free(stu_list);
//         free(line);
//     }

//     for (int i = 0; i < 8; i++) {
//         printf("File %d highest gpa student: fname: %s, lname: %s, gpa: %f\n", i +1, 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 50
#define MAX_LEN 1000

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
}
