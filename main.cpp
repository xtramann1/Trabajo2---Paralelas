#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "funciones.cpp"

using namespace std;

int main(int argc, char** argv){
    agregarAmatriz();
    //Quicksort(1,0,384628);
    OrdenarDias();
    Regresion();
    Acsv("Ordenado",MatrizDias);
    return EXIT_SUCCESS;
}