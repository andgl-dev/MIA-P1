#include "analizador.h"
#include <iomanip>
#include <ctime>
#include <cstring>

int inicio_global;

analizador::analizador()
{
    //ctor
}

//Inicia el analisis donde esperamos un comando
void analizador::analiza()
{
    string entrada;
    string comand;
    vector <string> parametros;
    while(true)
    {
        cout<<"Osman Guacamaya   201504354"<<endl;
        cout<<"Ingrese un comando o exit para salir"<<endl;
        getline(cin,entrada);
        if(iguales(entrada,"EXIT"))
        {
            break;
        }

        comand = comando(entrada);

        entrada.erase(0,comand.length()+1);

        parametros = segmentos(entrada);
        ejecutar(comand,parametros);
    }

}


//Este metodo ejecuta la accion deseada, si se crea un disco, si se ejecuta un script, si se quiere un reporte
void analizador::ejecutar(string comando, vector <string> tkns)
{
    if(iguales(comando,"EXEC"))
    {
        cout<<"Osman Guacamaya   201504354"<<endl;
        cout<<"Parametro  "<<"Categoria   "<<"Descripcion"<<endl;
        cout<<"path       "<<"Obligatorio "<<tkns[0]<<endl;
        abrirArchivo(tkns);
    }
    else if(iguales(comando,"MKDISK"))
    {
       CrearDisco(tkns);
    }
    else if(iguales(comando,"RMDISK"))
    {
        BorrarDisco(tkns);
    }
    else if(iguales(comando,"FDISK"))
    {
        AdminPart(tkns);
    }
    else if(iguales(comando,"REP"))
    {
        HacerGrafica(tkns[0].substr(5).c_str());
    }
    else
    {
        cout<<tkns[0]<<endl;
    }
    Pausa();
}


//procedemos a crear un archivo o un disco con los parametros (mbr,ubicacion,tamaño)
void analizador::Crear_Archivo(tmbr *m,string path,int tam)
{
    try
    {
        int comienzo=(int) sizeof(tmbr);
        FILE *f1;
        f1= fopen(path.c_str(),"wb");
        rewind(f1);
        fwrite(m, sizeof(tmbr), 1, f1);
        Poner_Ceros(f1,comienzo,tam-comienzo);
        fclose(f1);
    }
    catch(int i)
    {
        cout<<"Existe un error al crear el archivo"<< i<<endl;
//        this->Pausa();
    }
}

void analizador::CrearDisco(vector <string> tkns)
{
        string data[4] = {"","","",""}; //size fit unit path

        for(string tk:tkns)
        {
            string par =tk.substr(0,tk.find("="));
            tk.erase(0,par.length()+1);

            if(iguales(par,"FIT"))
            {
                if(data[1].empty())
                {
                    data[1]=tk;
                }
                else
                {
                    cout<<"parametro fit en MKDist repetido"<<endl;
                }
            }
            else if(iguales(par,"SIZE"))
            {
                if(data[0].empty())
                {
                    data[0]=tk;
                }
                else
                {
                    cout<<"parametro size en MKDist repetido"<<endl;
                }
            }
            else if(iguales(par,"UNIT"))
            {
                if(data[2].empty())
                {
                    data[2]=tk;
                }
                else
                {
                    cout<<"parametro unit en MKDist repetido"<<endl;
                }
            }
            else if(iguales(par,"PATH"))
            {
                if(data[3].empty())
                {
                    data[3]=tk;
                }
                else
                {
                    cout<<"parametro path en MKDist repetido"<<endl;
                }
            }
            else
            {
                cout<<"Error en los parametros de MKDisk"<<endl;
            }
        }

        if(data[1].empty())
            data[1]="BF";

        if(data[2].empty())
            data[2]="K";

        if(data[0].empty() || data[3].empty())
        {
            cout<<"error en parametro path o size en MKDisk"<<endl;
        }
        else if(!iguales(data[1],"BF") && !iguales(data[1],"FF") && !iguales(data[1],"WF"))
        {
            cout<<"no se puede defirinir el tipo de ajuste parametro: "<<data[1]<<endl;
        }
        else
        {
            int bytes =0;
            string a=data[1].substr(0,1);
            tmbr m = DameMBR(atoi(data[0].c_str()),data[2],a,&bytes);
            Crear_Archivo(&m,data[3],bytes);
        }

}


