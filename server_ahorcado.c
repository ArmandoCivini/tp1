#include "server_ahorcado.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define ERROR_NO -1

void ahorcado_init(Ahorcado *ahorcado, const char *p, int intentos){
	int largo = strlen(p);
	ahorcado->len = largo;
	ahorcado->pal = malloc(largo+1);
	strncpy(ahorcado->pal, p, largo+1);
	ahorcado->intentos = intentos;
	ahorcado->intentos_totales = intentos;
	char *pal_escondida = malloc(largo+1);
	for (int i = 0; i < largo; ++i){
		pal_escondida[i] = '_';
	}
	pal_escondida[largo] = '\0';
	ahorcado->revelados = pal_escondida;
}

bool ahorcado_probar(Ahorcado *ahorcado, 
const char c, char *revelado, int *intentos){
	bool ganaste = true;	
	bool correcto = false;
	for (int i = 0; i < ahorcado->len; ++i){
		if (ahorcado->pal[i] == c){
			ahorcado->revelados[i] = c;
			correcto = true;
		}
		if(ahorcado->revelados[i] == '_'){
			ganaste = false;
		}
	}
	if (!correcto){
		--ahorcado->intentos;
	}
	if (ahorcado->intentos == 0){
		strncpy(revelado, ahorcado->pal, ahorcado->len +1);
		*intentos = ahorcado->intentos;
		return ganaste;
	}
	strncpy(revelado, ahorcado->revelados, ahorcado->len +1);
	*intentos = ahorcado->intentos;
	return ganaste; 
}

int nueva_palabra(Ahorcado *ahorcado, char *palabra){
	free(ahorcado->pal);
	free(ahorcado->revelados);
	ahorcado->intentos = ahorcado->intentos_totales;
	int largo = strlen(palabra);
	ahorcado->len = largo;
	ahorcado->pal = malloc(largo+1);
	if (ahorcado->pal == NULL){
		return ERROR_NO;
	}
	strncpy(ahorcado->pal, palabra, largo+1);
	char *pal_escondida = malloc(largo+1);
	if (pal_escondida == NULL){
		return ERROR_NO;
	}
	for (int i = 0; i < largo; ++i){
		pal_escondida[i] = '_';
	}
	pal_escondida[largo] = '\0';
	ahorcado->revelados = pal_escondida;
	return ahorcado->intentos_totales;
}

void ahorcado_destroy(Ahorcado *ahorcado){
	free(ahorcado->pal);
	free(ahorcado->revelados);
}
