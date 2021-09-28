#include "client_sockt.h"
#include <stdio.h>

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

uint8_t adivinar_char(Sockt_cli *skt, char *c, const uint16_t pal_len){
	sockt_cli_write(skt, c, 1);
	char *buf = malloc((pal_len+4)*sizeof(char));
	sockt_cli_read(skt, buf, pal_len+3);
	buf[pal_len+3] = '\0';
	uint8_t intentos = (uint8_t)buf[0];
	buf[pal_len+3] = '\0';
	if (intentos < 128){
		juego_print(&buf[3], intentos);
	} else{
		if (intentos == 128){
			terminar_partida_perder(&buf[3]);
		} else{
			terminar_partida_ganar();
		}
	}
	free(buf);
	return intentos;
}


void jugar_ahorcado(Sockt_cli *skt){
	char buf[32];
	char s[32];
    char *input = s;
    size_t size = 10;
	sockt_cli_read(skt, buf, 3);
	uint8_t pre_pal_len[2];
	pre_pal_len[0] = buf[1];
	pre_pal_len[1] = buf[2];
	uint16_t pal_len_net = *(uint16_t *)pre_pal_len;
	uint16_t pal_len = ntohs(pal_len_net);
	sockt_cli_read(skt, &buf[3], pal_len);
	buf[pal_len+3] = '\0';
	uint8_t intentos = (uint8_t)buf[0];
	juego_print(&buf[3], intentos);
	while(intentos < 128){
		size_t input_len = getline(&input, &size, stdin);
		if (input_len == 1){
			juego_print(&buf[3], intentos);
		}
		for (int i = 0; i < input_len-1; ++i){
			if (intentos >= 128){
				break;
			}
			intentos = adivinar_char(skt, &input[i], pal_len);
		}
	}
}

int main(int argc, char *argv[]){
	if (argc < 2){
		perror("argumentos insuficientes");
		exit(1);
	}
	Sockt_cli skt;
	sockt_cli_init(&skt, argv[1], (uint16_t)atoi(argv[2]));
	jugar_ahorcado(&skt);
	sockt_cli_destroy(&skt);
	return 0;
}
