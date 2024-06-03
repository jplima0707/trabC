#include <stdio.h>
#include "hardware.h"

void set_register_bit(unsigned short* reg, int bit_position, int value) {
    if (value) {
        *reg |= (1 << bit_position);
    } else {
        *reg &= ~(1 << bit_position);
    }
}

void liga(unsigned short* reg) {
    set_register_bit(reg, 0, 1);
}

void desliga(unsigned short* reg) {
    set_register_bit(reg, 0, 0);
}

void exibicao(int i, unsigned short* reg) {
    switch(i) {
        case 1:
            set_register_bit(reg, 1, 0);
            set_register_bit(reg, 2, 0);
            break;
        case 2:
            set_register_bit(reg, 1, 1);
            set_register_bit(reg, 2, 0);
            break;
        case 3:
            set_register_bit(reg, 1, 0);
            set_register_bit(reg, 2, 1);
            break;
        case 4:
            set_register_bit(reg, 1, 1);
            set_register_bit(reg, 2, 1);
            break;
        default:
            printf("Invalid command\n");
            break;
    }
}

void def_vermelho(unsigned short *reg, int value) {
    *reg &= ~(255 << 0);
    if(value <= 255 && value >= 0) {
        *reg |= (value << 0);
    }
}

void def_verde(unsigned short *reg, int value) {
    *reg &= ~(255 << 8);
    if(value <= 255 && value >= 0) {
        *reg |= (value << 8);
    }
}

void def_azul(unsigned short *reg, int value) {
    *reg &= ~(255 << 0);
    if(value <= 255 && value >= 0) {
        *reg |= (value << 0);
    }
}

void limpa_emulador(unsigned short *reg[]) {
    for (int i = 4; i < 16; i++) {
        *reg[i] = 0;
    }
}

void definir_velocidade(unsigned short* reg, int velocidade) {
    if (velocidade >= 0 && velocidade <= 63) {
        int inversa_velocidade = 63 - velocidade;
        *reg = (*reg & 0xFC07) | (inversa_velocidade << 3);
    } else {
        printf("Invalid speed\n");
    }
}

void configure_message(const char* message, unsigned short** registers) {
    for (int i = 0; i < 12; ++i) {
        if (message[i] != '\0') {
            registers[i + 4][0] = message[i];
        } else {
            registers[i + 4][0] = ' ';
        }
    }
}

void controlar_led_status(unsigned short* reg, int nivel_bateria) {
    switch(nivel_bateria) {
        case 0: // Crítico
            set_register_bit(reg, 10, 1); // Vermelho
            set_register_bit(reg, 11, 0);
            set_register_bit(reg, 12, 0);
            break;
        case 1: // Baixo
            set_register_bit(reg, 10, 1); // Amarelo (Vermelho + Verde)
            set_register_bit(reg, 11, 1);
            set_register_bit(reg, 12, 0);
            break;
        case 2: // Demais
            set_register_bit(reg, 10, 0); // Verde
            set_register_bit(reg, 11, 1);
            set_register_bit(reg, 12, 0);
            break;
        default:
            printf("Invalid battery level\n");
            break;
        }

    // int le_bateria_int(unsigned short* reg) {
    // return (*reg >> 0) & 0b11;
    // }

    // char* le_bateria_status(unsigned short *r3) {
    //     switch (le_bateria_int(r3)) {
    //         case 0: return "Critico";
    //         case 1: return "Baixo";
    //         case 2: return "Medio";
    //         case 3: return "Alto";
    //     default: return "Invalido";
    //     }
    // }
}