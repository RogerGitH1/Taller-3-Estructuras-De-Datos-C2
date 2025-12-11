#pragma once

class NodoGrafo{
private:
    int id;
public:
    bool es_directorio();
    int* lista_padres();
};

class NodoDirectorio: public NodoGrafo {
public:
    bool es_directorio() { return true; }
    int* lista_hijos();
};

class NodoArchivo: public NodoGrafo {
private:
    int tamaño;
    int tipo; // 0: Imagen; 1: Documento; 2: ejecutable; 3: Video; 4: Comprimido
public:
    bool es_directorio() { return false; }
};