#ifndef CODIGO_H
#define CODIGO_H

#include <string>
using namespace std;


class codigo
{
    public:
        codigo();
       void Menu();
        virtual ~codigo();

    protected:

    private:

    void Crear_Archivo();
    void Poner_Ceros(FILE * f);
    void Escribir_Archivo(void *dato,int opc);
    void Imprimir_Datos(int opc);
    void Pausa();
};


#endif // CODIGO_H
