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

void int_array_to_binary_string(int* arr, int num) {
    char *binary_string;
    if(num>=0)
    {
        binary_string = malloc(37*sizeof(char));
        binary_string[0] = 'R';
        if (num < 10) {
            snprintf(&binary_string[1], 2, "%d", num);
            binary_string[2] = ' ';
        } else {
            snprintf(&binary_string[1], 3, "%02d", num);
        }
        binary_string[3] = ':';
        for (int i = 0; i < 32; i++) {
            binary_string[i+4] = arr[i] ? '1' : '0';
        }
        binary_string[36] = '\0';  // +1 for null terminator

    }
    else{
        binary_string = malloc(33*sizeof(char));  // +1 for null terminator
        for (int i = 0; i < 32; i++) {
            binary_string[i] = arr[i] ? '1' : '0';
        }
        binary_string[32] = '\0';  
    }
    log_print(binary_string);
}