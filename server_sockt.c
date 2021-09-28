#include "server_sockt.h"
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


void sockt_srv_init(Sockt_srv *skt, uint16_t port, int max_q){
	skt->fd_act = -1;
	struct sockaddr_in addr;
	skt->fd = socket(AF_INET, SOCK_STREAM, 0);
	if(skt->fd == -1){
		perror("inicialización del socket");
		exit(1);
	}
	memset((char *)&addr,0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	socklen_t sockaddr_size = (socklen_t)sizeof(struct sockaddr);
	if ( bind( skt->fd,(struct sockaddr *)&addr, sockaddr_size) == -1 ){
		perror("union socket server");
		exit(1);
	}
	listen(skt->fd, max_q);
}

void sockt_srv_accept(Sockt_srv *skt){
	struct sockaddr_in *cli_act;
	unsigned int len = (socklen_t)sizeof(cli_act);
	if (skt->fd_act != -1){
		close(skt->fd_act);
	}
	skt->fd_act = accept(skt->fd,(struct sockaddr *)&cli_act, &len);
	if (skt->fd_act == -1){
		perror("falla en aceptar");
		exit(1);
	}
}

void sockt_srv_read(Sockt_srv *skt, char *buf, size_t exp_len){
	int bytes = recv(skt->fd_act, buf, exp_len, 0);
	if (bytes == -1){
		perror("falla al recivir mensaje");
		exit(1);
	}
	int bytes_sum = bytes;
	while (bytes_sum < exp_len){
		bytes = recv(skt->fd_act, &buf[bytes_sum], exp_len-bytes_sum, 0);
		if (bytes == -1){
			perror("falla al recivir mensaje");
			exit(1);
		}
		bytes_sum += bytes;
	}
	buf[exp_len] = '\0';
}

void sockt_srv_write(Sockt_srv *skt, char *buf, size_t exp_len){
	int bytes = send(skt->fd_act, buf, exp_len, 0);
	if (bytes == -1){
		perror("falla al enviar mensaje");
		exit(1);
	}
	int bytes_sum = bytes;
	while (bytes_sum < exp_len){
		bytes = send(skt->fd_act, &buf[bytes_sum], exp_len-bytes_sum, 0);
		if (bytes == -1){
			perror("falla al enviar mensaje");
			exit(1);
		}
		bytes_sum += bytes;
	}
}

void sockt_srv_destroy(Sockt_srv *skt){
	close(skt->fd_act);
	close(skt->fd);
}