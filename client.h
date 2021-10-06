#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "common_sockt.h"
#include "client_sockt_connect.h"
#include <stdio.h>

//imprime interfaz para jugar
void juego_print(char *pal, uint8_t intentos);

//imprime pantalla de derrota
void terminar_partida_perder(char *pal);

//imprime pantalla de victoria
void terminar_partida_ganar();

//maneja la logica de recivir mensaje al server
int protocolo_mensajes(Sockt *skt, uint8_t *intentos, char **palabra);
 
//envia el caracter y imprime pantalla correspondiente
uint8_t adivinar_char(Sockt *skt, char *c, char **palabra);

void jugar_ahorcado(Sockt *skt);

int main(int argc, char *argv[]);

#endif
