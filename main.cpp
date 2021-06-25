#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "funciones.cpp"

using namespace std;

int main(int argc, char** argv){
    agregarAmatriz();
    //Quicksort(1,0,384628);
    OrdenarDias();
    cout<<"Regresion lineal para cantidades vendidas"<<endl;
    Regresion(1);
    cout<<endl;
    cout<<"Regresion lineal para valor total"<<endl;
    Regresion(2);
    Acsv("Ordenado",MatrizDias);
    return EXIT_SUCCESS;
}