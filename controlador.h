#ifndef CONTROLADOR_H
#define CONTROLADOR_H


class Controlador
{
public:
    Controlador();
    void multiplicacion_matrices();


private:
    void matriz_random( unsigned short** matriz, int tamano );
    void inicializar_matriz( unsigned short** matriz, int tamano );


private:
    unsigned short** matriz_a;
    unsigned short** matriz_b;
};

#endif // CONTROLADOR_H
