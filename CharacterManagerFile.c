#include <stdio.h>
#include <stdlib.h>
#include <locale.h> //Para usar setlocale
#include <time.h>

void eliminarPersonajes(juegosLocales);
void eliminarPersonajeEleccion(conjuntoPersonajes);
void mostrarPersonajes(conjuntoPersonajes);
void llenarConjuntoPersonajesInicio(conjuntoPersonajes);
void llenarConjuntoPersonajes(conjuntoPersonajes);


typedef enum{ RPG = 1, gacha, shooter, terror} generoJuego;

typedef struct{
    char * nombrePersonaje;
    int anoJuego;
    generoJuego genero;
}Juego;

typedef struct{
    Juego * juegos;
    int numeroJuegos;
} conjuntoPersonajes;

void llenarConjuntoPersonajes(conjuntoPersonajes * juegosLocales) //Paso por referencia pq vamos a modificar
{
    int cantidadNuevaJuegos;
    printf("\n\t¿Cuantos personajes nuevos quieres agregar? -> ");
    scanf("%d", &cantidadNuevaJuegos);

    // Reasignar memoria para contener los personajes anteriores y los nuevos
    juegosLocales->juegos = (Juego *)realloc(juegosLocales->juegos, (juegosLocales->numeroJuegos + cantidadNuevaJuegos) * sizeof(Juego));

    // Llenado de los nuevos personajes
    for (int i = juegosLocales->numeroJuegos; i < juegosLocales->numeroJuegos + cantidadNuevaJuegos; i++) {
        printf("\n\tDame el año de salida del juego del personaje %d: ", i + 1);
        scanf("%d", &(juegosLocales->juegos[i].anoJuego));

        printf("\n\tDame el género del juego del personaje %d (de 1 a 4) : ", i + 1);
        scanf("%d", &(juegosLocales->juegos[i].genero));

        char cadenaAuxiliar[100];
        fflush(stdin);
        printf("\n\tDame el nombre del personaje: ");
        gets(cadenaAuxiliar);

        // Reservamos memoria para el nombre del nuevo personaje
        juegosLocales->juegos[i].nombrePersonaje = (char *)malloc((strlen(cadenaAuxiliar) + 1) * sizeof(char));
        strcpy(juegosLocales->juegos[i].nombrePersonaje, cadenaAuxiliar);
    }

    // Actualizo el número total de personajes
    juegosLocales->numeroJuegos += cantidadNuevaJuegos;
    printf("\n\tPersonajes añadidos con exito, recuerde guardarlos en el fichero");
}

void llenarConjuntoPersonajesFichero(conjuntoPersonajes * juegosLocales)
{
    FILE * flectura = fopen("personajes.txt", "r");
    if (flectura == NULL) {
        printf("Error al abrir el archivo de lectura");
        return;
    }

    while (!feof(flectura)) {
        char nombrePersonaje[100];
        int anoJuego;
        int genero;

        if(fscanf(flectura, "%s %d %d", nombrePersonaje, &anoJuego, &genero) !=3) break;

        juegosLocales->juegos = (Juego *)realloc(juegosLocales->juegos, (juegosLocales->numeroJuegos + 1) * sizeof(Juego));

        juegosLocales->juegos[juegosLocales->numeroJuegos].nombrePersonaje = (char *)malloc((strlen(nombrePersonaje) + 1) * sizeof(char));
        strcpy(juegosLocales->juegos[juegosLocales->numeroJuegos].nombrePersonaje, nombrePersonaje);

        juegosLocales->juegos[juegosLocales->numeroJuegos].anoJuego = anoJuego;
        juegosLocales->juegos[juegosLocales->numeroJuegos].genero = genero;

        juegosLocales->numeroJuegos++;
    }

    fclose(flectura);
}

void mostrarPersonajes(conjuntoPersonajes juegosLocales)
{
    int i = 0;

    if (juegosLocales.numeroJuegos <= 0) {
        printf("\n\tNo hay personajes disponibles.\n");
        return;
    }

    printf("\n\n\tVoy a mostrar el set de personajes: ");
    for (i=0; i<juegosLocales.numeroJuegos;i++){

        printf("\n\n\tNOMBRE: %s", juegosLocales.juegos[i].nombrePersonaje);
        printf("\n\tAÑO: %d", juegosLocales.juegos[i].anoJuego);
        printf("\n\tGÉNERO: ");

        switch (juegosLocales.juegos[i].genero)
        {
            case 1:
            printf("RPG");
            break;
            case 2:
            printf("gacha");
            break;
            case 3:
            printf("shooter");
            break;
            case 4:
            printf("terror");
            break;
        }
    }
}

