#include "client_sockt.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#define h_addr  h_addr_list[0]



void sockt_cli_init(Sockt_cli *skt, char *host, uint16_t port){
	struct hostent *he;
	struct sockaddr_in end_info;
	if ((he=gethostbyname(host)) == NULL){
		perror("no se pudo obtener el nombre del host");
		exit(1);
	}
	skt->fd = socket(AF_INET, SOCK_STREAM, 0);
	if(skt->fd == -1){
		perror("inicialización del socket");
		exit(1);
	}
	end_info.sin_family = AF_INET;
	end_info.sin_port = htons(port);
	end_info.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(end_info.sin_zero), 8, sizeof(int));
	size_t tam = sizeof(struct sockaddr);
	if (connect(skt->fd, (struct sockaddr *)&end_info, tam) == -1){
		close(skt->fd);
		perror("falla de conexión");
		exit(1);
	}
}

void sockt_cli_read(Sockt_cli *skt, char *buf, size_t exp_len){
	int bytes = recv(skt->fd, buf, exp_len, 0);
	if (bytes == -1){
		close(skt->fd);
		perror("falla al recivir mensaje");
		exit(1);
	}
	int bytes_sum = bytes;
	while (bytes_sum < exp_len){
		bytes = recv(skt->fd, &buf[bytes_sum], exp_len-bytes_sum, 0);
		if (bytes == -1){
			close(skt->fd);
			perror("falla al recivir mensaje");
			exit(1);
		}
		bytes_sum += bytes;
	}
	buf[exp_len] = '\0';
}

void sockt_cli_write(Sockt_cli *skt, char *buf, size_t exp_len){
	int bytes = send(skt->fd, buf, exp_len, 0);
	if (bytes == -1){
		perror("falla al enviar mensaje");
		exit(1);
	}
	int bytes_sum = bytes;
	while (bytes_sum < exp_len){
		bytes = send(skt->fd, &buf[bytes_sum], exp_len-bytes_sum, 0);
		if (bytes == -1){
			close(skt->fd);
			perror("falla al enviar mensaje");
			exit(1);
		}
		bytes_sum += bytes;
	}
}

void sockt_cli_destroy(Sockt_cli *skt){
	close(skt->fd);
}
