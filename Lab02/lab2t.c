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

// // int stdcmp(const void *std1, const void *std2){
// //     return (*(struct student*) std2).gpa > (*(struct student*) std1).gpa ;
// // }

// // int stdcmp(const void *std1, const void *std2){
// //     return (*(struct student*) std1).gpa > (*(struct student*) std2).gpa ;
// // }



// int main(){
//     char file_name[] = "CSCI4060U_Lab02_data/1.csv";

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

//     // qsort(stu_list, list_size, sizeof(struct student), stdcmp);
//     // qsort(stu_list, list_size, sizeof(struct student), stdcmp);
//     for (int i=0; i< list_size; i++){
//         printf("fname:%s,lname:%s,gpa:%f\n", stu_list[i].fname, stu_list[i].lname, stu_list[i].gpa);
//     }
//     printf("Size of the list: %d\n", list_size);


    

//     fclose(fp);

//     free(stu_list);
//     free(line);

//     return 0;
// }


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
    char file_name[] = "CSCI4060U_Lab02_data/1.csv";

    FILE *fp;
    fp = fopen(file_name, "r");

    if (fp == NULL) {
        printf("Unable to open file\n");
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
    for (int i=0; i< list_size; i++){
        printf("fname:%s,lname:%s,gpa:%f\n", stu_list[i].fname, stu_list[i].lname, stu_list[i].gpa);
    }
    printf("Size of the list: %d\n", list_size);

    fclose(fp);

    free(stu_list);
    free(line);

   
}