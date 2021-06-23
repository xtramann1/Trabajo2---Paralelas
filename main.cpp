#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "funciones.cpp"

using namespace std;

int main(int argc, char** argv){
    agregarAmatriz();
    Quicksort(1,0,384628);
    Ordenar();
    imprimir();
    Acsv("Ordenado",MatrizOrdenada);
    return EXIT_SUCCESS;
}