void analizador::BorrarDisco(vector <string> tkns)
{
    string par =tkns[0].substr(0,tkns[0].find("="));
    tkns[0].erase(0,par.length()+1);
    if(iguales(par,"PATH"))
    {
        par = tkns[0];

        try
        {
            FILE *f1;
            f1= fopen(par.c_str(),"r");

            if(!f1==NULL)
            {
                fclose(f1);
                char eliminar='0';
                while(true){
                cout<<"Esta seguro de eliminar el Disco? "<<endl;
                cout<<"S/N...   "<<endl;
                cin>>eliminar;
                    if(eliminar=='S')
                    {
                        cout<<"Eliminando..."<<endl;
                        if(remove(par.c_str())==0)
                        {
                            cout<<"Disco Eliminado"<<endl;
                            break;
                        }
                        cout<<"No se elimino Disco"<<endl;
                    }
                    if(eliminar=='N')
                    {
                        cout<<"Cancelando Eliminacion..."<<endl;
                        break;
                    }
                    cout<<"por favor ingrese el caracter especificado"<<endl;
                    Pausa();
                }

            }

        }catch(int i)
        {
            cout<<"Error en borrado de disco "<<i<<endl;
        }
    }
    else
    {
        cout<<"Parametro inesperado en RMDISK"<<endl;
    }
}


void analizador::AdminPart(vector <string> tkns)
{
    vector <string> data = {"","","","","","","",""}; //size unit path type fit delete name add

    for(string tk:tkns)
    {
        string par =tk.substr(0,tk.find("="));
        tk.erase(0,par.length()+1);
        if(iguales(par,"SIZE"))
        {
            if(data[0].empty())
            {
                data[0]=tk;
            }
            else
            {
                cout<<"parametro size en FDist repetido"<<endl;
            }
        }
        else if(iguales(par,"UNIT"))
        {
            if(data[1].empty())
            {
                data[1]=tk;
            }
            else
            {
                cout<<"parametro UNIT en FDist repetido"<<endl;
            }
        }
        else if(iguales(par,"PATH"))
        {
            if(data[2].empty())
            {
                data[2]=tk;
            }
            else
            {
                cout<<"parametro PATH en FDist repetido"<<endl;
            }
        }
        else if(iguales(par,"TYPE"))
        {
            if(data[3].empty())
            {
                data[3]=tk;
            }
            else
            {
                cout<<"parametro TYPE en FDist repetido"<<endl;
            }
        }
        else if(iguales(par,"FIT"))
        {
            if(data[4].empty())
            {
                data[4]=tk;
            }
            else
            {
                cout<<"parametro FIT en FDist repetido"<<endl;
            }
        }
        else if(iguales(par,"DELETE"))
        {
            if(data[5].empty())
            {
                data[5]=tk;
            }
            else
            {
                cout<<"parametro DELETE en FDist repetido"<<endl;
            }
        }
        else if(iguales(par,"NAME"))
        {
            if(data[6].empty())
            {
                data[6]=tk;
            }
            else
            {
                cout<<"parametro NAME en FDist repetido"<<endl;
            }
        }
        else if(iguales(par,"ADD"))
        {
            if(data[7].empty())
            {
                data[7]=tk;
            }
            else
            {
                cout<<"parametro ADD en FDist repetido"<<endl;
            }
        }
        else
        {
            cout<<"Error en los parametros de FDISK"<<endl;
        }
    }

    //borrado 5
    if(!data[5].empty())
    {
        //obligatorios path, name
        if(data[2].empty() || data[6].empty())
        {
            cout<<"algunos parametros obligatorios para borrar no se encontraron"<<endl;
        }
    }
    //obligatorios size, path, name
    else if(data[0].empty() || data[2].empty() || data[6].empty())
    {
        cout<<"algunos parametros obligatorios no se encontraron"<<endl;
    }
    else
    {
        if(data[1].empty())
            data[1]="K";

        if(data[3].empty())
            data[3]="P";

        if(data[4].empty())
            data[4]="WF";
        int bytes =0;

        tpart p = DamePART(data,&bytes);
        Pausa();
        Escribir_Particion(data[2],&p);

    }
    cout<<"Imprimiendo FDisk"<<endl;
    for(string tk:data)
    {
        cout<<tk<<endl;
    }
    Pausa();

}

