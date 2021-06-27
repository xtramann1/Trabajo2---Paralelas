#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "funciones.cpp"

using namespace std;

int main(int argc, char** argv){
    agregarAmatriz();
    OrdenarDias();
    cout<<endl;
    cout<<"<<<INTEGRANTES>>>"<<endl;
    cout<<"Felipe González Duarte"<<endl;
    cout<<"Fabián Urriola Poisson"<<endl;
    cout<<endl;
    cout<<"Profesor: Sebastian Alexis Salazar Molina"<<endl;
    cout<<"Asignatura: Computación Paralela y Distribuida"<<endl;
    cout<<endl;
    cout<<"Regresion lineal para cantidades vendidas"<<endl;
    Regresion(1);
    cout<<"Regresion lineal para valor total"<<endl;
    Regresion(2);
    cout<<endl;
    cout<<"Correlacion para cantidades vendidas"<<endl;
    correlacion(1);
    cout<<"Correlacion para valor total"<<endl;
    correlacion(2);
    Acsv("Ordenado",MatrizDias);
    return EXIT_SUCCESS;
}