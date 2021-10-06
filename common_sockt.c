#include "common_sockt.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#define h_addr  h_addr_list[0]
#define ERROR_MSG "falla al enviar mensaje"
#define ERROR_NO -1

void sockt_init(Sockt *skt, int fd){
	skt->fd = fd;
}

int sockt_read(Sockt *skt, char *buf, size_t exp_len){
	int bytes = recv(skt->fd, buf, exp_len, 0);
	if (bytes == -1){
		sockt_destroy(skt);
		perror(ERROR_MSG);
		return ERROR_NO;
	}
	int bytes_sum = bytes; //bytes enviados hasta ahora
	while (bytes_sum < exp_len){
		bytes = recv(skt->fd, &buf[bytes_sum], exp_len-bytes_sum, 0);
		if (bytes == -1){
			sockt_destroy(skt);
			perror(ERROR_MSG);
			return ERROR_NO;
		}
		bytes_sum += bytes;
	}
	buf[exp_len] = '\0';
	return 0;
}

int sockt_write(Sockt *skt, char *buf, size_t exp_len){
	int bytes = send(skt->fd, buf, exp_len, 0);
	if (bytes == -1){
		sockt_destroy(skt);
		perror(ERROR_MSG);
		return ERROR_NO;
	}
	int bytes_sum = bytes; //bytes enviados hasta ahora
	while (bytes_sum < exp_len){
		bytes = send(skt->fd, &buf[bytes_sum], exp_len-bytes_sum, 0);
		if (bytes == -1){
			sockt_destroy(skt);
			perror(ERROR_MSG);
			return ERROR_NO;
		}
		bytes_sum += bytes;
	}
	return 0;
}

void sockt_destroy(Sockt *skt){
	shutdown(skt->fd, SHUT_RDWR);
	close(skt->fd);
}
