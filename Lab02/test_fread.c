#include <stdio.h>
// #include <iostream>
#include <stdlib.h>
// #include <omp.h>
#include <string.h>

int main(){

    char file_name[] = "CSCI4060U_Lab02_data/1.csv";

    printf("%s\n", file_name);
    
    FILE *fp;
    // fp = fopen("CSCI4060U_Lab02_data/1.csv", "r");
    fp = fopen(file_name, "r");

    size_t len = (size_t)50 * 3;
    char *line = (char *) malloc(sizeof(char) * len);
    char *ptr;

    while(getline(&line, &len, fp) != -1){
    // while(fgets(&line, &len, fp) != -1){
        // handle the file read
        char * fname = strtok_r(line, ",", &ptr);
        char * lname = strtok_r(NULL, ",", &ptr);
        printf("first name: %s\n", lname);

        // read in gpa
        // update top_student if necessary
    }

    fclose(fp);


    // FILE *fp;
    // char line[100];
    
    // fp = fopen("CSCI4060U_Lab02_data/1.csv", "r");
    // if (fp == NULL) {
    //     printf("Unable to open file\n");
    //     return 1;
    // }
    
    // while (fgets(line, sizeof(line), fp) != NULL) {
    //     printf("%s", line);
    // }
    
    // fclose(fp);
    // return 0;



    return 0;
}