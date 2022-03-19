#include "codigo.h"
#include "analizador.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

codigo::codigo()
{
    //ctor
}

void codigo::Menu()
{
    //this->Crear_Archivo();
   // int opc=0;
        analizador a;
        a.analiza();

}


void codigo::Crear_Archivo()
{
    try
    {
        FILE *f1,*f2;
        f1= fopen("profesores.bin","wb");
        f2= fopen("alumnos.bin","wb");
        fclose(f1);
        fclose(f2);
    }
    catch(int i)
    {
        cout<<"Existe un error al crear el archivo"<< i<<endl;
        this->Pausa();
    }
}

void codigo::Poner_Ceros(FILE *f)
{
    char buffer[10];
    int leidos=0;
    while(leidos<10)
    {
        buffer[leidos]=0;
        leidos++;
    }
    try
    {
        fwrite (buffer, 1, leidos, f);
    }
    catch(int i)
    {
        cout<<"error en archivos al poner ceros"<<endl;
    }

}

void codigo::Pausa()
{
    cout << "Enter to continue..." <<endl;
    cin.ignore();
    cin.get();
    system("clear");
}


codigo::~codigo()
{
    //dtor
}
