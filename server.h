#ifndef __SERVER_H__
#define __SERVER_H__

#include "server_sockt.h"
#include "server_ahorcado.h"
#include <stdio.h>
#include <stdbool.h>

//envia el mensaje al cliente con el formato correcto
void enviar_msg(Sockt_srv *skt, uint8_t intentos, char *pal, uint16_t len);

//para la primera impresion de caracteres vacios
void crear_fill(char *pal, int len);

//maneja la logica del juego de ahorcado
bool juego_loop(Sockt_srv *skt, Ahorcado *aho, int intent, uint16_t len);

//imprime pantalla final
void print_recrd(int victorias, int derrotas);

void empezar_juego(char *file, int port, int intentos);

int main(int argc, char *argv[]);

#endif