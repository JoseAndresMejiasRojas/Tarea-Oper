#ifndef CONTROLADOR_H
#define CONTROLADOR_H


class Controlador
{
public:

    int** matriz_a;
    int** matriz_b;
    Controlador();
    void multiplicacion_matrices();
    void mostrar_matriz(int** matriz);
    void mostrar_matriz_resultante(); 


private:
    void matriz_random( int** matriz, int tamano );
    void inicializar_matriz(int **matriz, int tamano );

    void guardar_fila( int** matriz, int fila, struct msgbuffer msg_enviar );
    void guardar_columna( int** matriz, int columna, struct msgbuffer msg_recibir );
    int calcular_escalar(struct msgbuffer msg_recibir);

    int shmid;
};

#endif // CONTROLADOR_H
