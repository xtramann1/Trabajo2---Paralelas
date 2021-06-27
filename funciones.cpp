#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>
#include <math.h>

using namespace std;

//Variales globales a utilizar

string Matriz[384629][6];
string MatrizDias[198][4];
string Formulas[3][3];

/*En esta primera parte se realiza un 
traspaso de los datos a una matriz para poder trabajar con ellos*/

//Transforma la fecha a cantidad de dias
void Dias(int fila){
    string fecha = "";
    int tamano = Matriz[fila][0].length();
    int anio, mes, dia, SumaTotal, SumaTotal2, aux;
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
                }
            }
        }
    }
    SumaTotal = anio*365 + (mes-1)*30 + dia;
    if(fila == 0){
        Matriz[fila][5] = to_string(SumaTotal);
    }
    else{
        if(fila == 384628){
            Matriz[0][5] = "1";
        }
        else{
            istringstream(Matriz[0][5]) >> SumaTotal2;
            aux = SumaTotal - SumaTotal2;
            if(aux == 0){
                Matriz[fila][5] = "1";
            }
            else{
                Matriz[fila][5] = to_string(aux+1);
            }
        }
    }
}

// Agrega a matriz los datos del archivo datos.csv
void agregarAmatriz(){
    ifstream infile("datos.csv");
    string line ="";
    string dato = "";
    int i = 0;
    double conversion = 0;
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
//Deja solo la fecha en formato YY-MM-DD (Año-Mes-Día)
string FechaSinHHMM(string dato){
    int tamano = dato.length();
    string fecha = "";
    for(int i=0; i < tamano-1 ;i++){
        if(dato[i] != ' '){
            fecha = fecha + dato[i];
        }
        else{
            return fecha;
        }
    }
    return fecha;
}
//Pasa a MatrizDias la acumulacion de ventas en un solo dia mas el valor total ganado
void JuntarDias(int contador, double cantidad, double valor, string dato, string fechaDias){
    if(contador == 0){
        MatrizDias[contador][0] = "Fecha ;";
        MatrizDias[contador][1] = " cantidad ;";
        MatrizDias[contador][2] = " Valor total ;";
        MatrizDias[contador][3] = " Fecha en dias ;";

    }
    else{
        MatrizDias[contador][0] = FechaSinHHMM(dato);
        MatrizDias[contador][1] = to_string(cantidad);
        MatrizDias[contador][2] = to_string(valor);
        MatrizDias[contador][3] = fechaDias;
    }
}

//Agregar a MatrizDias los dias acumulados
void OrdenarDias(){
    double cantidad=0, cantidad1=0, valor=0, valor1=0, contador = 1;
    JuntarDias(0,0,0,"hola"," ");
    for(int i=0; i < 384629; i++){
        istringstream(Matriz[i][2]) >> cantidad1;
        cantidad = cantidad + cantidad1;
        istringstream(Matriz[i][3]) >> valor1;
        valor = valor + (valor1*cantidad1);
        if(Matriz[i][5] != Matriz[i+1][5]){
            JuntarDias(contador,cantidad,valor, Matriz[i][0], Matriz[i][5]);
            contador++;
            cantidad = 0;
            valor = 0;
        }
    }
}
/* En esta segunda parte se realizarán las funciones necesarias para utilizar los modelos
1. Regresión lineal      Y = a + (bR)x
2* Correlación lineal    Y = a + bx */

//////////////////////////////  Inicio Regresion lineal ///////////////////////////////////////////

//Función para sacar el cuadrado de un número
double Cuadrado(double valor){
    return valor*valor;
}

//Función para determinar el coeficiente de relación (R)
double Relacion(double x, double y, int columna){
    double arriba = 0, equis, ye, Rx = 0, Ry = 0, abajo;
    for (int i=0; i < 198; i++){
        istringstream(MatrizDias[i][3]) >> equis;
        istringstream(MatrizDias[i][columna]) >> ye;
        arriba += ((equis - x)*(ye - y));
        Rx += Cuadrado(equis-x);
        Ry += Cuadrado(ye-y);
    }
    Rx = sqrt(Rx);
    Ry = sqrt(Ry);
    abajo = Rx * Ry;
    return arriba/abajo;
}

//Función para determinar beta (b)
double beta(double x, double y, int columna){
    double sx = 0, sy = 0;
    double equis, ye;
    for(int i=0; i < 198; i++){
        istringstream(MatrizDias[i][3]) >> equis;
        istringstream(MatrizDias[i][columna]) >> ye;
        sy += ((equis - x)*(ye - y));
        sx += ((equis - x) * (equis - x));
    }
    return sy/sx;
}

//Función para determinar el alfa (a)
double alfa(double beta, double x, double y){
    return y - (beta * x);
}

//Función para determinar la ecuación de regresión lineal
void Regresion(int columna){
    double promediox = 0, promedioy = 0;
    double equis, ye;
    for(int i = 0; i < 198; i++){
        istringstream(MatrizDias[i][3]) >> equis;
        istringstream(MatrizDias[i][columna]) >> ye;
        promediox += equis/198;
        promedioy += ye/198;
    }
    double betacoef = (beta(promediox, promedioy, columna))*(Relacion(promediox, promedioy, columna));
    double alfacoef = alfa(betacoef, promediox, promedioy);
    cout<<"La ecuacion  de regresion lineal >>  es Y = "<< betacoef << "x + "<< alfacoef<<endl;
}
///////////////////////////////////     Fin regresion lineal      ////////////////////////////////////

//////////////////////////////////      Inicio Correlacion lineal       /////////////////////////////////////////

//Función para determinar el alfa (a)
double ALFA2(int columna){
    double Sxy=0, equis, ye, Sx=0, Sy=0, cuadrado=0, sumcuadrado=0;
    double arriba, abajo;
    for(int i=0; i < 198; i++){
        istringstream(MatrizDias[i][3]) >> equis;
        istringstream(MatrizDias[i][columna]) >> ye;
        Sxy += (equis*ye);
        Sx += equis;
        Sy += ye;
        cuadrado += Cuadrado(equis);
    }
    sumcuadrado = Cuadrado(Sx);
    arriba = (198*Sxy - (Sx*Sy));
    abajo = (198*cuadrado - sumcuadrado);
    return arriba/abajo;
}

//Función para determinar el beta (b)
double BETA2(int columna){
    double Sy = 0, Sx = 0, equis, ye;
    double alfita = 0, betita;
    alfita = ALFA2(columna);
    for (int i = 0; i < 198; i++){
        istringstream(MatrizDias[i][3]) >> equis;
        istringstream(MatrizDias[i][columna]) >> ye;
        Sy += ye;
        Sx += equis;
    }
    betita = Sy - (alfita*Sx);
    return betita/198;
}

//Función para determinar la ecuación de la correlación lineal
void correlacion(int columna){
    double alfita, betita;
    alfita = ALFA2(columna);
    betita = BETA2(columna);
    cout<<"La ecuacion  de Correlacion lineal >>  es Y = "<< alfita << "x + "<< betita<<endl;
}
//////////////////////////////////      Fin Correlacion lineal       /////////////////////////////////////////

//Crear el archivo CSV
void Acsv(string ArchivoCSV, string matriz[][4]){
    ofstream archivo;
    archivo.open("al/"+ArchivoCSV+".csv", ios::out | ios::app);
    for(int i=0; i < 198; i++){
        archivo<<matriz[i][0]<<"; "<<matriz[i][1]<<"; "<<matriz[i][2]<<"; "<<matriz[i][3]<<endl;
    }
}