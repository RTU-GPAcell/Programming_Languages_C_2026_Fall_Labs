// week5_task2_struct_save_load.c
// Name: Arda  Surname: Kocaman  Student ID: 251ADB187
// Week 5, Task 2: save/load one struct as text.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
    char name[MAX_NAME_LEN];
    int age;
    float gpa;
} Student;

int save_student(const Student *s, const char *filename);
int load_student(const char *filename, Student *out);

int main(void) {
    Student s1 = {0};
    strcpy(s1.name, "Alice");
    s1.age = 21;
    s1.gpa = 3.75f;

    Student loaded = {0};
    const char *filename = "student.txt";

    printf("Saving student to file...\n");
    if (save_student(&s1, filename) != 0) {
        fprintf(stderr, "Failed to save student.\n");
        return EXIT_FAILURE;
    }

    printf("Loading student from file...\n");
    if (load_student(filename, &loaded) != 0) {
        fprintf(stderr, "Failed to load student.\n");
        return EXIT_FAILURE;
    }

    printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age, loaded.gpa);
    return EXIT_SUCCESS;
}

int save_student(const Student *s, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) return -1;

    int written = fprintf(fp, "%s %d %.2f\n", s->name, s->age, s->gpa);
    int closed = fclose(fp);
    return (written >= 0 && closed == 0) ? 0 : -1;
}

int load_student(const char *filename, Student *out) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return -1;

    // Width-limited scan protects the 50-byte name array.
    int scanned = fscanf(fp, "%49s %d %f", out->name, &out->age, &out->gpa);
    int closed = fclose(fp);
    return (scanned == 3 && closed == 0) ? 0 : -1;
}
