#include "stdlib.h"
#include "stdio.h"

#include <unistd.h>
#include <sys/ipc.h> // Memoria compartida
#include <sys/shm.h> // Memoria compartida
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/msg.h>

#define TAMANO 100 //Si cambia el tamano a 10, favor cambiar la condicion del for, esta comentado abajo

struct msgbuf
{
    long mtype;
    char mtext[TAMANO+TAMANO+2];
};

int** matriz_a;
int** matriz_b;
int shmid;
void* p;

void multiplicacion_constructor();
void multiplicacion_matrices();
void mostrar_matriz(int** matriz);
void mostrar_matriz_resultante(int* memoria_compartida);
void matriz_random( int** matriz, int tamano );
void inicializar_matriz(int **matriz, int tamano, int semilla);
void guardar_fila( int** matriz, int fila, struct msgbuf* msg_enviar );
void guardar_columna( int** matriz, int columna, struct msgbuf* msg_recibir );
void calcular_escalar(char* mtext);

union semun
{
    int val;
    struct semid_ds* buf_stat;
    unsigned short* array;
    struct seminfo* buf_info;
};

void multiplicacion_constructor()
{
    //matriz_a = new int*[TAMANO];
    int semilla = 0;
    matriz_a = (int**) malloc(sizeof(int*)*TAMANO);
    inicializar_matriz(matriz_a, TAMANO, semilla);
    matriz_random(matriz_a, TAMANO);
    mostrar_matriz(matriz_a);
    
    matriz_b = (int**) malloc(sizeof(int*) * TAMANO);
    inicializar_matriz(matriz_b, TAMANO, semilla);
    matriz_random(matriz_b, TAMANO);
    mostrar_matriz(matriz_b);
}

void inicializar_matriz(int** matriz, int tamano, int semilla)
{
    srand(semilla);
    for( int contador = 0; contador < tamano; ++contador )
    {
        for( int otro_contador = 0; otro_contador < tamano; ++otro_contador )
        {
            matriz[contador] = (int*) malloc(sizeof(int) * tamano);
        }
    } 
}


void matriz_random(int **matriz, int tamano )
{
    for( int contador = 0; contador < tamano; ++contador )
    {
        for( int otro_contador = 0; otro_contador < tamano; ++otro_contador )
        {
            matriz[contador][otro_contador] = rand() % 100;
        }
    }
}


// Aquí iniciamos con los procesos :)
void multiplicacion_matrices()
{
    // Memoria compartida: Matriz int TAMANOxTAMANO
    shmid = shmget(IPC_PRIVATE, sizeof(int)*TAMANO*TAMANO, IPC_CREAT | 0600);

    // Dos semáforos.
    int semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    struct sembuf operacionSemaforo;    // Semáforo

    // Cola de mensajes.
    int msgid = msgget(IPC_PRIVATE, IPC_CREAT| 0600);

    struct msgbuf msg_enviar;
    struct msgbuf msg_recibir_padre;

    for( size_t contador = 0; contador < 10; ++contador )
    {
        if(fork() != 0)   // Lo que hace el padre
        {
            continue;
        }
        else
        {                   // Lo que hace el hijo.
            struct msgbuf msg_recibir;
            // Si TAMANO =  100, la condición es 1000 o TAMANO*10.
            //Para TAMANO = 10^n con n >= 2, la condicion del for es TAMANO*10
            //Si se va a usar TAMANO = 10, CAMBIAR CONDICION DE FOR A i < 100
            for(int i = 0; i < TAMANO*10; ++i)
            {
                // Recibo el mensaje enviado por el padre.
                msgrcv(msgid, &msg_recibir, (TAMANO+TAMANO+2), 1, 0);
                calcular_escalar(msg_recibir.mtext);
            }
            //Decirle al padre ya termine
            operacionSemaforo.sem_num = 0;
            operacionSemaforo.sem_op = 1;
            operacionSemaforo.sem_flg = 0;
            semop(semid, &operacionSemaforo, 1);
            exit(0);
        }
    }
    // Creados los 10 hijos

    // Padre envía info a sus hijos.
    for( size_t fila = 0; fila < TAMANO; ++fila )
    {
        for( size_t columna = 0; columna < TAMANO; ++columna )
        {
            msg_enviar.mtype = 1;
            // Copio las posiciones (fila, columna) al array.
            msg_enviar.mtext[0] = fila;
            msg_enviar.mtext[1] = columna;

            // Obtengo y copio fila
            guardar_fila(matriz_a, fila, &msg_enviar);

            // Obtengo y copio columna
            guardar_columna(matriz_b, columna, &msg_enviar);
            msgsnd(msgid, &msg_enviar, (TAMANO+TAMANO+2), 0);
        }
    }
        operacionSemaforo.sem_num = 0;
        operacionSemaforo.sem_op = -1;
        operacionSemaforo.sem_flg = 0;
    for( size_t columna = 0; columna < 10; ++columna )
    {
        semop(semid, &operacionSemaforo, 1);
    }

    // fork() hijo impresor.
    if(fork() != 0)
    {
        return;
    }
    else
    {
        int* memoria_compartida = (int*) (shmat(shmid, NULL, 0));
        mostrar_matriz_resultante(memoria_compartida);
    }
}

void guardar_fila( int** matriz, int fila, struct msgbuf* msg_enviar )
{
    // Aquí mismo copio al array (accediento al struct.array)
    for(int i = 0; i < TAMANO; ++i)
    {
        msg_enviar->mtext[i+2] = matriz[fila][i];
    }
}

void guardar_columna( int** matriz, int columna, struct msgbuf* msg_enviar )
{
    for(int i = 0; i < TAMANO; ++i)
    {
        msg_enviar->mtext[i+2+TAMANO] = matriz[i][columna];
    }
}

void calcular_escalar(char* mtext)
{
    int* memoria_compartida = (int*) shmat(shmid, NULL, 0);
    int resultado = 0;
    for(int i = 0; i < TAMANO; ++i)
    {
        resultado += mtext[2+i]*mtext[TAMANO+2+i];  // Hago la multiplicacion con el vector de la cola.
    }
    // Guardar como matriz: (fila*TAMANO)+columna
    int posicion = (int)( (mtext[0]*TAMANO) + mtext[1] );
    memoria_compartida[posicion] = resultado;
}

void mostrar_matriz(int** matriz)
{
    for (size_t i = 0; i < TAMANO; i++)
    {
        for (size_t j = 0; j < TAMANO; j++)
        {
            printf("%d ", matriz[i][j]);
        }
        putchar('\n');
    }
}

void mostrar_matriz_resultante(int* memoria_compartida)
{
    for(int i = 0; i < TAMANO; i++){
        for(int j = 0; j < TAMANO; j++){
            printf(" ");
            printf("%d", memoria_compartida[i*j]);
            //std::cout << memoria_compartida[i*j];
        }
        putchar('\n');
    }
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
