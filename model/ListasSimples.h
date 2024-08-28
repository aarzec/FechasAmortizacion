/*************************
 * UFA-ESPE
 * ALUMNOS:  Leonardo Obando, William Leon
 * FECHA CREACION: Miercoles, 21 de noviembre de 2022
 * FECHA MODIFICACION: Miercoles, 23 de noviembre de 2022
 * Enunciado del problema: Crear los m�todos Insertar, Buscar
 y Eliminar elementos de una lista Simple, Doble y Circular
 * Nivel: TERCERO     NRC: 7999
 *************************/
#ifndef LISTASIMPLE_H
#define LISTASIMPLE_H

#include <functional>
#include "Nodo.h"

template<class T>
class ListaSimple {
private:
    Nodo<T>* cabeza;
public:
    ListaSimple();
    void Insertar(T);
    void InsertarPos(T val, int pos);
    void Buscar(T);
    void Eliminar(T);
    void Mostrar();
    int Obtener(int);
    int Tamano();
    int Intercambiar(int, int);

    void recorrer(std::function<void(Nodo<T>*)> fn);
};

template<typename T>
ListaSimple<T>::ListaSimple() {
    cabeza = NULL;
}

template<typename T>
void ListaSimple<T>::Insertar(T _dato) {
    Nodo<T>* nuevo = new Nodo<T>(_dato);
    if (cabeza == NULL) {
        cabeza = nuevo;
    }
    else {
        Nodo<T>* aux = cabeza;
        while (aux->getSiguiente() != NULL) {
            aux = aux->getSiguiente();
        }
        aux->setSiguiente(nuevo);
    }
}

template<typename T>
void ListaSimple<T>::InsertarPos(T val, int pos) {
    if (pos > Tamano() - 1) {
        for (int i = Tamano(); i < pos + 1; i++) {
            Insertar(-1);
        }
    }

    Nodo<T>* aux = cabeza;
    int cont = 0;
    while (aux != NULL) {
        if (cont == pos) {
            aux->setDato(val);
            return;
        }
        cont++;
        aux = aux->getSiguiente();
    }
}

template<typename T>
void ListaSimple<T>::Buscar(T _dato) {
    Nodo<T>* aux = cabeza;
    while (aux != NULL) {
        if (aux->getDato() == _dato) {
            cout << "El dato si se encuentra en la lista" << endl;
            return;
        }
        aux = aux->getSiguiente();
    }
    cout << "El dato no se encuentra en la lista" << endl;
}

template<typename T>
void ListaSimple<T>::Eliminar(T _dato) {
    Nodo<T>* aux = cabeza;
    Nodo<T>* anterior = NULL;
    while (aux != NULL) {
        if (aux->getDato() == _dato) {
            if (anterior == NULL) {
                cabeza = aux->getSiguiente();
            }
            else {
                anterior->setSiguiente(aux->getSiguiente());
            }
            delete aux;
            cout << "El dato se ha eliminado de la lista" << endl;
            return;
        }
        anterior = aux;
        aux = aux->getSiguiente();
    }
    cout << "El dato no se encuentra en la lista" << endl;
}

template<typename T>
void ListaSimple<T>::Mostrar() {
    Nodo<T>* aux = cabeza;
    while (aux != NULL) {
        if (aux->getDato() >= 0) {
            cout << aux->getDato() << " -> ";
        }
        aux = aux->getSiguiente();
    }
    cout << "NULL" << endl;
}

template<typename T>
int ListaSimple<T>::Obtener(int _pos) {
    Nodo<T>* aux = cabeza;
    int cont = 0;
    while (aux != NULL) {
        if (cont == _pos) {
            return aux->getDato();
        }
        cont++;
        aux = aux->getSiguiente();
    }
    return -1;
}

template<typename T>
int ListaSimple<T>::Tamano() {
    Nodo<T>* aux = cabeza;
    int cont = 0;
    while (aux != NULL) {
        cont++;
        aux = aux->getSiguiente();
    }
    return cont;
}

template<typename T>
int ListaSimple<T>::Intercambiar(int _pos1, int _pos2) {
    Nodo<T>* aux1 = cabeza;
    Nodo<T>* aux2 = cabeza;
    int cont = 0;
    while (aux1 != NULL) {
        if (cont == _pos1) {
            break;
        }
        cont++;
        aux1 = aux1->getSiguiente();
    }
    cont = 0;
    while (aux2 != NULL) {
        if (cont == _pos2) {
            break;
        }
        cont++;
        aux2 = aux2->getSiguiente();
    }
    int temp = aux1->getDato();
    aux1->setDato(aux2->getDato());
    aux2->setDato(temp);
    return 0;
}

template<typename T>
void ListaSimple<T>::recorrer(std::function<void(Nodo<T>*)> fn) {
    Nodo<T>* aux = cabeza;
    while (aux != NULL) {
        fn(aux);
        aux = aux->getSiguiente();
    }
}

#endif