//Procedemos a verificar si es un path y si trae la ubicacion del archivo
void analizador::abrirArchivo(vector <string> tkns)
{
    string path = "";
    for (string token:tkns)
    {
        string tk = token.substr(0, token.find("="));
        token.erase(0,tk.length()+1);
        if (iguales(tk, "PATH"))
        {
            path = token;
        }
    }
    if (path.empty())
    {
        cout<<"posible problema con el path"<<endl;
        return;
    }
    abrirArch2(path);
}

//Utilizamos esta segunda parte para realizar la apertura del archivo como tal, ya mandando la ubicacion

void analizador::abrirArch2(string path)
{
    string filename(path);
    vector <string> lines;
    string line;
    ifstream input_file(filename);
    if(!input_file.is_open())
    {
        cerr << "No se puede abrir el archivo" << filename << endl;
        return;
    }
    while(getline(input_file,line))
    {
        lines.push_back(line);
    }
    for(const auto &i:lines)
    {
        string texto = i;
        string tk = comando(texto);
        if(texto!="")
        {
            if(iguales(texto,"PAUSE"))
            {
                string pause;
                cout<<"PAUSE, Presione enter para continuar..."<<endl;
                getline(cin,pause);
                continue;
            }
            texto.erase(0,tk.length()+1);
            vector <string> tks = segmentos(texto);
            ejecutar(tk,tks);
        }
    }
    input_file.close();
    return;
}


void analizador::Escribir_Particion(string path,tpart*p)
{
    string pt =path;
    string tk = path;
    inicio_global=0;

        FILE *f1;
        f1=fopen(pt.c_str(),"rb");
        if(f1==nullptr)
        {
            cout<<"El disco no existe"<<endl;
            return;
        }

        tmbr m;
        fread(&m,sizeof(tmbr),1,f1);
        //int tamD = m.mbr_tamano;
        int tamD = sizeof(m);
        vector <tpart> parti;
        parti.push_back(m.mbr_partition_1);
        parti.push_back(m.mbr_partition_2);
        parti.push_back(m.mbr_partition_3);
        parti.push_back(m.mbr_partition_4);

        vector <tenlace> inter;

        int act = 0;
        bool extendida=false;
        int cont=0;
        tpart ex;
        for(tpart part: parti)
        {
            cont++;
            if(part.part_status==1)
            {
                int xD = strcmp(part.part_name, p->part_name);
                if(xD==0)
                {
                    cout<<"El nombre de la particion ya existe"<<endl;
                    cout<<part.part_name<<endl;
                    Pausa();
                    return;
                }

                tenlace enl;
                enl.pa=cont;
                enl.ini=part.part_start;
                enl.fin=enl.ini+part.part_size;
                enl.adelante=enl.ini - tamD;
                tamD=enl.fin;

                if(act !=0)
                {
                    inter.at(act-1).atras = enl.ini -(inter.at(act-1).fin);
                }
                inter.push_back(enl);
                act++;

                if(part.part_type=='E')
                {
                    extendida=true;
                    ex=part;
                }
            }

            if(act==4)
            {
                if(p->part_type=='L' && !extendida)
                {
                    cout<<"Es necesario tener una particion extendida para crear una logica"<<endl;
                    return;
                }
                else if(!(p->part_type=='L'))
                {
                    cout<<"Ya cuentas con suficientes particiones"<<endl;
                    return;
                }
            }
            else if(!extendida && (p->part_type=='E'))
            {
                cout<<"Ya cuientas con una particion Extendida"<<endl;
                return;
            }
        }

        if(act!=0)
        {
            inter.at(inter.size() - 1).adelante = m.mbr_tamano - inter.at(inter.size() - 1).fin;
        }

        if(act==0)
        {
            p->part_start=sizeof(tmbr);
            inicio_global=p->part_start;
            m.mbr_partition_1=*p;
        }
        else if(act==1)
        {

            p->part_start=sizeof(tmbr)+m.mbr_partition_1.part_size;
            inicio_global=p->part_start;
            m.mbr_partition_2=*p;
        }
        else if(act==2)
        {
            p->part_start=sizeof(tmbr)+m.mbr_partition_1.part_size+m.mbr_partition_2.part_size;
            inicio_global=p->part_start;
            m.mbr_partition_3=*p;
        }
        else if(act==3)
        {
            p->part_start=sizeof(tmbr)+m.mbr_partition_1.part_size+m.mbr_partition_2.part_size+m.mbr_partition_3.part_size;
            inicio_global=p->part_start;
            m.mbr_partition_4=*p;
        }

        f1=fopen(pt.c_str(),"rb+");
        fseek(f1,0,SEEK_SET);
        fwrite(&m,sizeof(tmbr),1,f1);
        fclose(f1);


}



