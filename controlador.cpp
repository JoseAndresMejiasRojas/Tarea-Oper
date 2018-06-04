#include "controlador.h"

#include <iostream> // MISMA MIERDA QUE FORK
#include <unistd.h> // Reconoce el fork aquí, no en el main.cpp

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
    int id_procesos;
    // 11 procesos hijos + el padre.
    for( size_t contador = 0; contador < 11; ++contador  )
    {
        id_procesos = fork();
    }


    // 12 procesos en total a partir de acá.
    if( id_procesos != 0 )  // Padre
    {
    }
    else                    // Hijos
    {
    }
}
