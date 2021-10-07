#include "common_sockt.h"
#include "common_ahorcado.h"
#include "common_sockt_srv.h"
#include "common_protocolo.h"
#include <stdio.h>
#include <stdbool.h>
#define ERROR_NO -1
#define BUFSIZE_PROTOCOLO 3
#define RESUMEN "Resumen:\n"
#define VICTORIAS "	Victorias: %d\n"
#define DERROTAS "	Derrotas: %d\n"
#define ULTIMO_BIT 128
#define CARACTER 1

int protocolo_mensajes(Sockt *skt, uint8_t *intentos, char **palabra){
	char buf[BUFSIZE_PROTOCOLO];
	sockt_read(skt, buf, BUFSIZE_PROTOCOLO);
	uint8_t pre_pal_len[2];
	pre_pal_len[0] = buf[1];
	pre_pal_len[1] = buf[2];
	uint16_t pal_len_net = *(uint16_t *)pre_pal_len;
	uint16_t pal_len = ntohs(pal_len_net); //conversion x endianess
	free(*palabra);
	*palabra = (char *)malloc(sizeof(char) * (pal_len+1));
	if (*palabra==NULL){
		return ERROR_NO;
	}
	sockt_read(skt, *palabra, pal_len);

	*intentos = (uint8_t)buf[0];
	return 0;
}


void servidor_init(Servidor *servidor, Sockt_srv *skt_srv,
Ahorcado *ahorcado, uint8_t intentos){
	servidor->ahorcado = ahorcado;
	servidor->skt_srv = skt_srv;
	servidor->victorias = 0;
	servidor->derrotas = 0;
	servidor->intentos = intentos;
	servidor->skt = NULL;
}

void servidor_victoria(Servidor *servidor){
	++servidor->victorias;
}

void servidor_derrota(Servidor *servidor){
	++servidor->derrotas;
}

int servidor_new_sockt(Servidor *servidor){
	if (servidor->skt != NULL){
		sockt_destroy(servidor->skt);
		free(servidor->skt);
	}
	servidor->skt = malloc(sizeof(Sockt));
	if (servidor->skt == NULL){
		return ERROR_NO;
	}
	int fd = sockt_srv_accept(servidor->skt_srv);
	if (fd == -1){
		return ERROR_NO;
	}
	sockt_init(servidor->skt, fd);
	return 0;
}

void servidor_crear_fill(char *pal, int len){
	for (int i = 0; i < len; ++i){
		pal[i] = '_';
	}
	pal[len] = '\0';
}

int servidor_cambiar_palabra(Servidor *servidor, char *pal){
	int intentos_totales = nueva_palabra(servidor->ahorcado, pal);
	if (intentos_totales == -1){
		return ERROR_NO;
	}
	servidor->intentos = intentos_totales;
	return 0;
}

void servidor_destroy(Servidor *servidor){
	sockt_destroy(servidor->skt);
	free(servidor->skt);
	sockt_srv_destroy(servidor->skt_srv);
	ahorcado_destroy(servidor->ahorcado);
}

void servidor_enviar_msg(Servidor *servidor, char *pal, uint16_t len){
	char len_pal[2];
	char *intentos = (char *)&servidor->intentos;
	uint16_t len_pal_ns = htons(len); //conversion x endianess
	memcpy(len_pal, &len_pal_ns, 2);
	sockt_write(servidor->skt, intentos, 1);
	sockt_write(servidor->skt, len_pal, 2);
	sockt_write(servidor->skt, pal, (size_t)len);
}

bool servidor_jugar_letra(Servidor *servidor, char *revelados, char caracter){
	int intentos;
	bool ganar;
	ganar = ahorcado_probar(servidor->ahorcado, caracter, revelados, &intentos);
	if (intentos == 0 || ganar){ //checkea si juego terminó
		intentos += ULTIMO_BIT;
	}
	servidor->intentos = intentos;
	return ganar;
}

bool servidor_juego_loop(Servidor *servidor, uint16_t len){
	char buf[CARACTER], revelados[len+1];
	bool ganaste = false;
	int intentos = servidor->intentos;
	while(ganaste == false && intentos < ULTIMO_BIT){
		sockt_read(servidor->skt, buf, 1);
		
		ganaste = servidor_jugar_letra(servidor, revelados, buf[0]);

		servidor_enviar_msg(servidor, revelados, len-1);
		intentos = servidor->intentos;
	}
	return ganaste;
}

int servidor_palabras_loop(Servidor *servidor, char *pal, uint16_t len){
	char *pal_revelada = malloc(sizeof(char) * len);
	if (pal_revelada == NULL){
		return ERROR_NO;
	}
	if (pal[0] == '\n'){
		free(pal_revelada);
		return 0; //si hay linea vacia sigue
	}
	pal[len-1] = '\0';
	int err = servidor_cambiar_palabra(servidor, pal);
	if (err == -1){
		free(pal_revelada);
		return ERROR_NO;
	}
	err = servidor_new_sockt(servidor);
	if (err == ERROR_NO){
		free(pal_revelada);
		return ERROR_NO;
	}
	servidor_crear_fill(pal_revelada, len-1);
	servidor_enviar_msg(servidor, pal_revelada, len-1);
	if(servidor_juego_loop(servidor, len)){
		servidor_victoria(servidor);
	} else{
		servidor_derrota(servidor);
	}
	free(pal_revelada);
	return 0;
}

void servidor_print_recrd(Servidor *servidor){
	printf(RESUMEN VICTORIAS DERROTAS, servidor->victorias, servidor->derrotas);
}
