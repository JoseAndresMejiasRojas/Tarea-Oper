#ifndef CONTROLADOR_H
#define CONTROLADOR_H


class Controlador
{
public:
    Controlador();
    void multiplicacion_matrices();


private:
    void matriz_random( int** matriz, int tamano );
    void inicializar_matriz(int **matriz, int tamano );

    void devolver_fila( int** matriz, int fila, struct msgbuffer msg_enviar );
    void devolver_columna( int** matriz, int columna, struct msgbuffer msg_recibir );


private:
    int** matriz_a;
    int** matriz_b;
};

#endif // CONTROLADOR_H
