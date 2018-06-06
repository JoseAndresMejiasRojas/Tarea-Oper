#include "controlador.h"

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <sys/ipc.h> // Memoria compartida
#include <sys/shm.h> // Memoria compartida
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/msg.h>


#define TAMANO 100

// Para la cola de mensajes.
//ToDo: definir cómo se va enviar la información a los procesos hijos.
struct msgbuffer
{
    long mtype;
    char mtext[202];
};

union semun
{
    int val;
    struct semid_ds* buf_stat;
    unsigned short* array;
    struct seminfo* buf_info;
};

Controlador::Controlador()
{
    matriz_a = new int*[TAMANO];
    inicializar_matriz(matriz_a, TAMANO);
    matriz_random(matriz_a, TAMANO);

    matriz_b = new int*[TAMANO];
    inicializar_matriz(matriz_b, TAMANO);
    matriz_random(matriz_b, TAMANO);
}

void Controlador::inicializar_matriz(int** matriz, int tamano)
{
    srand(0);// Mando señal a los hijos.
    for( int contador = 0; contador < tamano; ++contador )
    {
        for( int otro_contador = 0; otro_contador < tamano; ++otro_contador )
        {
            matriz[contador] = new int[tamano];
        }
    }
}


void Controlador::matriz_random(int **matriz, int tamano )
{
    for( int contador = 0; contador < tamano; ++contador )
    {
        for( int otro_contador = 0; otro_contador < tamano; ++otro_contador )
        {
            matriz[contador][otro_contador] = rand() % 100;
        }// Mando señal a los hijos.
    }
}

// Aquí iniciamos con los procesos :)
void Controlador::multiplicacion_matrices()
{
    // Memoria compartida: Matriz int 100x100
    int shmid = shmget(IPC_PRIVATE, 4*100*100, IPC_CREAT | 0600);

    // Dos semáforos.
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    struct sembuf operacionSemaforo;    // Semáforo

    // Cola de mensajes.
    int msgid = msgget(IPC_PRIVATE, IPC_CREAT| 0600);
    struct msgbuffer msg_enviar;


    for( size_t contador = 0; contador < 10; ++contador )
    {
        if( fork() != 0 )   // Lo que hace el padre
        {




            continue;
        }
        else
        {                   // Lo que hace el hijo.
            struct msgbuffer msg_recibir;          // Cola de mensaje

            // wait al hijo.
            operacionSemaforo.sem_num = 0;
            operacionSemaforo.sem_op = -1;
            operacionSemaforo.sem_flg = 0;
            semop(semid, &operacionSemaforo, 1);

            std::cout << "Hijo" << std::endl;

            exit(0);
        }
    }

    // Creados los 10 hijos.

    // Padre envía info a sus hijos.
    for( size_t fila = 0; fila < TAMANO; ++fila )
    {
        for( size_t columna = 0; columna < TAMANO; ++columna )
        {
            // Copio la posición (fila, columna) al array.

            // Obtengo y copio fila
            devolver_fila(matriz_a, fila);

            // Obtengo y copio columna
            devolver_columna(matriz_a, columna);

            // Copio vector columna
        }
    }


/*
    // Mando señal a los hijos.
    operacionSemaforo.sem_num = 0;
    operacionSemaforo.sem_op = 1;
    operacionSemaforo.sem_flg = 0;
    semop(semid, &operacionSemaforo, 1);
    */


    // fork() hijo impresor.
}

int* Controlador::devolver_fila( int** matriz, int fila )
{
    // Aquí mismo copio al array (accediento al struct.array)
    // NECESITAMOS TENER POR PARÁMETRO EL STRUCT msg_enviar.
}

int* Controlador::devolver_columna( int** matriz, int columna )
{

}
