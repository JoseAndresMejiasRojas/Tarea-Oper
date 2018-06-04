#include "controlador.h"

#include <stdlib.h>
#include <iostream>
#include <unistd.h>

#define TAMANO 100

Controlador::Controlador()
{
    matriz_a = new unsigned short*[TAMANO];
    inicializar_matriz(matriz_a, TAMANO);
    matriz_random(matriz_a, TAMANO);

    matriz_b = new unsigned short*[TAMANO];
    inicializar_matriz(matriz_b, TAMANO);
    matriz_random(matriz_b, TAMANO);



}

void Controlador::inicializar_matriz(unsigned short** matriz, int tamano)
{
    for( int contador = 0; contador < tamano; ++contador )
    {
        matriz[contador] = new unsigned short[tamano];
    }
}


void Controlador::matriz_random( unsigned short** matriz, int tamano )
{
    for( int contador = 0; contador < tamano; ++contador )
    {
        for( int otro_contador = 0; otro_contador < tamano; ++otro_contador )
        {
            matriz[contador][otro_contador] = 10;//Falta hacerlo random.
        }
    }
}

// Aquí iniciamos con los procesos :)
void Controlador::multiplicacion_matrices()
{
    for( size_t contador = 0; contador < 10; ++contador )
    {
        if( fork() != 0 )
        {
            // Lo que hace el padre, como hay 10 iteraciones, pasa 10 veces por acá.
            continue;
        }
        else
        {
            // Lo que hace el hijo.

            exit(0);        // Termino ejecución del hijo.
        }
    }
}
