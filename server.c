#include "server.h"
#include "common_sockt_srv.h"
#include "common_sockt.h"
#include "common_ahorcado.h"
#include "common_protocolo.h"
#include <stdio.h>
#include <stdbool.h>
#define MAX_Q 100
#define ERROR_NO -1
#define ARCH_ERROR_MSG "falla al abrir el archivo"
#define ARG_ERROR_MSG "argumentos insuficientes"


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
		perror(ARCH_ERROR_MSG);
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
	servidor_print_recrd(&servidor);
	servidor_destroy(&servidor);
}

int main(int argc, char *argv[]){
	if (argc < 1){
		perror(ARG_ERROR_MSG);
		return -1;
	}
	empezar_juego(argv[3], argv[1], atoi(argv[2])); 
	return 0;
}
