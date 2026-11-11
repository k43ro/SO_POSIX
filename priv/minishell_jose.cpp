
//AUTOR:José Manuel Ros Rodrigo

#include <iostream> //Necesario para cout

#include <fstream> //Necesario para los guiones

#include <cstdlib> //Necesario para la función exit

#include <unistd.h>//Proporciona acceso a las llamadas al sistema a través de la API POSIX, como fork

#include <fcntl.h>

#include <cstring> //Necesario para la función strcpy

#include <sys/wait.h> //Neceasario para la función wait

#include <stdio.h>

using namespace std;

const int MAX_CARACTERES = 255; //Constantes globales
const int MAX_ELEMENTOS = 20;

void getredir(char * str, char * & in , char * & out, char * & err, bool & anadir) { //Función que proporciona las posibles redirecciones del mandato
        in = NULL;
        out = NULL;
        err = NULL;
        for (int i = 0; str[i] != '\0'; i++) {
                if (str[i] == '>' && str[i + 1] == '>') {
                        anadir = true;
                }
                if (str[i] == '2' && str[i + 1] == '>') {
                        err = & str[i + 2];
                        str[i] = '\0';
                        str[i + 1] = '\0';
                        i++;
                }
                if (str[i] == '>') {
                        out = & str[i + 1];
                        str[i] = '\0';
                }
                if (str[i] == '<') {
                        in = & str[i + 1];
                        str[i] = '\0';
                }
        }
}

char * trim(char * str) { //Función que elimina, de haberlos, los espacios en blanco que rodena a la cadena recibida 

        char * aux = str;
        if (str != NULL) {
                for (int i = 0; str[i] == ' '; i++) {

                        aux = & str[i + 1];

                }

                for (int i = strlen(aux) - 1; aux[i] == ' '; i--) {

                        aux[i] = '\0';

                }
                return aux;
        } else {
                return NULL;
        }
}

void trocear_linea(char linea[], const char delim[], const int maxElems, char * lista_elementos[]) { //Función que separa la linea en trozos
        char * trozo;
        int num = 0;
        while (linea != NULL && num < maxElems - 1) {
                trozo = strsep( & linea, delim);
                if (strcmp(trozo, "") != 0) {
                        lista_elementos[num++] = trozo;
                }
        }
        lista_elementos[num] = NULL;
}

bool contiene(char cadena[], char caracter) { //Función auxiliar que devuelve true si la cadena de caracteres cadena contiene a caracter, devuelve falso en caso contrario
        for (int i = 0; i < strlen(cadena); i++) {
                if (cadena[i] == caracter) {
                        return true;
                }
        }
        return false;
}

bool cortaCadena(char * cadena, char caracter){ //Función auxiliar que devuelve true si ha cortado la cadena de caracteres por caracter, devuelve falso en caso contrario
	if(contiene(cadena,caracter)){
		for(int i=0;i<strlen(cadena);i++){
			if(cadena[i]==caracter){
			
				cadena[i]='\0';
			
			}
	
		}
		return true;
	}
	return false;
}