void eliminarPersonajeEleccion(conjuntoPersonajes * juegosLocales)
{
    int numeroPersonaje;

    if (juegosLocales->numeroJuegos == 0) {
        printf("\n\tNo hay personajes disponibles.\n");
        return;
    }

    //hacemos una lista para mostrar los personajes a borrar
    printf("\n\tPersonajes disponibles:\n");
    for (int i = 0; i < juegosLocales->numeroJuegos; i++) {
        printf("\t%d. %s\n", i + 1, juegosLocales->juegos[i].nombrePersonaje);
    }

    printf("\n\t¿Que personaje desea eliminar?: ");
    scanf("%d", &numeroPersonaje);

    if (numeroPersonaje <= 0 || numeroPersonaje > juegosLocales->numeroJuegos) {
        printf("\n\tNo existe ese personaje\n");
        return;
    }

    numeroPersonaje--; //Reducimos el numero de personajes porque lo hemos eliminado

    // Liberar memoria dinámica del nombre del personaje a borrar
    free(juegosLocales->juegos[numeroPersonaje].nombrePersonaje);
    juegosLocales->juegos[numeroPersonaje].nombrePersonaje = NULL;

    // Mover los personajes restantes para llenar el espacio del personaje eliminado
    for (int i = numeroPersonaje; i < juegosLocales->numeroJuegos - 1; i++) {
        juegosLocales->juegos[i] = juegosLocales->juegos[i + 1];
    }

    // Disminuir el número total de personajes
    (juegosLocales->numeroJuegos)--;

    // Reasignar memoria dinámica para el conjunto de personajes
    juegosLocales->juegos = (Juego *)realloc(juegosLocales->juegos, juegosLocales->numeroJuegos * sizeof(Juego));

    printf("\n\tPersonaje eliminado exitosamente. Recuerde guardar los cambios en el fichero\n");
}

void eliminarPersonajes( conjuntoPersonajes * juegosLocales)
{
    int i = 0;

    if (juegosLocales->numeroJuegos == 0) {
        printf("\n\tNo hay personajes disponibles.\n");
        return;
    }

    //Elimino la memoria dinámica de cada una de los personajes
    for (i=0; i<juegosLocales->numeroJuegos; i++)
    {
    if (juegosLocales->juegos[i].nombrePersonaje != NULL)
        {
        free(juegosLocales->juegos[i].nombrePersonaje);
        juegosLocales->juegos[i].nombrePersonaje = NULL;
        }
    }

    //Eliminar la memoria dinñamica del vector que contiene todos los personajes
    if (juegosLocales->juegos != NULL)
    {
    free(juegosLocales->juegos);
    juegosLocales->juegos = NULL;
    }

    // Actualizo el número de personajes a cero
    juegosLocales->numeroJuegos = 0;

    printf("\n\n\t\tTODOS LOS PERSONAJES HAN SIDO ELIMINADOS. Recuerde guardar los cambios en el fichero\n");
}

void guardarDatosFichero(conjuntoPersonajes * juegosLocales){

    FILE * fescritura = fopen("personajes.txt", "w");

    if (fescritura == NULL) {
        printf("Error al abrir el archivo de escritura");
        return;
    }

    for (int i = 0; i < juegosLocales->numeroJuegos; i++) {
        fprintf(fescritura, "%s %d %d\n", juegosLocales->juegos[i].nombrePersonaje, juegosLocales->juegos[i].anoJuego, juegosLocales->juegos[i].genero);
    }

    fclose(fescritura);
    printf("\n\tDatos guardados en el fichero");

}

int main(int argc, char *argv[])
{
    srand(time(NULL));//Semilla para numero aleatorio para incializar tres personajes
    setlocale(LC_ALL,""); // Configurar la localización para UTF-8 para poner la ñ y acentos
    int eleccion = 0;

    conjuntoPersonajes misPersonajes = {NULL , 0}; //Declaro mi estructra de personajes
    llenarConjuntoPersonajesFichero(&misPersonajes); //Le paso mis personajes por referencia;
    do {
        printf("\n\n\t¿Que desea hacer?\n\tBorrar personaje = 1\n\tAñadir personaje = 2\n\tListar personajes = 3\n\tEliminar todos los personajes = 4\n\tGuardar personajes en fichero = 5\n\tSalir = 6\n\tElección->");
        scanf("%d",&eleccion);

        switch (eleccion){
            case 1:
                eliminarPersonajeEleccion(&misPersonajes);
                break;
            case 2:
                llenarConjuntoPersonajes(&misPersonajes);
                break;
            case 3:
                mostrarPersonajes(misPersonajes);
                break;
            case 4:
                eliminarPersonajes(&misPersonajes);
                break;
            case 5:
                guardarDatosFichero(&misPersonajes);
                break;
            case 6:
                exit;
            break;
        }
    } while (eleccion != 6);
    return 0;
}
