#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>

using namespace std;

string Matriz[384629][5];
string MatrizOrdenada[52440][5];

void Dias(int fila){
    string fecha = "";
    int tamano = Matriz[fila][0].length();
    float anio, mes, dia, hora, minuto, SumaTotal;
    for(int i=0; i < tamano-1 ;i++){
        if(Matriz[fila][0][i] != '-' && Matriz[fila][0][i] != ' ' && Matriz[fila][0][i] != ':'){
            fecha = fecha + Matriz[fila][0][i];
        }
        else{
            if(Matriz[fila][0][i] == '-' && i == 4){
                istringstream(fecha) >> (anio);
                fecha = "";
            }
            else{
                if(Matriz[fila][0][i] == '-' && i == 7){
                    istringstream(fecha) >> (mes);
                    fecha = "";
                }
                else{
                    if(Matriz[fila][0][i] == ' '){
                        istringstream(fecha) >> (dia);
                        fecha = "";
                    }
                    else{
                        if(Matriz[fila][0][i] == ':' && i == 13){
                            istringstream(fecha) >> (hora);
                            fecha = "";
                        }
                        else{
                            if(Matriz[fila][0][i] == ':' && i == 16){
                                istringstream(fecha) >> (minuto);
                                fecha = "";
                                i = tamano;
                            }
                        }
                    }
                }
            }
        }
    }
    anio = anio*365;
    SumaTotal =(mes-1)*30 + dia + (hora/24) + (minuto/1440);
    Matriz[fila][0] = to_string(SumaTotal+anio);
}

void agregarAmatriz(){
    ifstream infile("datos.csv");
    string line ="";
    string dato = "";
    int i = 0;
    long long conversion = 0;
    getline(infile,line);
    while(i<384629){
        for(int j=0;j<5;j++){
            if(j == 4){
                getline(infile,line,'\n');
            }
            else{
                getline(infile,line,';');
            }
            if(j != 0 && j != 4){
                line.replace(line.find('"'),1,"");
                istringstream(line) >> (conversion);
                dato = to_string(conversion);
                Matriz[i][j] = dato;
            }
            else{
                if (j == 0){
                    line.replace(line.find('"'),1,"");
                    Matriz[i][j] = line;
                    Dias(i);
                }
                else{
                    Matriz[i][j] = line;
                }
            }
        }
        i++;
    }
}
void imprimir(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 5; j++){
            cout<<" "<<MatrizOrdenada[i][j];
        }
        cout<<endl;
    }
}
void Quicksort(int columna, int izq, int der){
    int i = izq;
    int j = der;
    long long cambio1;
    long long cambio2;
    long long cambio3;
    string tmp[1][5];
    string p = Matriz[(izq+der)/2][columna];
    istringstream(p) >> cambio2;
    while(i<=j){
        istringstream(Matriz[i][columna]) >> cambio1;
        while(cambio1 < cambio2){
            i++;
            istringstream(Matriz[i][columna]) >> cambio1;
        }
        istringstream(Matriz[j][columna]) >> cambio3;
        while(cambio3 > cambio2){
            j--;
            istringstream(Matriz[j][columna]) >> cambio3;
        }
        if(i<=j){
            tmp[0][0] = Matriz[i][0];
            tmp[0][1] = Matriz[i][1];
            tmp[0][2] = Matriz[i][2];
            tmp[0][3] = Matriz[i][3];
            tmp[0][4] = Matriz[i][4];
            Matriz[i][0] = Matriz[j][0];
            Matriz[i][1] = Matriz[j][1];
            Matriz[i][2] = Matriz[j][2];
            Matriz[i][3] = Matriz[j][3];
            Matriz[i][4] = Matriz[j][4];
            Matriz[j][0] = tmp[0][0];
            Matriz[j][1] = tmp[0][1];
            Matriz[j][2] = tmp[0][2];
            Matriz[j][3] = tmp[0][3];
            Matriz[j][4] = tmp[0][4];
            i++;j--;
        }

    }
    if (izq < j)
    {
        Quicksort(columna, izq, j);
    }
    if (i < der)
    {
        Quicksort(columna, i, der);
    }
}

void CantidadDias(int min, int max, int contador, int cantidad){
    int fecha1, fecha2, dias;
    istringstream(Matriz[min][0]) >> fecha1;
    istringstream(Matriz[max][0]) >> fecha2;
    if ((fecha2 - fecha1) == 0){
        dias = 1;
    }
    else{
        dias = fecha2 - fecha1;
    }
    if(contador == 0){
        MatrizOrdenada[contador][0] = "dias ;";
        MatrizOrdenada[contador][1] = " skrull ;";
        MatrizOrdenada[contador][2] = " cantidad ;";
        MatrizOrdenada[contador][3] = " Valor ;";
        MatrizOrdenada[contador][4] = " nombre ;";
    }
    else{
        MatrizOrdenada[contador][0] = to_string(dias);
        MatrizOrdenada[contador][1] = Matriz[min][1];
        MatrizOrdenada[contador][2] = to_string(cantidad);
        MatrizOrdenada[contador][3] = Matriz[min][3];
        MatrizOrdenada[contador][4] = Matriz[min][4];
    }
}

void Ordenar(){
    int j=0, cantidad=0, cantidad1=0, contador = 1;
    CantidadDias(0,0,0,cantidad);
    for(int i=0; i < 384629; i++){
        istringstream(Matriz[i][2]) >> cantidad1;
        cantidad = cantidad + cantidad1;
        if(Matriz[i][1] != Matriz[i+1][1]){
            Quicksort(0,j,i);
            CantidadDias(j,i,contador,cantidad);
            j = i + 1;
            contador++;
            cantidad = 0;
        }
    }
    cout<< contador << endl;
}
//modificar para colocar en los archivos, de momento quedara como comentario

void Acsv(string ArchivoCSV, string matriz[][5]){
    ofstream archivo;
    archivo.open("al/"+ArchivoCSV+".csv", ios::out | ios::app);
    for(int i=0; i < 52440; i++){
        archivo<<matriz[i][0]<<"; "<<matriz[i][1]<<"; "<<matriz[i][2]<<"; "<<matriz[i][3]<<"; "<<matriz[i][4]<<endl;
    }
}