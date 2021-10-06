#include "server.h"
#include "server_sockt.h"
#include "common_sockt.h"
#include "server_ahorcado.h"
#include <stdio.h>
#include <stdbool.h>
#define MAX_Q 100
#define CARACTER 1
#define ULTIMO_BIT 128
#define ERROR_NO -1




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

void servidor_cambiar_palabra(Servidor *servidor, char *pal){
	int intentos_totales = nueva_palabra(servidor->ahorcado, pal);
	servidor->intentos = intentos_totales;
}

void servidor_destroy(Servidor *servidor){
	sockt_destroy(servidor->skt);
	free(servidor->skt);
	sockt_srv_destroy(servidor->skt_srv);
	ahorcado_destroy(servidor->ahorcado);
}

void enviar_msg(Servidor *servidor, char *pal, uint16_t len){
	char len_pal[2];
	char *intentos = (char *)&servidor->intentos;
	uint16_t len_pal_ns = htons(len); //conversion x endianess
	memcpy(len_pal, &len_pal_ns, 2);
	sockt_write(servidor->skt, intentos, 1);
	sockt_write(servidor->skt, len_pal, 2);
	sockt_write(servidor->skt, pal, (size_t)len);
}

void crear_fill(char *pal, int len){
	for (int i = 0; i < len; ++i){
		pal[i] = '_';
	}
	pal[len] = '\0';
}

bool jugar_letra(Servidor *servidor, char *revelados, char caracter){
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
		
		ganaste = jugar_letra(servidor, revelados, buf[0]);

		enviar_msg(servidor, revelados, len-1);
		intentos = servidor->intentos;
	}
	return ganaste;
}

void print_recrd(Servidor *servidor){
	printf("Resumen:\n");
	printf("	Victorias: %d\n", servidor->victorias);
	printf("	Derrotas: %d\n", servidor->derrotas);
}

int servidor_palabras_loop(Servidor *servidor, char *pal, uint16_t len){
	char *pal_revelada = malloc(sizeof(char) * len);
	if (pal[0] == '\n'){
		return 0; //si hay linea vacia sigue
	}
	pal[len-1] = '\0';
	servidor_cambiar_palabra(servidor, pal);

	int err = servidor_new_sockt(servidor);
	if (err == ERROR_NO){
		free(pal_revelada);
		return ERROR_NO;
	}
	crear_fill(pal_revelada, len-1);
	enviar_msg(servidor, pal_revelada, len-1);
	if(servidor_juego_loop(servidor, len)){
		servidor_victoria(servidor);
	} else{
		servidor_derrota(servidor);
	}
	free(pal_revelada);
	return 0;
}

void empezar_juego(char *file, char *port, int intentos){
	Sockt_srv skt_srv;
	Ahorcado ahorcado;
	Servidor servidor;
	char *pal = NULL;
	size_t size = 0;
	ssize_t len;
	sockt_srv_init(&skt_srv, port, MAX_Q);

	FILE *fil = fopen(file, "r");
	if (fil == NULL){
		sockt_srv_destroy(&skt_srv);
		perror("falla al abrir el archivo");
		return;
	}
	ahorcado_init(&ahorcado, "", intentos);

	servidor_init(&servidor, &skt_srv, &ahorcado, (uint8_t)intentos);

	while ((len = getline(&pal, &size, fil)) != -1){
		int err = servidor_palabras_loop(&servidor, pal, len);
		if (err == ERROR_NO){
			break;
		}
	}
	free(pal);
	fclose(fil);
	print_recrd(&servidor);
	servidor_destroy(&servidor);
}

int main(int argc, char *argv[]){
	if (argc < 1){
		perror("argumentos insuficientes");
		return -1;
	}
	empezar_juego(argv[3], argv[1], atoi(argv[2])); 
	return 0;
}
