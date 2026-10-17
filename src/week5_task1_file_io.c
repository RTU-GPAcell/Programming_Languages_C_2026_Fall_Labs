// week5_task1_file_io.c
// Name: Arda  Surname: Kocaman  Student ID: 251ADB187
// Week 5, Task 1: text file writing and reading.

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_FILENAME "data.txt"
#define LINE_LEN 256

int main(int argc, char *argv[]) {
    const char *filename = argc > 1 ? argv[1] : DEFAULT_FILENAME;
    char line[LINE_LEN];
    int line_count = 0;

    printf("Writing lines to %s...\n", filename);
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    // Explicit newlines ensure that fgets reads two separate lines.
    if (fprintf(fp, "Hello, file I/O in C!\n") < 0 ||
        fprintf(fp, "This is another line.\n") < 0) {
        perror(filename);
        fclose(fp);
        return EXIT_FAILURE;
    }
    if (fclose(fp) != 0) {
        perror(filename);
        return EXIT_FAILURE;
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return EXIT_FAILURE;
    }

    printf("Reading contents:\n");
    while (fgets(line, sizeof line, fp) != NULL) {
        fputs(line, stdout);
        line_count++;
    }
    if (ferror(fp)) {
        perror(filename);
        fclose(fp);
        return EXIT_FAILURE;
    }
    if (fclose(fp) != 0) {
        perror(filename);
        return EXIT_FAILURE;
    }

    printf("Lines read: %d\n", line_count);
    return EXIT_SUCCESS;
}