//Realizamos un pequeño automata para extraer fragmentos de la cadena donde viene la informacion path=jslkdjalsd.dk... etc
vector <string> analizador::segmentos(string text)
{
    vector<string> tokens;
    if (text.empty())
    {
        return tokens;
    }
    text.push_back(' ');
    string token = "";
    int estado = 0;
    for(char &c: text)
    {
        if (estado ==0 && c=='-')
        {
            estado = 1;

        }
        else if(estado==0 && c=='#')
        {
            continue;
        }
        else if(estado!=0)
        {
            if (estado == 1)
            {
                if(c=='=')
                {
                    estado = 2;
                }
                else if(c == ' ')
                {
                    continue;
                }
            }
            else if(estado == 2)
            {
                if (c=='\"')
                {
                    estado = 3;
                }
                else
                {
                    estado = 4;
                }

            }
            else if(estado == 3)
            {
                if (c=='\"')
                {
                    estado = 4;
                }
            }
            else if (estado==4 && c=='\"')
            {
                tokens.clear();
                continue;
            }
            else if (estado ==4 && c==' ')
            {
                estado = 0;
                tokens.push_back(token);
                token = "";
                continue;
            }
            token+=c;
        }
    }
    return tokens;
}

//con este fragmento obtenemos el codigo de entrada, para ver que estan solicitando MKDISK, EXEC....
string analizador::comando(string entrada)
{
    string tkns;
    int posI = 0;
    int posF = 0;
    string fragmento;
    bool flag =false;
    while(posF >=0)
    {
        if(entrada[posI]=='-')
            break;      //

        if(entrada[posI]=='#')
        {
            posF = entrada.find('\n',posI);
        }
        else
        {
            posF = entrada.find(' ',posI);
        }
        fragmento = entrada.substr(posI,posF-posI);
        posI=posF+1;
        tkns =fragmento;
    }
    return tkns;
}


//comparamos si una cadena es igual a otra
bool analizador::iguales(string cad1, string cad2)
{
    //convirtiendo en mayusculas a cad1
    string cad1M="";
    for(char&cad1:cad1)
    {
        cad1M+=toupper(cad1);
    }

    //comparando
    return cad1M==cad2 ? true:false;
}


//este codigo nos sirve para llenar de ceros un archivo, recibe (archivo, desde donde empiezan los ceros, cuantos ceros) lo llena del tamaño que necesitemos
void analizador::Poner_Ceros(FILE *f,int comienzo, int cant)
{
    for(int i = comienzo; i < cant; i++)
    {
        fputc('0', f);
    }

}

tmbr analizador::DameMBR(int tam,string uni,string f,int *bytes)
{
    tmbr mb;
    time_t fecha;
    fecha = time(NULL);
    char b = f[0];
    mb.dsk_fit =b;
    mb.mbr_fecha_creacion = fecha;
    mb.mbr_dsk_signature = rand();

    if(iguales(uni,"B"))  //si la unidad son kilobytes
    {
        *bytes = tam;//en b
    }
    else if(iguales(uni,"K"))  //si la unidad son kilobytes
    {
        *bytes = tam*1024;//en kb
    }
    else if (iguales(uni,"M"))  //sino es en megabytes
    {
        *bytes = tam*1024*1024;//en Mb
    }
    else
    {
        cout<<"existe un error en el tamaño del disco"<<endl;
    }
    mb.mbr_tamano=*bytes;
    mb.mbr_partition_1=tpart();
    mb.mbr_partition_2=tpart();
    mb.mbr_partition_3=tpart();
    mb.mbr_partition_4=tpart();
    return mb;
}


