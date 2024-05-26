#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include "hardware.h"

#define FILE_PATH "registers.bin"
#define FILE_SIZE 1024  // Same size as used in the first program
#define NUM_REGISTERS 16

// Function to open or create the file and map it into memory
char* registers_map(const char* file_path, int file_size, int* fd) {
    *fd = open(file_path, O_RDWR | O_CREAT, 0666);
    if (*fd == -1) {
        perror("Error opening or creating file");
        return NULL;
    }

    // Ensure the file is of the correct size
    if (ftruncate(*fd, file_size) == -1) {
        perror("Error setting file size");
        close(*fd);
        return NULL;
    }

    // Map the file into memory
    char *map = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
    if (map == MAP_FAILED) {
        perror("Error mapping file");
        close(*fd);
        return NULL;
    }

    return map;
}

// Function to release mapped memory and file descriptor
int registers_release(void* map, int file_size, int fd) {
    if (munmap(map, file_size) == -1) {
        perror("Error unmapping the file");
        close(fd);
        return -1;
    }

    if (close(fd) == -1) {
        perror("Error closing file");
        return -1;
    }

    return 0;
}

int main() {
    int fd;
    int command;
    // Open the file and map it into memory
    char* map = registers_map(FILE_PATH, FILE_SIZE, &fd);
    if (map == NULL) {
        return EXIT_FAILURE;
    }

    unsigned short *base_address = (unsigned short *)map;
    unsigned short *registers[NUM_REGISTERS];

    // Initialize the registers
    for (int i = 0; i < NUM_REGISTERS; i++) {
        registers[i] = base_address + i;
    }

    // Print the initial values of the registers
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("Current value of R%d: 0x%04x\n", i, *registers[i]);
    }

    // Use the liga and desliga functions
    liga(registers[0]);
    //printf("New value of R0 after setting bit 0: 0x%04x\n", *registers[0]);

    //desliga(registers[0]);
    //printf("New value of R0 after clearing bit 0: 0x%04x\n", *registers[0]);
    printf("1 - Estatico \n");
    printf("2 - Deslizante \n");
    printf("3 - Piscante \n");
    printf("4 - Deslizante/Piscante \n");
    scanf("%i", &command);
    exibicao(command, registers[0], registers[0]);
    // Release resources
    if (registers_release(map, FILE_SIZE, fd) == -1) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
