#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = open("server_output.txt", O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        fprintf(stderr, "Failed to get file size.\n");
        close(fd);
        return 1;
    }

    char* file_data = (char*)malloc(file_stat.st_size + 1);
    if (file_data == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        close(fd);
        return 1;
    }

    ssize_t bytes_read = read(fd, file_data, file_stat.st_size);
    if (bytes_read == -1) {
        fprintf(stderr, "Failed to read file.\n");
        close(fd);
        free(file_data);
        return 1;
    }
    file_data[bytes_read] = '\0';

    double sum = 0.0;
    int count = 0;

    char* line = strtok(file_data, "\n");
    while (line != NULL) {
        char* time_pos = strstr(line, "Time taken to process order:");
        if (time_pos != NULL) {
            double time;
            if (sscanf(time_pos + 29, "%lf", &time) == 1) {
                sum += time;
                count++;
            }
        }
        line = strtok(NULL, "\n");
    }

    if (count > 0) {
        double average_time = sum / count;
        printf("Average time taken to process an order: %lf microseconds.\n", average_time);
    }

    close(fd);
    free(file_data);

    return 0;
}
