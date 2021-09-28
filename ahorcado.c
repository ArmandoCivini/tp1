#include "ahorcado.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


void ahorcado_init(Ahorcado *aho, const char *p, int intent){
	int largo = strlen(p);
	aho->len = largo;
	aho->pal = malloc(largo+1);
	strncpy(aho->pal, p, largo+1);
	aho->intentos = intent;
	char *inten = malloc(largo+1);
	for (int i = 0; i < largo; ++i){ //crea la palabra con caracteres ocultos
		inten[i] = '_';
	}
	inten[largo] = '\0';
	aho->revelados = inten;
}

bool ahorcado_probar(Ahorcado *aho, const char c, char *revel, int *intent){
	bool ganaste = true;	//chequea si se revelo toda la palabra
	bool correcto = false;  //chequea si la letra era contenida
	for (int i = 0; i < aho->len; ++i){
		if (aho->pal[i] == c){
			aho->revelados[i] = c;
			correcto = true;
		}
		if(aho->revelados[i] == '_'){
			ganaste = false;
		}
	}
	if (!correcto){
		--aho->intentos;
	}
	if (aho->intentos == 0){ //en caso de derrota envia la palabra verdadera
		strncpy(revel, aho->pal, aho->len +1);
		*intent = aho->intentos;
		return ganaste;
	}
	strncpy(revel, aho->revelados, aho->len +1);
	*intent = aho->intentos;
	return ganaste; 
}

void ahorcado_destroy(Ahorcado *aho){
	free(aho->pal);
	free(aho->revelados);
}