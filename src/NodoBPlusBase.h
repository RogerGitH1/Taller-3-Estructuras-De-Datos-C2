#pragma once
#include "NodoGrafo.h"

class NodoBPlusBase {
private:
    int* claves; // vector de ids de NodoGrafo
    int orden; // factor m de árbol
    bool es_hoja; // indica si es nodo interno o hoja del árbol
};

class NodoBInterno: public NodoBPlusBase {
private:
    NodoBPlusBase** punteros; // Vector del tipo NodoBPlusBase*
    // función que dado un id de NodoGrafo indica a cual Nodo del árbol B hay
    // que moverse. Devuelve el índice de vector punteros.
    int buscar_siguiente();
};

class NodoBHoja: public NodoBPlusBase {
private:
    NodoGrafo** datos; // vector del tipo NodoGrafo*
    NodoBHoja* siguiente_hoja; // puntero del tipo NodoBHoja*
};