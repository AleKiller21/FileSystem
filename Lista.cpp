#include "Lista.h"

template<class T>
Lista<T>::Lista()
{
    this->temporal = NULL;
    this->inicio = NULL;
    this->cantidad = 0;
}

template<class T>
void Lista<T>::insertar(int posicion ,T valor)
{
    if(posicion < 0 || posicion > cantidad)
    {
        return;
    }

    else
    {
        Nodo<T>* nodo = new Nodo<T>(valor);

        this->temporal = inicio;

        if(posicion == 0)
        {
            inicio = nodo;
            inicio->next = temporal;
            cantidad++;
            return;
        }

        for(int i = 0; i < posicion-1; i++)
        {
            temporal = temporal->next;
        }

        Nodo<T>* temp2 = temporal->next;
        temporal->next = nodo;
        temporal->next->next = temp2;
        cantidad++;
    }
}

template<class T>
void Lista<T>::agregar(T valor)
{
    insertar(cantidad, valor);
}

template<class T>
int Lista<T>::getCantidad()
{
    return this->cantidad;
}

template<class T>
T Lista<T>::obtenerValor(int posicion)
{
    temporal = inicio;

    for(int i = 0; i < cantidad; i++)
    {
        if(i == posicion)
        {
            return temporal->valor;
        }

        temporal = temporal->next;
    }
}

template<class T>
int Lista<T>::obtenerPosicion(T valor)
{
    temporal = inicio;

    for(int i = 0; i < cantidad; i++)
    {
        if(temporal->valor == valor)
        {
            return i;
        }

        temporal = temporal->next;
    }
}

template<class T>
void Lista<T>::eliminar(int posicion)
{
    if(posicion < 0 || posicion > cantidad)
    {
        return;
    }

    if(posicion == 0)
    {
        Nodo<T>* temp2 = inicio;
        inicio = inicio->next;
        delete temp2;
        cantidad--;
        return;
    }

    temporal = inicio;

    for(int i = 0; i < posicion-1; i++)
    {
        temporal = temporal->next;
    }

    Nodo<T>* temp2 = temporal->next;
    temporal->next = temporal->next->next;
    delete temp2;
    cantidad--;
}

template<class T>
void Lista<T>::mostrarLista()
{
    this->temporal = inicio;

    for(int i = 0; i < cantidad; i++)
    {
        std::cout << temporal->valor << std::endl;
        temporal = temporal->next;
    }
}

template<class T>
bool Lista<T>::existe(T valor)
{
    temporal = inicio;

    for(int i = 0; i < cantidad; i++)
    {
        if(temporal->valor == valor)
        {
            return true;
        }

        temporal = temporal->next;
    }

    return false;
}

template<class T>
Lista<T>::~Lista()
{
    //dtor
}