tpart analizador::DamePART(vector <string> tkns,int *bytes)
{
    tpart part;

    //minimos size 0, type 3, fit 4, name 6

    int tam = atoi(tkns[0].c_str());
    string fit = tkns[4];
    string name = tkns[6];

    for(int i=0; i<name.size(); i++)
    {
        part.part_name[i]=name[i];
    }

    if(iguales(tkns[1],"B"))  //si la unidad son bytes
    {
        *bytes = tam;//en bytes
    }
    else if(iguales(tkns[1],"K"))  //si la unidad son kilobytes
    {
        *bytes = tam*1024;//en kb
    }
    else if (iguales(tkns[1],"M"))  //sino es en megabytes
    {
        *bytes = tam*1024*1024;//en Mb
    }
    else
    {
        cout<<"existe un error en el tamaño del disco"<<endl;
    }
    part.part_size=*bytes;

    part.part_status=1;
    char b=tkns[3][0];
    part.part_type=b;
    part.part_fit=tkns[4][0 ];

    return part;
}


void analizador::HacerGrafica(string path)
{
    FILE *F;
    F = fopen(path.c_str(),"rb");
    if(F!=nullptr)
    {
        struct tm *ts;
        char fecha[80];
        string cadena="digraph g{\n";
        cadena+="struct[shape=record,";
        tmbr m;
        int cont=sizeof(tmbr);
        fread(&m,cont,1,F);
        ts = localtime(&m.mbr_fecha_creacion);
        strftime(fecha,sizeof(fecha),"%y-%m-%d %H:%M:%S",ts);
        cadena+="label=\"";
        cadena+="MBR\\n";
        cadena+="tamaño: ";
        cadena+=to_string(m.mbr_tamano);
        cadena+="\\n";
        cadena+="fecha:  ";
        cadena+=fecha;
        cadena+="\\n";
        cadena+="Fit:  ";
        cadena+=m.dsk_fit;
        cadena+="\\n";
        cadena+="ID:     ";
        cadena+=to_string(m.mbr_dsk_signature);

        fseek(F,cont,SEEK_SET);

        vector <tpart> parti;
        parti.push_back(m.mbr_partition_1);
        parti.push_back(m.mbr_partition_2);
        parti.push_back(m.mbr_partition_3);
        parti.push_back(m.mbr_partition_4);

        for(tpart part: parti)
        {
            if(part.part_status==1)
            {
                cadena+=" | ";
                cadena+="Particion\\n";
                cadena+="nombre: ";
                cadena+=part.part_name;
                cadena+="\\n";
                cadena+="tamaño:     ";
                cadena+=to_string(part.part_size);
                cadena+="\\n";
                cadena+="Inicio:     ";
                cadena+=to_string(part.part_start);
                cadena+="\\n";
                cadena+="Fit:     ";
                cadena+=part.part_fit;
                cadena+="\\n";
                cadena+="Tipo:     ";
                cadena+=part.part_type;
                cadena+="\\n";
                cadena+="Estado:     ";
                cadena+=part.part_status;
            }
            else
            {
                cadena+=" | ";
                cadena+="LIBRE\\n";
            }
        }

        fclose(F);
        cadena+="\"];\n";
        cadena +="}";
        cout<<cadena<<endl;
        remove("grafo.dot");
        FILE *f;
        f=fopen("grafo.dot","w");
        fwrite(cadena.c_str(),cadena.size(),1,f);
        fclose(f);

        system("dot -Tpng grafo.dot -o disco.png");
       // system("nohup display disco.png &" );
    }
}


//con este codigo podemos hacer pausa en el sistema y luego dar ENTER y conttinuar
void analizador::Pausa()
{
    cout << "Enter to continue..." <<endl;
    cin.ignore();
    cin.get();
    system("clear");
}

analizador::~analizador()
{
    //dtor
}
