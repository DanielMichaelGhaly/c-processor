#include "logger.h"


FILE* log_file_memory = NULL;
FILE* log_file_registers = NULL;

void init_logger_memory(const char* filename) {
    errno_t res = fopen_s(&log_file_memory, filename, "w");
    if (res != 0) {
        perror("Failed to open log file");
    }
}

void init_logger_registers(const char* filename)
{
    errno_t res = fopen_s(&log_file_registers, filename, "w");
    if(res!=0)
    {
        perror("Failed to open log file");
    }
}

void close_logger() {
    if (log_file_memory != NULL&&log_file_registers != NULL) {
        fclose(log_file_memory);
        log_file_memory = NULL;
        fclose(log_file_registers);
        log_file_registers = NULL;
    }
}

void log_print(const char* msg, char* file_name) {
    if (log_file_memory != NULL && log_file_registers != NULL) {
        if(strcmp(file_name, "registers") == 0) {
            fprintf(log_file_registers, "%s\n", msg);
            fflush(log_file_registers); 
        } else if(strcmp(file_name, "memory") == 0) {
            fprintf(log_file_memory, "%s\n", msg);
            fflush(log_file_memory);  
        }
    }
}

void int_array_to_binary_string(int* arr, int num) {
    char *binary_string;
    binary_string = malloc(33*sizeof(char)); 
    for (int i = 0; i < 32; i++) {
        binary_string[i] = arr[i] ? '1' : '0';
    }
    binary_string[32] = '\0';  
    if(num==-1)
    {
        log_print(binary_string, "memory");
    }
    else
    {
        log_print(binary_string, "registers");
    }
}