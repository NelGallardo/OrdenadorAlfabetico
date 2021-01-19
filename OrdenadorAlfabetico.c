/*
*   Programa que ordena alfabéticamente las palabras que se encuentran en
*   un fichero indicado por el usuario.
*   Crea otro fichero con las palabras ordenadas.
*/


/*** LIBRERIAS ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*** PROTOTIPOS ***/
// Lee desde la entrada estándar (teclado) una cadena de caracteres.
    // Postcondicion: Devuelve una cadena de caracteres.
char* LeerCadenaTeclado();

// Guarda las palabras del fichero en una matriz que devuelve al main.
    // Precondicion: fichero de origen de las palabras, la direccion de un entero
    //  tal que *totPal = 0 y el caracter separador.
    // Postcondicion: Devuelve una matriz con todas las palabras que ha leído del
    //  fichero.
char** AlmacenarPalabras(FILE *fp, int *totPal, char separador);

// Leer una única línea del fichero indicado.
    // Precondicion: fichero desde donde vamos a leer y el caracter separador de las
    // palabras.
    // Postcondicion: devuelve la palabra leída.
char* LeerPalabraFich(FILE *fp, char separador);

// Ordena las palabras y las escribe líena a línea en el fichero palabras_ordenadas.txt.
    // Precondicion: Matriz con todas las palabras y tope_matriz <- cantidad de
    //  palabras.
void OrdenarPalabras(char **palabras, int tope_matriz);


/*** FUNCIÓN PRINCIPAL ***/
int main(int argc, char *argv[]){
    if(argc != 1){
        printf("\n\tDatos mal introducidos");
        exit(-1);
    }
    else{
        FILE *fp;
        char *nomfich, **palabras, separador;
        int totPal=0;

        // Información sobre lo que realiza el programa
        printf("\n\tEste programa ordena alfab%cticamente las palabras de un fichero",130);
        printf("\n\ty las escribe en otro fichero llamado \"palabras_ordenadas.txt\"");

        // Pedimos el nombre del fichero al usuario
        printf("\n\n\tIntroduce el nombre del fichero junto con la extensi%cn: ",162);
            fflush(stdin);
            nomfich = LeerCadenaTeclado();

        printf("\n\tIntroduce el separador de las palabras en el fichero: ");
            fflush(stdin);
            scanf("%c",&separador);

        // Abrimos el fichero
        fp = fopen(nomfich,"r");
        if(fp == NULL){
            printf("\n\tError al abrir el fichero");
            exit(-1);
        }

        free(nomfich);

        // Almacenamos las palabras en la matriz y la cantidad que hay
        palabras = AlmacenarPalabras(fp,&totPal,separador);

        fclose(fp);

        // Ponemos las palabras ordenadas en el fichero de salida
        OrdenarPalabras(palabras,totPal);

        system("pause");

        return 0;
    }
}


/*** FUNCIONES ***/
char* LeerCadenaTeclado(){
    char *cad,c;
        cad = (char*)malloc(sizeof(char));
    int i=0;

    // Bucle para obtener los caracteres desde el teclado
    while( (c = getchar()) != '\n'){
        // Almacenamos el caracter
        cad[i] = c;
        // Incrementamos el índice
        i++;
        // Incrementamos el tamaño de la cadena
        cad = (char*)realloc(cad,(i+1)*sizeof(char));
    }
    // Guardamos el \0 en la última posición
    cad[i] = '\0';

    // Devolvemos la cadena de caracteres
    return cad;
}

char** AlmacenarPalabras(FILE *fp, int *totPal, char separador){
    char **m;
    int i=0;

    m = (char**)malloc(sizeof(char*));

    while(!feof(fp)){
        // Reservamos el espacio para la siguiente palabra.
        m = (char**)realloc(m,(i+1)*sizeof(char*));

        // Almacenamos la palabra
        m[i] = LeerPalabraFich(fp,separador);

        i++;
    }
    // Cuando salgamos, i tendrá por valor el indice de la ultima posición+1 \
        por tanto tendremos el total de palabras. Lo guardamos en *totPal
    *totPal = i;

    return m;
}

