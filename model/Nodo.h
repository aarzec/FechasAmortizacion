/*************************
 * UFA-ESPE
 * ALUMNOS:  Leonardo Obando, William Leon
 * FECHA CREACION: Miercoles, 21 de noviembre de 2022
 * FECHA MODIFICACION: Miercoles, 23 de noviembre de 2022
 * Enunciado del problema: Crear los m�todos Insertar, Buscar
 y Eliminar elementos de una lista Simple, Doble y Circular
 * Nivel: TERCERO     NRC: 7999
 *************************/
#ifndef NODO_H
#define NODO_H

#include <iostream>
#include <string>

using namespace std;

template<class T>
class Nodo {
    private:
        T dato;
        Nodo<T>* siguiente;
    public:
        Nodo(T);
        void setDato(T);
        void setSiguiente(Nodo<T>*);
        T getDato();
        Nodo<T>* getSiguiente();
};

template<typename T>
Nodo<T>::Nodo(T _dato) {
    dato = _dato;
    siguiente = NULL;
}

template<typename T>
void Nodo<T>::setDato(T _dato) {
    dato = _dato;
}

template<typename T>
T Nodo<T>::getDato() {
    return dato;
}

template<typename T>
void Nodo<T>::setSiguiente(Nodo<T>* _siguiente) {
    siguiente = _siguiente;
}

template<typename T>
Nodo<T>* Nodo<T>::getSiguiente() {
    return siguiente;
}


#endif