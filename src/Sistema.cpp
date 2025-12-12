#include "Sistema.h"
#include <iostream>

Sistema::Sistema(int orden) : orden(orden), siguiente_id(0) {
    raiz = new NodoBHoja(orden);
}

NodoBHoja* Sistema::buscar_hoja(int clave) {
    NodoBPlusBase* actual = raiz;

    while (!actual -> get_es_hoja()) {
        NodoBInterno* interno = (NodoBInterno*) actual;
        int idx = interno -> buscar_siguiente(clave);
        actual = interno -> get_hijo(idx);
    }

    return (NodoBHoja*) actual;
}