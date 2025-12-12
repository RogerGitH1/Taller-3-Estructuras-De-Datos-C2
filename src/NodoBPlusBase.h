#pragma once
#include "NodoGrafo.h"

class NodoBPlusBase {
protected:
    int* claves; 
    int orden; 
    int num_claves; 
    bool es_hoja; 

public:
    NodoBPlusBase(int orden, bool es_hoja);
    virtual ~NodoBPlusBase();

    int get_num_claves() const;
    int get_clave(int idx) const;
    bool get_es_hoja() const;
};

class NodoBInterno: public NodoBPlusBase {
protected:
    NodoBPlusBase** punteros; 
public:
    NodoBInterno(int orden);
    virtual ~NodoBInterno();
    int buscar_siguiente(int clave);

    NodoBPlusBase* get_hijo(int idx);
};

class NodoBHoja: public NodoBPlusBase {
protected:
    NodoGrafo** datos; 
    NodoBHoja* siguiente_hoja; 

public:
    NodoBHoja(int orden);
    virtual ~NodoBHoja();
    NodoGrafo* buscar_en_hoja(int clave, int& accesos);

    NodoBHoja* get_siguiente_hoja() const;
    void set_siguiente_hoja(NodoBHoja* sig);
};