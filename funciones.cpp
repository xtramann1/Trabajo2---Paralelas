#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>

using namespace std;

string Matriz[384629][6];
string MatrizDias[198][4];
string Formulas[3][3];

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
void imprimir(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 6; j++){
            cout<<"  ;  "<<Matriz[i][j];
        }
        cout<<endl;
    }
}
void imprimir2(){
    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 4; j++){
            cout<<"  ;  "<<MatrizDias[i][j];
        }
        cout<<endl;
    }
}

//deja solo la fecha en formato YY-MM-DD
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
//pasa a MatrizDias la acumulacion de ventas en un solo dia mas el valor total ganado
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
// procedimiento para llevar a cabo la regresión lineal
//////////////////////////////  Inicio Regresion lineal ///////////////////////////////////////////
double beta(double x, double y, int columna){
    double sx = 0, sy = 0;
    double equis, ye;
    for(int i=0; i < 198; i++){
        istringstream(MatrizDias[i][3]) >> equis;
        istringstream(MatrizDias[i][columna]) >> ye;
        sy = sy + ((equis - x)*(ye - y));
        sx = sx + (equis - x) * (equis - x);
    }
    return sy/sx;
}

double alfa(double beta, double x, double y){
    return y - (beta * x);
}

void Regresion(int columna){
    double promediox = 0, promedioy = 0;
    double equis, ye;
    for(int i = 0; i < 198; i++){
        istringstream(MatrizDias[i][3]) >> equis;
        istringstream(MatrizDias[i][columna]) >> ye;
        promediox = promediox + equis/198;
        promedioy = promedioy + ye/198;
    }
    double betacoef = beta(promediox, promedioy, columna);
    double alfacoef = alfa(betacoef, promediox, promedioy);
    cout<<"La ecuacion  de regresion lineal >>  es Y = "<< betacoef << "x + "<< alfacoef<<endl;
}
///////////////////////////////////     Fin regresion lineal      ////////////////////////////////////
//////////////////////////////////      Inicio Arima        /////////////////////////////////////////
//Crear el archivo CSV
void Acsv(string ArchivoCSV, string matriz[][4]){
    ofstream archivo;
    archivo.open("al/"+ArchivoCSV+".csv", ios::out | ios::app);
    for(int i=0; i < 198; i++){
        archivo<<matriz[i][0]<<"; "<<matriz[i][1]<<"; "<<matriz[i][2]<<"; "<<matriz[i][3]<<endl;
    }
}