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

NodoBHoja* dividir_hoja(NodoBHoja* hoja, int clave, NodoGrafo* info, int& clave_arriba) {
    int m = hoja -> get_orden();
    int total = m + 1;

    int* claves_temp = new int[total];
    NodoGrafo** datos_temp = new NodoGrafo*[total];

    int usadas = hoja -> get_num_claves();
    int i = 0;

    while (i < usadas && hoja -> get_clave(i) < clave) {
        claves_temp[i] = hoja -> get_clave(i);
        datos_temp[i]  = hoja -> get_datos()[i];
        i++;
    }

    claves_temp[i] = clave;
    datos_temp[i]  = info;

    for (int j = i; j < usadas; j++) {
        claves_temp[j + 1] = hoja -> get_clave(j);
        datos_temp[j + 1]  = hoja -> get_datos()[j];
    }

    int mitad = total / 2;
    NodoBHoja* nueva = new NodoBHoja(m);

    hoja -> set_num_claves(0);
    for (int j = 0; j < mitad; j++) {
        hoja -> set_clave(j, claves_temp[j]);
        hoja -> set_dato(j, datos_temp[j]);
    }
    hoja -> set_num_claves(mitad);

    int k = 0;
    for (int j = mitad; j < total; j++) {
        nueva -> set_clave(k, claves_temp[j]);
        nueva -> set_dato(k, datos_temp[j]);
        k++;
    }
    nueva -> set_num_claves(k);

    nueva -> set_siguiente_hoja(hoja -> get_siguiente_hoja());
    hoja -> set_siguiente_hoja(nueva);

    clave_arriba = nueva -> get_clave(0);

    delete[] claves_temp;
    delete[] datos_temp;

    return nueva;
}

void insertar_en_interno(NodoBInterno* nodo, int clave, NodoBPlusBase* hijo_izq, NodoBPlusBase* hijo_der) {
    int usadas = nodo -> get_num_claves();
    int i = usadas - 1;

    while (i >= 0 && nodo -> get_clave(i) > clave) {
        nodo -> set_clave(i + 1, nodo -> get_clave(i));
        nodo -> set_hijo(i + 2, nodo -> get_punteros()[i + 1]);
        i--;
    }

    nodo -> set_clave(i + 1, clave);
    nodo -> set_hijo(i + 1, hijo_izq);
    nodo -> set_hijo(i + 2, hijo_der);
    nodo -> set_num_claves(usadas + 1);
}

NodoBInterno* dividir_interno(NodoBInterno* nodo, int clave, NodoBPlusBase* hijo_izq, NodoBPlusBase* hijo_der, int& clave_arriba) {
    int m = nodo -> get_orden();
    int total = m + 1;

    int* claves_temp = new int[total];
    NodoBPlusBase** hijos_temp = new NodoBPlusBase*[total + 1];

    int usadas = nodo -> get_num_claves();
    int i = 0;

    while (i < usadas && nodo -> get_clave(i) < clave) {
        claves_temp[i] = nodo -> get_clave(i);
        hijos_temp[i]  = nodo -> get_punteros()[i];
        i++;
    }

    claves_temp[i] = clave;
    hijos_temp[i]  = hijo_izq;
    hijos_temp[i + 1] = hijo_der;

    for (int j = i; j < usadas; j++) {
        claves_temp[j + 1] = nodo -> get_clave(j);
        hijos_temp[j + 2]  = nodo -> get_punteros()[j + 1];
    }

    int mitad = total / 2;
    clave_arriba = claves_temp[mitad];

    NodoBInterno* nuevo = new NodoBInterno(m);

    nodo -> set_num_claves(0);
    for (int j = 0; j < mitad; j++) {
        nodo -> set_clave(j, claves_temp[j]);
        nodo -> set_hijo(j, hijos_temp[j]);
    }
    nodo -> set_hijo(mitad, hijos_temp[mitad]);
    nodo -> set_num_claves(mitad);

    int k = 0;
    for (int j = mitad + 1; j < total; j++) {
        nuevo -> set_clave(k, claves_temp[j]);
        nuevo -> set_hijo(k, hijos_temp[j]);
        k++;
    }
    nuevo -> set_hijo(k, hijos_temp[total]);
    nuevo -> set_num_claves(k);

    delete[] claves_temp;
    delete[] hijos_temp;

    return nuevo;
}

void Sistema::insertar_nodo_grafo(int clave, NodoGrafo* info) {
    NodoBHoja* hoja = buscar_hoja(clave);

    if (hoja -> get_num_claves() < hoja -> get_orden()) {
        insertar_en_hoja(hoja, clave, info);
        return;
    }

    int clave_arriba;
    NodoBHoja* hoja_nueva = dividir_hoja(hoja, clave, info, clave_arriba);

    if (raiz -> get_es_hoja()) {
        NodoBInterno* nueva = new NodoBInterno(orden);
        nueva -> set_clave(0, clave_arriba);
        nueva -> set_hijo(0, hoja);
        nueva -> set_hijo(1, hoja_nueva);
        nueva -> set_num_claves(1);
        raiz = nueva;
        return;
    }

    NodoBPlusBase* actual = raiz;
    NodoBPlusBase* padre = nullptr;

    while (!actual -> get_es_hoja()) {
        NodoBInterno* in = (NodoBInterno*) actual;
        int pos = in -> buscar_siguiente(clave_arriba);
        padre = actual;
        actual = in -> get_hijo(pos);
    }

    NodoBInterno* p = (NodoBInterno*) padre;

    if (p -> get_num_claves() < p -> get_orden()) {
        insertar_en_interno(p, clave_arriba, hoja, hoja_nueva);
        return;
    }

    int clave_arriba2;
    NodoBInterno* nuevo_int = dividir_interno(p, clave_arriba, hoja, hoja_nueva, clave_arriba2);

    if (padre == raiz) {
        NodoBInterno* r2 = new NodoBInterno(orden);
        r2 -> set_clave(0, clave_arriba2);
        r2 -> set_hijo(0, p);
        r2 -> set_hijo(1, nuevo_int);
        r2 -> set_num_claves(1);
        raiz = r2;
    }
}
