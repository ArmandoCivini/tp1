#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "client_sockt.h"
#include <stdio.h>

//imprime interfaz para jugar
void juego_print(char *pal, uint8_t intentos);

//imprime pantalla de derrota
void terminar_partida_perder(char *pal);

//imprime pantalla de victoria
void terminar_partida_ganar();

//maneja la logica de enviar al server el caracter y imprime pantalla correspondiente
uint8_t adivinar_char(Sockt_cli *skt, char *c, const uint16_t pal_len);

void jugar_ahorcado(Sockt_cli *skt);

int main(int argc, char *argv[]);

#endif