void ejecutar(char mandato[]) {
        int fd,auxiliar = 0,v=0,w,tuberia[2];
        char * lista_arg[MAX_ELEMENTOS + 1];
        char * lista_aux[MAX_ELEMENTOS + 1];
        char * in , * out, * err;
        bool anadir = false , segundoplano=cortaCadena(mandato,'&'), comentario=cortaCadena(mandato,'#') ;

        trocear_linea(mandato, "|", MAX_ELEMENTOS, lista_arg);

        for (int i = 0; lista_arg[i] != NULL; i++) {

                getredir(lista_arg[i], in , out, err, anadir); //Separando el mandato de la entrada y de la salida

                out = trim(out); //Limpieza de la salida
                in = trim( in ); //Limpieza de la entrada
                err = trim(err); //Limpieza de la salida de errores

                trocear_linea(lista_arg[i], " \t", MAX_ELEMENTOS, lista_aux);

                if (strcmp(lista_aux[0], "cd") == 0) { //Implementación del comando cd
                        	if(lista_aux[1]==NULL || strcmp(lista_aux[1],"~")==0  || strcmp(lista_aux[1],"$HOME")==0 ){
                        		chdir(getenv("HOME"));
                        	}else{
                        		if(chdir(lista_aux[1])==-1){
                        			perror("ERROR: No se ha podido acceder al directorio");
                        		}
                        	}
                        
                } else {

                        if (pipe(tuberia) == -1) //el PADRE crea una tubería para compartir con sus hijos
                        {
                                perror("ERROR: No se ha podido crear la tubería");
                                exit(1);
                        }

						w=fork();//Guardamos el PID del hijo					
						
                        switch (w) { //creamos un hijo.
                        case -1:
                                perror("Error al crear un proceso");
                                exit(-2);
                                break;

                        case 0:

                                if (err != NULL) { // Redireccionando la salida de errores
                                        close(2);          
                                        if(creat(err, 0666)==-1){
                                        	perror("ERROR: No se ha podidio crear el fichero de salida de errores");
                                			exit(4);
                                        }

                                }
                                if (out != NULL) { //Redireccionando la salida
                                        close(1);
                                        if (anadir) {

                                                if(open(out, O_WRONLY | O_APPEND | O_CREAT, 0644)==-1){ //Añadir al archivo por el final
                                                	perror("ERROR: No se ha podidio crear el fichero de salida");
                                					exit(4);
                                                }

                                        } else {

                                               if(open(out, O_WRONLY | O_CREAT, 0644)==-1){ //Redireccionar la salida a un fichero, si existe lo pisará, sino lo creará
                                                	perror("ERROR: No se ha podidio crear el fichero de salida");
                                					exit(4);
                                                }

                                        }
                                }
                                if ( in != NULL) { //Redireccionando la entrada
                                        close(0);
                                        if(open( in , O_RDONLY)==-1){
                                          perror("ERROR: No se ha podidio acceder al fichero de entrada");
                                		  exit(4);
                                        }
                                }


								if (dup2(auxiliar, 0) == -1) //cerramos la entrada estándar y leemos por auxiliar(extremo de escritura del mandato anterior)
                                        {
                                                perror("Error al redireccionar stdin del mandato");
                                                exit(2);
                                        }

                                if (lista_arg[i + 1] != NULL) { //miramos si existe el siguiente comando

                                        if (dup2(tuberia[1], 1) == -1) // cerramos la salida estándar y la salida ahora es el extremo de lectura de la pipe
                                        {
                                                perror("Error al redireccionar stdout del mandato");
                                                exit(2);
                                        }

                                }

                                close(tuberia[0]); //cerramos el extremo de escritura

                                execvp(lista_aux[0], lista_aux); //ejecutamos el comando
                                perror("Error al ejecutar el mandato ");
                                exit(3);

                        default:
                        		if(!segundoplano){ //Miramos si queremos ejecutar el mandato en segundo plano
                                	wait(NULL); //El padre espera a que acabe el hijo
                                }else{
                                	if(v==0){
                                	cout<< "[1] " <<w<<endl; //Devolvemos el PID del proceso en segundo plano
                                	v++;
                                	}
                                }
                                
                                close(tuberia[1]); //Cierra el extremo de lectura
                                auxiliar = tuberia[0]; //Guarda en auxiliar el extremo de escritura 

                        }
                }
        }

}

void prompt() { //Función que proporciona el prompt de la minishell
        char usuario[1000], host[1000], directorio[1000];
        getcwd(directorio, 1000);
        getlogin_r(usuario, 1000);
        gethostname(host, 1000);
        cout << "\033[1;36m" << usuario << "@" << host << ":" << "\033[1;31m" << directorio << "\033[0m$ ";
}

void multicomando(char comando[]) { //Función que permite ejecutar varios comandos separados por ;
        char * lista_comandos[MAX_ELEMENTOS + 1];
        trocear_linea(comando, ";", MAX_ELEMENTOS, lista_comandos);
        for (int i = 0; lista_comandos[i] != NULL; i++) {

                if (strcmp(trim(lista_comandos[i]), "salir") == 0) {

                        cout << "\033[1;33m" << "Hasta pronto." << " \033[0m\n";
                        exit(0);

                } else {
                        ejecutar(lista_comandos[i]);
                }
        }

}

int main(int argc, char * argv[]) {
        char comando[MAX_CARACTERES + 1];
        if (argc == 1) {
                cout << "\033[1;33m" << "Bienvenid@ a la minishell, escribe 'salir' si quieres salir." << " \033[0m\n";
                prompt();
                cin.getline(comando, MAX_CARACTERES + 1, '\n');
                while (strcmp(trim(comando), "salir") != 0) {
                        if (contiene(comando, ';')) {
                                multicomando(comando);
                        } else {

                                ejecutar(comando);
                        }     
                        prompt();
                        cin.getline(comando, MAX_CARACTERES, '\n');
                }
        } else {
                cout << "\033[1;33m" << "Bienvenid@ a la minishell, esta es la versión de ejecución de guiones." << " \033[0m\n";
                for (int i = 1; i < argc; i++) {
                        ifstream entrada;
                        entrada.open(argv[i]);
                        if (entrada.fail()) {
                                cerr << "No puede abrirse el archivo " << argv[i] << endl;
                                exit(2);
                        } else {

                                entrada.getline(comando, MAX_CARACTERES, '\n');
                                while (!entrada.eof() && strcmp(trim(comando), "salir") != 0) {
                                        if (contiene(comando, ';')) {
                                                multicomando(comando);
                                        } else {
                                                ejecutar(comando);
                                        }
                                        entrada.getline(comando, MAX_CARACTERES, '\n');

                                }
                                entrada.close();
                        }
                }
        }
        cout << "\033[1;33m" << "Hasta pronto." << " \033[0m\n";
        exit(0);
}