char* LeerPalabraFich(FILE *fp, char separador){
    char *palabra,c;
        palabra = (char*)malloc(sizeof(char));
    int i=0;

    // Leemos la línea completa. El último caracter puede ser un \n o EOF en \
        función de la línea en la que nos encontremos.
    while( (c = fgetc(fp)) != '\n' && c != EOF && c != separador ){
        palabra[i] = c;
        i++;
        palabra = (char*)realloc(palabra,(i+1)*sizeof(char));
    }
    palabra[i] = '\0';

    return palabra;
}

void OrdenarPalabras(char **palabras, int tope_matriz){
    FILE *fp;
        fp = fopen("palabras_ordenadas.txt","a");
        if(fp == NULL){
            printf("\n\tError al abrir fichero");
            exit(-1);
        }
    int pos = 0, palabras_rest = tope_matriz, i = 0, j = 0, ind_escr, ext = 0;
        /*
        *   pos: índice en la matriz
        *   palabras_rest: palabras que nos quedan por revisar.
        *   i: índice para comparar las palabras con pos.
        *   j: segundo índice para comparar letra a letra.
        *   ind_escr: posición de la palabra que será escrita en siguiente lugar.
        *   ext: variable para salir del bucle cuando ya tenemos una palabra elegida
        */

    while(pos < tope_matriz && palabras_rest > 0){
        // Si la cadena es una nula avanzamos a la siguiente posición.
        if(palabras[pos][0] == '\0'){
            pos++;
        }
        // Sino empezamos a buscar cual será la palabra a escribir
        else{
            // En principio escribiremos la palabra de la posición actual
            ind_escr = pos;
            // Tomamos la posición de la siguiente palabra
            i = pos + 1;

            // Recorremos la matriz comparando las palabras
            while(i < tope_matriz){
                // Si la cadena es una nula avanzamos a la siguiente posición.
                if(palabras[i][0] != '\0'){
                    j = 0;
                    ext = 0;
                    // Buscamos la siguiente palabra que tenemos que escribir
                    while(palabras[i][j] != '\0' && palabras[ind_escr][j] != '\0' && ext == 0){
                        // Si la letra de la palabra i es menor que la de la palabra ind_escr \
                            guardamos la posición i y salimos
                        if(tolower(palabras[ind_escr][j]) > tolower(palabras[i][j])){
                            ind_escr = i;
                            ext = 1;
                        }
                        else{
                            // Si la letra de la palabra ind_escr es menor que la de la palabra i salimos
                            if(tolower(palabras[ind_escr][j]) < tolower(palabras[i][j])){
                                ext = 1;
                            }
                            // Si son iguales comparamos la siguiente posición
                            else{
                                j++;
                            }
                        }
                    }
                    // Caso para cuando sale del bucle con ext igual a 0. Es decir,
                    // una cadena es más corta que otra pero está contenida en esa
                    if(ext == 0){
                        if(strlen(palabras[ind_escr]) > strlen(palabras[i])){
                            ind_escr = i;
                        }
                    }
                }
                i++;
            } // while. Recorrido de matriz comparando

            // Salimos con la palabra a escribir en ind_escr
            fputs(palabras[ind_escr],fp);
            fputc('\n',fp);

            // Eliminamos la palabra ya escrita y la sustituimos por "\0"
            palabras[ind_escr] = (char*)realloc(palabras[ind_escr],sizeof(char));
            palabras[ind_escr][0] = '\0';
            // Quitamos una palabra al total
            palabras_rest--;

            // Aquí pos no aumenta porque puede ser que la palabra escrita no \
                fuese la que teniamos en un principio.
        } // else
    } // while. Recorrido de la matriz con pos

    // Para separar las diferentes ordenaciones
    fprintf(fp,"---------------------------------------------------\n");

    printf("\n\t--Fichero con las palabras ordenadas creado--\n\n");

    fclose(fp);
}
