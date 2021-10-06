#include "client.h"
#include "common_sockt.h"
#include "client_sockt_connect.h"
#include <stdio.h>
#define ERROR_NO -1
#define ULTIMO_BIT 128
#define BUFSIZE 1000
#define ERROR_NO_U8 255


void juego_print(char *pal, uint8_t intentos){
	printf("Palabra secreta: %s\n", pal);
	int intent_p = intentos;
	printf("Te quedan %d intentos\n", intent_p);
	printf("Ingrese letra: \n");
}

void terminar_partida_perder(char *pal){
	printf("Perdiste! La palabra secreta era: '%s'\n", pal);
}

void terminar_partida_ganar(){
	printf("Ganaste!!\n");
}

uint8_t adivinar_char(Sockt *skt, char *c){
	char *palabra = NULL;
	uint8_t intentos;
	sockt_write(skt, c, 1);

	int err = protocolo_mensajes(skt, &intentos, &palabra);
	if (err == -1){
		return ERROR_NO_U8;
	}

	if (intentos < ULTIMO_BIT){
		juego_print(palabra, intentos);
	} else{
		if (intentos == ULTIMO_BIT){
			terminar_partida_perder(palabra);
		} else{
			terminar_partida_ganar();
		}
	}
	free(palabra);
	return intentos; //para comunicar si partida terminó
}

int protocolo_mensajes(Sockt *skt, uint8_t *intentos, char **palabra){
	char buf[3];
	sockt_read(skt, buf, 3);
	uint8_t pre_pal_len[2];
	pre_pal_len[0] = buf[1];
	pre_pal_len[1] = buf[2];
	uint16_t pal_len_net = *(uint16_t *)pre_pal_len;
	uint16_t pal_len = ntohs(pal_len_net); //conversion x endianess
	*palabra = (char *)malloc(sizeof(char) * (pal_len+1));
	if (*palabra==NULL){
		return ERROR_NO;
	}
	sockt_read(skt, *palabra, pal_len);
	palabra[pal_len] = '\0';
	*intentos = (uint8_t)buf[0];
	return 0;
}

void jugar_ahorcado(Sockt *skt){
    char *input = NULL;
    size_t size = 0;
    uint8_t intentos;
    char *palabra = NULL;
    int err = protocolo_mensajes(skt, &intentos, &palabra);
    if (err == -1){
    	return;
    }

	juego_print(palabra, intentos);
	while(intentos < ULTIMO_BIT){ //checkeo si el juego terminó
		size_t input_len = getline(&input, &size, stdin);
		if (input_len == 1){
			juego_print(palabra, intentos);
		}
		for (int i = 0; i < input_len-1; ++i){
			if (intentos >= ULTIMO_BIT){ //checkeo si el juego terminó
				break;
			}
			intentos = adivinar_char(skt, &input[i]);
			if (intentos == ERROR_NO_U8){
				break;
			}
		}
	}
	free(input);
	free(palabra);
}

int main(int argc, char *argv[]){
	int err;
	int fd;
	if (argc < 2){
		perror("argumentos insuficientes");
		return -1;
	}
	Sockt_connect skt_connect;
	Sockt skt;
	err = sockt_connect_init(&skt_connect, argv[1], argv[2]);
	if (err == -1){
		return ERROR_NO;
	}
	fd = sockt_connect_connection(&skt_connect);
	if (fd == -1){
		sockt_connect_destroy(&skt_connect);
		return ERROR_NO;
	}
	sockt_init(&skt, fd);
	jugar_ahorcado(&skt);
	sockt_destroy(&skt);
	sockt_connect_destroy(&skt_connect);
	return 0;
}
