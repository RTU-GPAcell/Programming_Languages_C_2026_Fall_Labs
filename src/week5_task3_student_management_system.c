// week5_task3_student_management_system.c
// Name: Arda  Surname: Kocaman  Student ID: 251ADB187
// Week 5, Task 3: persistent in-memory student database.

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
    char name[NAME_LEN];
    int id;
    float gpa;
} Student;

int load_students(Student arr[], int max);
int save_students(const Student arr[], int count);
void add_student(Student arr[], int *count);
void list_students(const Student arr[], int count);
int read_line(const char *prompt, char *buf, size_t size);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    char input[LINE_LEN];
    int running = 1;

    count = load_students(students, MAX_STUDENTS);
    printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

    while (running) {
        printf("\n=== Student Management System ===\n");
        printf("1. List students\n");
        printf("2. Add student\n");
        printf("3. Save and Exit\n");

        int choice;
        if (read_line("Select an option: ", input, sizeof input) != 0) {
            choice = 3;
        } else {
            char *end;
            long value = strtol(input, &end, 10);
            choice = (end != input && *end == '\0') ? (int)value : 0;
        }

        switch (choice) {
            case 1:
                list_students(students, count);
                break;
            case 2:
                add_student(students, &count);
                break;
            case 3:
                if (save_students(students, count) != 0) {
                    fprintf(stderr, "Error saving data.\n");
                    return EXIT_FAILURE;
                }
                printf("Data saved. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return EXIT_SUCCESS;
}

// Provided helper: reads a full line, stripping newline and draining long lines.
int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (fgets(buf, (int)size, stdin) == NULL) {
        return -1;
    }
    size_t len = strcspn(buf, "\r\n");
    if (buf[len] == '\0' && len == size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[len] = '\0';
    return 0;
}

int load_students(Student arr[], int max) {
    FILE *fp = fopen(DATA_FILE, "r");
    if (fp == NULL) return 0; // First run: no file is normal.

    char line[LINE_LEN];
    int count = 0;
    while (count < max && fgets(line, sizeof line, fp) != NULL) {
        Student student = {0};
        int consumed = 0;
        // %n helps reject malformed lines with extra non-space characters.
        int parsed = sscanf(line, "%49[^,],%d,%f %n",
                            student.name, &student.id, &student.gpa, &consumed);
        if (parsed != 3 || consumed == 0 || line[consumed] != '\0') continue;
        if (student.name[0] == '\0' || student.gpa < 0.0f ||
            !(student.gpa <= 4.0f)) continue;
        arr[count++] = student;
    }
    fclose(fp);
    return count;
}

int save_students(const Student arr[], int count) {
    FILE *fp = fopen(DATA_FILE, "w");
    if (fp == NULL) return -1;

    int ok = 1;
    for (int i = 0; i < count; i++) {
        if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name,
                    arr[i].id, arr[i].gpa) < 0) {
            ok = 0;
            break;
        }
    }
    if (fclose(fp) != 0) ok = 0;
    return ok ? 0 : -1;
}

// Parse an entire integer, rejecting garbage and overflows.
static int parse_id(const char *text, int *out) {
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (end == text || errno == ERANGE || value < INT_MIN || value > INT_MAX)
        return -1;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return -1;
    *out = (int)value;
    return 0;
}

static int parse_gpa(const char *text, float *out) {
    char *end;
    errno = 0;
    float value = strtof(text, &end);
    if (end == text || errno == ERANGE || !(value >= 0.0f && value <= 4.0f))
        return -1;
    while (isspace((unsigned char)*end)) end++;
    if (*end != '\0') return -1;
    *out = value;
    return 0;
}

void add_student(Student arr[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("Cannot add more students (limit reached).\n");
        return;
    }

    char name[LINE_LEN], id_text[LINE_LEN], gpa_text[LINE_LEN];
    if (read_line("Enter name: ", name, sizeof name) != 0 ||
        read_line("Enter ID: ", id_text, sizeof id_text) != 0 ||
        read_line("Enter GPA: ", gpa_text, sizeof gpa_text) != 0)
        return;

    int id;
    float gpa;
    size_t len = strlen(name);
    int blank = 1;
    for (size_t i = 0; i < len; i++) {
        if (!isspace((unsigned char)name[i])) { blank = 0; break; }
    }

    if (blank || len >= NAME_LEN || strchr(name, ',') != NULL ||
        parse_id(id_text, &id) != 0 || parse_gpa(gpa_text, &gpa) != 0) {
        printf("Invalid input. Student not added.\n");
        return;
    }

    Student *s = &arr[*count];
    strcpy(s->name, name); // Verified below NAME_LEN bytes including NUL.
    s->id = id;
    s->gpa = gpa;
    (*count)++;
    printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
    if (count == 0) {
        printf("No students found.\n");
        return;
    }
    printf("%-5s %-20s %s\n", "ID", "Name", "GPA");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
    }
}
