#include "logger.h"


FILE* log_file = NULL;

void init_logger(const char* filename) {
    errno_t res = fopen_s(&log_file, filename, "w");
    if (res != 0) {
        perror("Failed to open log file");
    }
}

void close_logger() {
    if (log_file != NULL) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_print(const char* msg) {
    if (log_file != NULL) {
        fprintf(log_file, "%s\n", msg);
        fflush(log_file);  // Ensure the message is written immediately
    }
}

void int_array_to_binary_string(int* arr) {
    char binary_string[33];  // +1 for null terminator
    for (int i = 0; i < 32; i++) {
        binary_string[i] = arr[i] ? '1' : '0';
    }
    binary_string[32] = '\0';  // Null-terminate the string
    log_print(binary_string);
}