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
    int vector[TAMANO+TAMANO+2];
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
    // Memoria compartida: Matriz int TAMANOxTAMANO
    shmid = shmget(IPC_PRIVATE, 4*TAMANO*TAMANO, IPC_CREAT | 0600);

    // Dos semáforos.
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    struct sembuf operacionSemaforo;    // Semáforo

    // Cola de mensajes.
    int msgid = msgget(IPC_PRIVATE, IPC_CREAT| 0600);

    struct msgbuffer msg_enviar;

    for( size_t contador = 0; contador < 10; ++contador )
    {
        if(fork() != 0 )   // Lo que hace el padre
        {
            continue;
        }
        else
        {                   // Lo que hace el hijo.
            struct msgbuffer msg_recibir;
            operacionSemaforo.sem_num = 0;
            operacionSemaforo.sem_op = -1;
            operacionSemaforo.sem_flg = 0;
            for(int i = 0; i < 1000; ++i)
            {      // Cola de mensaje
              // wait al hijo.
              semop(semid, &operacionSemaforo, 1);

              msgrcv(msgid, &msg_recibir, (TAMANO*TAMANO+2)*sizeof(int), 1, 0);


              //msg_recibir()
            }

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
            //msg_enviar.mtext[0] = const_cast<char>(fila);
            //msg_enviar.mtext[1] = const_cast<char>(columna);


            // Obtengo y copio fila
            guardar_fila(matriz_a, fila, msg_enviar);

            // Obtengo y copio columna
            guardar_columna(matriz_b, columna, msg_enviar);
            msgsnd(msgid,(struct msgbuf*) &msg_enviar, (TAMANO*TAMANO+2)*sizeof(int), 0);
            // Copio vector columna
        }
    }

    // fork() hijo impresor.
}

void Controlador::guardar_fila( int** matriz, int fila, struct msgbuffer msg_enviar )
{
    // Aquí mismo copio al array (accediento al struct.array)
    for(int i = 0; i < TAMANO; ++i)
    {
      msg_enviar.vector[i+2] = matriz[fila][i];
    }
}

void Controlador::guardar_columna( int** matriz, int columna, struct msgbuffer msg_enviar )
{
  for(int i = 0; i < TAMANO; ++i)
  {
    msg_enviar.vector[i+2+TAMANO] = matriz[i][columna];
  }
}

int Controlador::calcular_escalar(struct msgbuffer recibir)
{
  int* p = (int*) shmat(shmid, NULL, 0);
  int resultado = 0;
  for(int i = 0; i < 100; ++i)
  {
    resultado += p[2+i]*p[TAMANO+2+i];
  }
  return resultado;
}

/*  Enviar mensaje
    sprintf(msg_enviar.mtext, "%s\n", "Este mensaje lo envía el padre.");
    msg_enviar.mtype = 1;
    msgsnd(msgid, &msg_enviar, 50, 0);
*/

/*  Recibir mensaje
    msgrcv(msgid, &msg_recibir, 50, 1, 0);
    printf("%d", msg_recibir.valor);
*/

/*
    // Mando señal a los hijos.
    operacionSemaforo.sem_num = 0;
    operacionSemaforo.sem_op = 1;
    operacionSemaforo.sem_flg = 0;
    semop(semid, &operacionSemaforo, 1);
*/
