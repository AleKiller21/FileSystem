#ifndef LISTA_H
#define LISTA_H
#include "Nodo.h"
#include "Nodo.cpp"
#include <iostream>

template <class T>
class Lista
{
private:
    int cantidad;

public:
    Nodo<T>* temporal;
    Nodo<T>* inicio;

    Lista();

    void insertar(int posicion, T valor);
    void agregar(T valor);
    void eliminar(int posicion);
    void mostrarLista();
    int getCantidad();
    int obtenerPosicion(T valor);
    T obtenerValor(int posicion);
    bool existe(T valor);

    virtual ~Lista();
};

#endif // LISTA_H
