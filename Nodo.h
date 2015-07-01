#ifndef NODO_H
#define NODO_H

template <class T>
class Nodo
{
    public:
    T valor;
    Nodo* next;

    Nodo(T valor);
        virtual ~Nodo();
    protected:
    private:
};

#endif // NODO_H
