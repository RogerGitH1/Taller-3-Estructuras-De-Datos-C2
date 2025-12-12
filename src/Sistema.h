#pragma once

#include "NodoGrafo.h"
#include "NodoBPlusBase.h"

#include <string>

class Sistema {

private:
    NodoBPlusBase* raiz;
    int orden;
    int siguiente_id;

public:
    Sistema(int orden);
    ~Sistema();

    NodoBHoja* buscar_hoja(int clave);
    
    void insertar_nodo_grafo(int clave, NodoGrafo* nodo_grafo);

    NodoGrafo* buscar_nodo_grafo(int clave);

    void crear_nodo(int id_padre);

    void eliminar_archivo(int id_archivo, int id_directorio_padre);

    void listar_contenido(int id_directorio);

    std::string* obtener_rutas_completas(int id_archivo);

    int calcular_espacio_ocupado(int id_directorio);
};