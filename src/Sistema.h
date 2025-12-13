#pragma once

#include "NodoGrafo.h"
#include "NodoBPlusBase.h"

#include <string>

class Sistema {

private:
    NodoBPlusBase* raiz;
    int orden;
    int siguiente_id;
    int ultimo_accesos_bplus;

    void destruir_arbol(NodoBPlusBase* nodo);
    void insertar_recursivo(NodoBPlusBase* actual, int clave, NodoGrafo* info,int& clave_promo, NodoBPlusBase*& hijo_derecho_promo);
    void insertar_en_hoja(NodoBHoja* hoja, int clave, NodoGrafo* info);
    NodoBHoja* dividir_hoja(NodoBHoja* hoja, int clave, NodoGrafo* info, int& clave_arriba);
    void insertar_en_interno(NodoBInterno* nodo, int clave_promo, NodoBPlusBase* hijo_derecho_promo);
    void dividir_interno(NodoBInterno* nodo, int& clave_promo, NodoBPlusBase*& hijo_derecho_promo);
    bool eliminar_del_arbol(int clave);

    void rutas_dfs_por_padres(int actual,const std::string& sufijo,int* pila_ids,int depth,int max_depth,std::string*& out,int& out_len,int& out_cap);
    int dfs_espacio(int id, bool* visitado, int cap_visitado);


public:
    Sistema(int orden);
    ~Sistema();

    NodoBHoja* buscar_hoja(int clave);
    int get_ultimo_accesos_bplus() const { return ultimo_accesos_bplus; }
    
    void insertar_nodo_grafo(int clave, NodoGrafo* info);

    NodoGrafo* buscar_nodo_grafo(int clave);

    void crear_nodo(int id_padre);
    void eliminar_archivo(int id_archivo, int id_directorio_padre);
    void listar_contenido(int id_directorio);

    std::string* obtener_rutas_completas(int id_archivo);

    int calcular_espacio_ocupado(int id_directorio);

    bool enlazar_existente(int id_directorio_padre, int id_hijo_existente);
    void mostrar_nodo(int id);
};