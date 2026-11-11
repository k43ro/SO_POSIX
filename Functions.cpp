#include "Functions.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

void Functions::read(char * in){

    cin.getline(in, MAX + 1);
    if (cin.fail()){
        cin.clear();
        cin.ignore(MAX, '\n');
    }

}

int Functions::strsplit(char * str, const char * delim, char ** & parts) {  
  parts = (char **) malloc(sizeof(char*));
  parts[0] = NULL;

  if (str == NULL) return 0;  // caso que no recibamos str

  int numParts = 0;
  
  int i = 0;
  while (str[i] != '\0') {
    int j = i;
	  // recorrer str hasta encontrar un delimitador o el final
    while (str[j] != '\0' && strchr(delim, str[j]) == NULL) {
      j++;
    }

    if (str[j] != '\0') { // Si no se ha llegado al final
      str[j] = '\0'; // terminar la subcadena, sustituir el delimitador por un '\0'
      j++;           // Avanzar al siguiente caracter de la cadena str
    }

    if (str[i] != '\0') { // Si la subcadena tiene caracteres
      // actualizar trozos
      numParts++;
      parts = (char **) realloc(parts, (numParts+1)*sizeof(char*));
      parts[numParts-1] = &str[i];
      parts[numParts] = NULL; // necesario para usar después con execvp
    }

    i = j; // Para continuar con el siguiente trozo
  }

  return numParts;
}

char * Functions::strtrim(char * str){
    if (str == NULL){
        return NULL;
    }

    int i = 0;
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }
    // Avanza por la derecha
    int f = strlen(str)-1;
    while (f > i && (str[f] == ' ' || str[f] == '\t')){
        f--;
    }
    // Poner el carácter de fin de cadena
    str[f+1] = '\0';
    // Devuelve la dirección del nuevo string
    return &str[i];
}

void Functions::getredir(char* str, char* & in, char* & out){

    // Suponemos que no hay ninguna redirección, y si la hay, simplemente reasignamos el valor.
    in = NULL;
    out = NULL;
    
    for (int i = 0; str[i] != '\0' ; i++){ // Vamos recorriendo el texto.
        if (str[i] == '<'){
            in = &str[i+1]; // Podemos "abusar" de que &str[a] devuelve str[a] y todo despues de str[a] hasta '\0'
            str[i] = '\0'; // Como antes, como son punteros, cortamos str, que devolverá todo excepto "< ..."
        }
        if (str[i] == '>'){
            out = &str[i+1];
            str[i] = '\0';
        }

        // Si encontramos posteriormente otro, volverá a entrar, priorizando el último redireccionamiento de cada tipo.
        
        in = strtrim(in);
        out = strtrim(out);
    }
}