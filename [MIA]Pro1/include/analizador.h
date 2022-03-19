#ifndef ANALIZADOR_H
#define ANALIZADOR_H
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace std;

typedef struct
{
    char part_status='0';
    char part_type;
    char part_fit;
    int part_start=-1;
    int part_size=0;
    char part_name[16];
}tpart;

typedef struct
{
    int mbr_tamano;
    time_t mbr_fecha_creacion;
    int mbr_dsk_signature;
    char dsk_fit;
    tpart mbr_partition_1;
    tpart mbr_partition_2;
    tpart mbr_partition_3;
    tpart mbr_partition_4;
}tmbr;

typedef struct {
        char part_status = '0';
        char part_fit;
        int part_start;
        int part_size = 0;
        int part_next = -1;
        char part_name[16];
} tebr;

typedef struct {
        int pa;
        int ini;
        int fin;
        int atras;
        int adelante;
    } tenlace;


class analizador
{
    public:
        analizador();
        void analiza();
        virtual ~analizador();

    protected:

    private:
    tmbr MBR;
    vector <string> listaTokens(string entrada);
    vector <string> segmentos(string text);
    string comando(string entrada);
    void ejecutar(string comando, vector <string> tkns);
    void abrirArchivo(vector <string> tkns);
    void abrirArch2(string path);
    bool iguales(string cad1,string cad2);
    tmbr DameMBR(int tam,string uni,string f,int *byte);
    tpart DamePART(vector <string> par,int *byte);
    void Poner_Ceros(FILE *f,int comienzo,int cant);
    void HacerGrafica(string path);
    void Pausa();
    /*Creacion de disco*/
    void CrearDisco(vector <string> tkns);
    void Crear_Archivo(tmbr *m,string path,int tam);
    void BorrarDisco(vector <string> tkns);
    void AdminPart(vector <string> tkns);
    void Escribir_Particion(string path, tpart*p);


};

#endif // ANALIZADOR_H
