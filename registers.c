#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include "hardware.h"

#define FILE_PATH "registers.bin"
#define FILE_SIZE 1024
#define NUM_REGISTERS 16
#define DISPLAY_LENGTH 24

char* registers_map(const char* file_path, int file_size, int* fd) {
    *fd = open(file_path, O_RDWR | O_CREAT, 0666);
    if (*fd == -1) {
        perror("Error opening or creating file");
        return NULL;
    }

    if (ftruncate(*fd, file_size) == -1) {
        perror("Error setting file size");
        close(*fd);
        return NULL;
    }

    char *map = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
    if (map == MAP_FAILED) {
        perror("Error mapping file");
        close(*fd);
        return NULL;
    }

    return map;
}

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
    int op;
    int fd;
    int command;
    int corVermelho;
    int corVerde;
    int corAzul;
    int velocidade;
    char message[DISPLAY_LENGTH + 1];

    char* map = registers_map(FILE_PATH, FILE_SIZE, &fd);
    if (map == NULL) {
        return EXIT_FAILURE;
    }

    unsigned short *base_address = (unsigned short *)map;
    unsigned short *registers[NUM_REGISTERS];

    for (int i = 0; i < NUM_REGISTERS; i++) {
        registers[i] = base_address + i;
    }

    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("Current value of R%d: 0x%04x\n", i, *registers[i]);
    }

    do {
        printf("1 - liga/desliga\n");
        printf("2 - mudar modo de exibição\n");
        printf("3 - mudar cores\n");
        printf("4 - mudar palavra\n");
        printf("5 - mudar velocidade do texto\n");
        printf("0 - sair\n");

        printf("Escolha uma opção: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                if (ve_bit_0(registers[0]) == 0) {
                    liga(registers[0]);
                } else {
                    desliga(registers[0]);
                }
                break;
            case 2:
                printf("Escolha o modo de exibição:\n");
                printf("1 - Estatico\n");
                printf("2 - Deslizante\n");
                printf("3 - Piscante\n");
                printf("4 - Deslizante/Piscante\n");
                scanf("%d", &command);
                exibicao(command, registers[0]);
                break;
            case 3:
                printf("Escolha a quantidade de cada cor da mensagem:\n");
                printf("Quantidade de vermelho (max. 255): \n");
                scanf("%d", &corVermelho);
                def_vermelho(registers[1], corVermelho);

                printf("Quantidade de verde (max. 255): \n");
                scanf("%d", &corVerde);
                def_verde(registers[1], corVerde);

                printf("Quantidade de azul (max. 255): \n");
                scanf("%d", &corAzul);
                def_azul(registers[2], corAzul);
                break;
            case 4:
                printf("Digite uma mensagem (max %d caracteres): ", DISPLAY_LENGTH);
                limpa_emulador(registers);
                getchar(); // Consumes the newline character left by previous input
                fgets(message, DISPLAY_LENGTH + 1, stdin);
                configure_message(message, registers);
                break;
            case 5:
                printf("Escolha a velocidade de atualização (0-63):\n");
                scanf("%d", &velocidade);
                definir_velocidade(registers[0], velocidade);
                break;
            case 0:
                printf("Fim do programa!\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (op != 0);

    // Controle do LED de status de acordo com o nível da bateria
    int nivel_bateria = le_bateria_int(registers[3]);
    controlar_led_status(registers[0], nivel_bateria);

    printf("Configurações aplicadas:\n");
    printf("Modo de exibição: %d\n", command);
    printf("Velocidade: %d\n", velocidade);
    printf("Nível da bateria: %d\n", nivel_bateria);

    if (registers_release(map, FILE_SIZE, fd) == -1) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
