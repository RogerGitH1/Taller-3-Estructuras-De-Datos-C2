#include "NodoGrafo.h"
#include <iostream>

using namespace std;
const int CAP_INICIAL = 4;

NodoGrafo::NodoGrafo(int id): id(id),padres(nullptr),num_padres(0),cap_padres(0) {
    cap_padres = CAP_INICIAL;
    padres = new int[cap_padres];
}

NodoGrafo::~NodoGrafo() {
    delete[] padres;
}

int NodoGrafo::get_id() const {
    return id;
}

int* NodoGrafo::lista_padres() {
    return padres;
}

int NodoGrafo::get_num_padres() const {
    return num_padres;
}

void NodoGrafo::agregar_padre(int id_padre) {
    if (num_padres == cap_padres) {
        int nueva_cap = cap_padres * 2;
        int* nuevo_arreglo = new int[nueva_cap];

        for (int i = 0; i < num_padres; i++) {
            nuevo_arreglo[i] = padres[i];
        }

        delete[] padres;
        padres = nuevo_arreglo;
        cap_padres = nueva_cap;
    }

    padres[num_padres] = id_padre;
    num_padres++;
}

void NodoGrafo::eliminar_padre(int id_padre) {
    int indice = -1;
    for (int i = 0; i < num_padres; i++) {
        if (padres[i] == id_padre) {
            indice = i;
            break;
        }
    }
    if (indice == -1) return; 

    for (int i = indice; i < num_padres - 1; i++) {
        padres[i] = padres[i + 1];
    }
    num_padres--;
}


NodoDirectorio::NodoDirectorio(int id): NodoGrafo(id),hijos(nullptr),num_hijos(0),cap_hijos(0) {
    cap_hijos = CAP_INICIAL;
    hijos = new int[cap_hijos];
}

NodoDirectorio::~NodoDirectorio() {
    delete[] hijos;
}

int* NodoDirectorio::lista_hijos() {
    return hijos;
}

int NodoDirectorio::get_num_hijos() const {
    return num_hijos;
}

void NodoDirectorio::agregar_hijo(int id_hijo) {
    if (num_hijos == cap_hijos) {
        int nueva_cap = cap_hijos * 2;
        int* nuevo_arreglo = new int[nueva_cap];

        for (int i = 0; i < num_hijos; i++) {
            nuevo_arreglo[i] = hijos[i];
        }

        delete[] hijos;
        hijos = nuevo_arreglo;
        cap_hijos = nueva_cap;
    }

    hijos[num_hijos] = id_hijo;
    num_hijos++;
}

void NodoDirectorio::eliminar_hijo(int id_hijo) {
    int indice = -1;
    for (int i = 0; i < num_hijos; i++) {
        if (hijos[i] == id_hijo) {
            indice = i;
            break;
        }
    }

    if (indice == -1) return;

    for (int i = indice; i < num_hijos - 1; i++) {
        hijos[i] = hijos[i + 1];
    }
    num_hijos--;
}


NodoArchivo::NodoArchivo(int id, int tamaño, int tipo): NodoGrafo(id),tamaño(tamaño),tipo(tipo) {
}

NodoArchivo::~NodoArchivo() {
}

int NodoArchivo::get_tamaño() const {
    return tamaño;
}

int NodoArchivo::get_tipo() const {
    return tipo;
}