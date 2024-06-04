#ifndef HARDWARE_H
#define HARDWARE_H

void liga(unsigned short* reg);
void desliga(unsigned short* reg);
void exibicao(int i, unsigned short* reg);
void mudar_cor(unsigned short* regR, unsigned short* regG, unsigned short* regB, int cor);
void definir_velocidade(unsigned short* reg, int velocidade);
void configure_message(const char* message, unsigned short** registers);
void controlar_led_status(unsigned short* reg, int nivel_bateria);
void limpa_emulador(unsigned short *reg[]);
void def_vermelho(unsigned short* reg, int value);
void def_verde(unsigned short* reg, int value);
void def_azul(unsigned short* reg, int value);
int le_bateria_int(unsigned short* reg);
int ve_bit_0(unsigned short* reg);

#endif