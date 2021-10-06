#ifndef __SERVER_H__
#define __SERVER_H__

#include "server.h"
#include "server_sockt.h"
#include "common_sockt.h"
#include "server_ahorcado.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct Servidor {
	int victorias;
	int derrotas;
	Ahorcado *ahorcado;
	Sockt_srv *skt_srv;
	Sockt *skt;
	uint8_t intentos;
} Servidor;

void servidor_init(Servidor *servidor, Sockt_srv *skt_srv,
Ahorcado *ahorcado, uint8_t intentos);

void servidor_victoria(Servidor *servidor);

void servidor_derrota(Servidor *servidor);

int servidor_new_sockt(Servidor *servidor);

void servidor_cambiar_palabra(Servidor *servidor, char *pal);

void servidor_destroy(Servidor *servidor);


//envia el mensaje al cliente con el formato correcto
void servidor_enviar_msg(Servidor *servidor, char *pal, uint16_t len);

//para la primera impresion de caracteres vacios
void crear_fill(char *pal, int len);

//maneja la logica del juego de ahorcado
bool servidor_juego_loop(Servidor *servidor, uint16_t len);

bool servidor_jugar_letra(Servidor *servidor, char *revelados, char caracter);

//imprime pantalla final
void print_recrd(Servidor *servidor);

int servidor_palabras_loop(Servidor *servidor, char *pal, uint16_t len);

void empezar_juego(char *file, char *port, int intentos);

int main(int argc, char *argv[]);

#endif
