#include "NodoBPlusBase.h"
#include <iostream>

using namespace std;

NodoBPlusBase::NodoBPlusBase(int orden, bool es_hoja): claves(nullptr),orden(orden),num_claves(0),es_hoja(es_hoja) {
    claves = new int[orden];
}

NodoBPlusBase::~NodoBPlusBase() {
    delete[] claves;
}

int NodoBPlusBase::get_num_claves() const {
    return num_claves;
}

int NodoBPlusBase::get_clave(int idx) const {
    if (idx < 0 || idx >= num_claves) return -1;
    return claves[idx];
}

bool NodoBPlusBase::get_es_hoja() const {
    return es_hoja;
}


NodoBInterno::NodoBInterno(int orden): NodoBPlusBase(orden, false),punteros(nullptr) {
    punteros = new NodoBPlusBase*[orden + 1];
    for (int i = 0; i < orden + 1; i++) {
        punteros[i] = nullptr;
    }
}

NodoBInterno::~NodoBInterno() {
    delete[] punteros;
}

int NodoBInterno::buscar_siguiente(int clave) {
    int i = 0;
    while (i < num_claves && clave >= claves[i]) {
        i++;
    }
    return i;
}

NodoBPlusBase* NodoBInterno::get_hijo(int idx) {
    return punteros[idx];
}


NodoBHoja::NodoBHoja(int orden): NodoBPlusBase(orden, true),datos(nullptr),siguiente_hoja(nullptr) {
    datos = new NodoGrafo*[orden];
    for (int i = 0; i < orden; i++) {
        datos[i] = nullptr;
    }
}

NodoBHoja::~NodoBHoja() {
    delete[] datos;
}

NodoGrafo* NodoBHoja::buscar_en_hoja(int clave, int& accesos) {
    accesos++;

    for (int i = 0; i < num_claves; i++) {
        if (claves[i] == clave) {
            return datos[i];
        }
    }
    return nullptr;
}

NodoBHoja* NodoBHoja::get_siguiente_hoja() const {
    return siguiente_hoja;
}

void NodoBHoja::set_siguiente_hoja(NodoBHoja* sig) {
    siguiente_hoja = sig;
}