#include "server_sockt.h"
#include "server_ahorcado.h"
#include <stdio.h>
#include <stdbool.h>


void enviar_msg(Sockt_srv *skt, uint8_t intentos, char *pal, uint16_t len){
	char *intent, len_pal[2];
	intent = (char *)&intentos;
	uint16_t len_pal_ns = htons(len);
	memcpy(len_pal, &len_pal_ns, 2);
	sockt_srv_write(skt, intent, 1);
	sockt_srv_write(skt, len_pal, 2);
	sockt_srv_write(skt, pal, (size_t)len);
}

void crear_fill(char *pal, int len){
	for (int i = 0; i < len; ++i){
		pal[i] = '_';
	}
	pal[len] = '\0';
}

bool juego_loop(Sockt_srv *skt, Ahorcado *aho, int intent, uint16_t len){
	char buf[1], revel[len+1];
	bool ganaste = false;
	while(ganaste == false && intent < 128){
		sockt_srv_read(skt, buf, 1);
		ganaste = ahorcado_probar(aho, buf[0], revel, &intent);
		if (intent == 0 || ganaste){
			intent += 128;
		}
		enviar_msg(skt, (uint8_t)intent, revel, (uint16_t)(len-1));
	}
	return ganaste;
}

void print_recrd(int victorias, int derrotas){
	printf("Resumen:\n");
	printf("	Victorias: %d\n", victorias);
	printf("	derrotas: %d\n", derrotas);
}

void empezar_juego(char *file, int port, int intentos){
	Sockt_srv skt;
	Ahorcado aho;
	char s[32];
	char *pal = s;
	size_t size = 10;
	char pal_revelada[32];
	ssize_t len;
	sockt_srv_init(&skt, (uint16_t)port, 5);
	int victorias = 0;
	int derrotas = 0;
	FILE *fil = fopen(file, "r");
	while ((len = getline(&pal, &size, fil)) != -1){
		pal[len-1] = '\0';
		ahorcado_init(&aho, pal, intentos);
		sockt_srv_accept(&skt);
		crear_fill(pal_revelada, len-1);
		enviar_msg(&skt, (uint8_t)intentos, pal_revelada, (uint16_t)(len-1));
		if(juego_loop(&skt, &aho, intentos, (uint16_t)len)){
			++victorias;
		} else{
			++derrotas;
		}
		ahorcado_destroy(&aho);
	}
	fclose(fil);
	print_recrd(victorias, derrotas);
	sockt_srv_destroy(&skt);
}

int main(int argc, char *argv[]){
	if (argc < 1){
		perror("argumentos insuficientes");
		exit(1);
	}
	empezar_juego(argv[3], atoi(argv[1]), atoi(argv[2])); 
	return 0;
}
