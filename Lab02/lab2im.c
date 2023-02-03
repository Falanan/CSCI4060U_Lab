#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 50
#define MAX_LEN 1000

struct student
{
    char *fname;
    char *lname;
    double gpa;
};

int stdcmp(const void *std1, const void *std2){
    return (*(struct student*) std2).gpa > (*(struct student*) std1).gpa ;
}



void append_stu(struct student* stu_list, struct student* temp_stu, int *size){

    if (*size == 0)
    {
        printf("Size is 0\n");
        stu_list = (struct student*) malloc(sizeof(struct student));
        (*size)++;
    }else{
        // stu_list = (struct student*) realloc(stu_list, ((*size)+1) * sizeof(struct student));
        // stu_list[*size].fname = strdup(temp_stu->fname);
        // stu_list[*size].lname = strdup(temp_stu->lname);
        // stu_list[*size] = *(temp_stu);
        printf("Size is %d\n", *size);
        struct student* new_list = (struct student*)malloc(((*size)+1) * sizeof(struct student));
        printf("1\n");
        for (int i = 0; i < *size; i++)
        {
            printf("2\n");

            new_list[i].fname = strdup(stu_list[i].fname);
            printf("%s\n",stu_list[i].fname);
            new_list[i].lname = strdup(stu_list[i].lname);
            new_list[i].gpa = stu_list[i].gpa;
            printf("3\n");
        }
        printf("4\n");

        new_list[*size].fname = strdup(temp_stu->fname);
        new_list[*size].lname = strdup(temp_stu->lname);
        new_list[*size].gpa = temp_stu->gpa;

        free(stu_list);
        stu_list = new_list;
        (*size)++;
    }
    

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

    // struct student *stu_list = (struct student*) malloc(MAX_LEN * sizeof(struct student));
    struct student *stu_list = NULL;
    struct student *temp_stu = NULL;
    int list_size = 0;
    while(getline(&line, &len, fp) != -1){
        char *fname_t = strtok_r(line, ",", &ptr);
        char *lname_t = strtok_r(NULL, ",", &ptr);
        char *gpa_str = strtok_r(NULL, ",", &ptr);
        double gpa;
        gpa = strtod(gpa_str, NULL);

        // stu_list[list_size].fname = strdup(fname_t);
        // stu_list[list_size].lname = strdup(lname_t);
        // stu_list[list_size].gpa = gpa;
        // list_size++;
        temp_stu = (struct student*) malloc(sizeof(struct student));
        temp_stu->fname = strdup(fname_t);
        temp_stu->lname = strdup(lname_t);
        temp_stu->gpa = gpa;

        append_stu(stu_list, temp_stu, &list_size);

        




    }
    
    printf("Size of the list: %d\n", list_size);

    for (int i=0; i< list_size; i++){
        printf("fname:%s,lname:%s,gpa:%f\n", stu_list[i].fname, stu_list[i].lname, stu_list[i].gpa);
    }

    fclose(fp);

    // free all memory
    for (int i=0; i< list_size; i++){
        free(stu_list[i].fname);
        free(stu_list[i].lname);
    }
    free(stu_list);
    free(line);

    return 0;
}
