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

NodoGrafo* Sistema::buscar_nodo_grafo(int clave) {
    int tocados = 0;
    NodoBHoja* hoja = buscar_hoja(clave);
    return hoja -> buscar_en_hoja(clave, tocados);
}


void insertar_en_hoja(NodoBHoja* hoja, int clave, NodoGrafo* info) {
    int usados = hoja -> get_num_claves();
    int i = usados - 1;

    while (i >= 0 && hoja -> get_clave(i) > clave) {
        hoja -> set_clave(i + 1, hoja -> get_clave(i));
        hoja -> set_dato(i + 1, hoja -> get_datos()[i]);
        i--;
    }

    hoja -> set_clave(i + 1, clave);
    hoja -> set_dato(i + 1, info);
    hoja -> set_num_claves(usados + 1);
}

