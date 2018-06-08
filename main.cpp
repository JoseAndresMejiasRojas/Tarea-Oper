#include <iostream>

#include "controlador.h"

int main()
{
    Controlador controlador;
    std::cout << "/* message */" << '\n';
    controlador.multiplicacion_matrices();
    controlador.mostrar_matriz(controlador.matriz_a);
    std::cout << "}" << '\n';
    return 0;
}
