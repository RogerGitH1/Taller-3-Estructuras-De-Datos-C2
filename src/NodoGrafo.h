#pragma once

class NodoGrafo{
protected:
    int id;
    int* padres;
    int num_padres;
    int cap_padres;

public: 
    NodoGrafo(int id);
    virtual ~NodoGrafo();

    int get_id() const;
    virtual bool es_directorio() = 0;
    int* lista_padres();
    int get_num_padres() const;
    void agregar_padre(int id_padre);
    void eliminar_padre(int id_padre);   
};


class NodoDirectorio: public NodoGrafo {
private:
    int* hijos;
    int num_hijos;
    int cap_hijos;

public:
    NodoDirectorio(int id);
    virtual ~NodoDirectorio();

    bool es_directorio() override { return true; }
    int* lista_hijos();
    int get_num_hijos() const;
    void agregar_hijo(int id_hijo);
    void eliminar_hijo(int id_hijo);
};


class NodoArchivo: public NodoGrafo {
private:
    int tamano;
    int tipo;
public:
    NodoArchivo(int id, int tamano, int tipo);
    virtual ~NodoArchivo();

    bool es_directorio() override { return false; }

    int get_tamano() const;
    int get_tipo() const;
};