#include "Sistema.h"
#include <iostream>

using namespace std;

Sistema::Sistema(int orden): raiz(nullptr), orden(orden), siguiente_id(1), ultimo_accesos_bplus(0) {
    raiz = new NodoBHoja(orden);
}

Sistema::~Sistema() {
    destruir_arbol(raiz);
}

void Sistema::destruir_arbol(NodoBPlusBase* nodo) {
    if (!nodo) return;

    if (!nodo->get_es_hoja()) {
        NodoBInterno* interno = (NodoBInterno*)nodo;
        for (int i = 0; i <= interno->get_num_claves(); ++i) {
            destruir_arbol(interno->get_hijo(i));
        }
    }
    delete nodo;
}

NodoBHoja* Sistema::buscar_hoja(int clave) {
    NodoBPlusBase* actual = raiz;

    while (!actual->get_es_hoja()) {
        NodoBInterno* interno = (NodoBInterno*)actual;
        int idx = interno->buscar_siguiente(clave);
        actual = interno->get_hijo(idx);
    }

    return (NodoBHoja*)actual;
}

NodoGrafo* Sistema::buscar_nodo_grafo(int clave) {
    int accesos = 0;
    NodoBHoja* hoja = buscar_hoja(clave);
    NodoGrafo* res = hoja->buscar_en_hoja(clave, accesos);
    ultimo_accesos_bplus = accesos;
    return res;
}

void Sistema::insertar_en_hoja(NodoBHoja* hoja, int clave, NodoGrafo* info) {
    int usados = hoja->get_num_claves();
    int i = usados - 1;

    while (i >= 0 && hoja->get_clave(i) > clave) {
        hoja->set_clave(i + 1, hoja->get_clave(i));
        hoja->set_dato(i + 1, hoja->get_datos()[i]);
        i--;
    }

    hoja->set_clave(i + 1, clave);
    hoja->set_dato(i + 1, info);
    hoja->set_num_claves(usados + 1);
}

NodoBHoja* Sistema::dividir_hoja(NodoBHoja* hoja, int clave, NodoGrafo* info, int& clave_arriba) {
    int m = hoja->get_orden();
    int total = m + 1;

    int* claves_temp = new int[total];
    NodoGrafo** datos_temp = new NodoGrafo*[total];

    int usadas = hoja->get_num_claves();
    int i = 0;

    while (i < usadas && hoja->get_clave(i) < clave) {
        claves_temp[i] = hoja->get_clave(i);
        datos_temp[i] = hoja->get_datos()[i];
        i++;
    }

    claves_temp[i] = clave;
    datos_temp[i] = info;

    for (int j = i; j < usadas; j++) {
        claves_temp[j + 1] = hoja->get_clave(j);
        datos_temp[j + 1] = hoja->get_datos()[j];
    }

    int mitad = total / 2;
    NodoBHoja* nueva = new NodoBHoja(m);

    hoja->set_num_claves(0);
    for (int j = 0; j < mitad; j++) {
        hoja->set_clave(j, claves_temp[j]);
        hoja->set_dato(j, datos_temp[j]);
    }
    hoja->set_num_claves(mitad);

    int k = 0;
    for (int j = mitad; j < total; j++) {
        nueva->set_clave(k, claves_temp[j]);
        nueva->set_dato(k, datos_temp[j]);
        k++;
    }
    nueva->set_num_claves(k);

    nueva->set_siguiente_hoja(hoja->get_siguiente_hoja());
    hoja->set_siguiente_hoja(nueva);

    clave_arriba = nueva->get_clave(0);

    delete[] claves_temp;
    delete[] datos_temp;

    return nueva;
}

void Sistema::insertar_en_interno(NodoBInterno* nodo, int clave_promo, NodoBPlusBase* hijo_derecho_promo) {
    int usadas = nodo->get_num_claves();
    int i = usadas - 1;

    while (i >= 0 && nodo->get_clave(i) > clave_promo) {
        nodo->set_clave(i + 1, nodo->get_clave(i));
        nodo->set_hijo(i + 2, nodo->get_hijo(i + 1));
        i--;
    }

    nodo->set_clave(i + 1, clave_promo);
    nodo->set_hijo(i + 2, hijo_derecho_promo);
    nodo->set_num_claves(usadas + 1);
}

void Sistema::dividir_interno(NodoBInterno* nodo, int& clave_promo, NodoBPlusBase*& hijo_derecho_promo) {
    int m = nodo->get_orden();
    int mitad = m / 2;

    int* claves_temp = new int[m + 1];
    NodoBPlusBase** hijos_temp = new NodoBPlusBase*[m + 2];

    int i = 0;
    while (i < m && nodo->get_clave(i) < clave_promo) {
        claves_temp[i] = nodo->get_clave(i);
        hijos_temp[i] = nodo->get_hijo(i);
        i++;
    }

    claves_temp[i] = clave_promo;
    hijos_temp[i] = nodo->get_hijo(i);
    hijos_temp[i + 1] = hijo_derecho_promo;

    for (int j = i; j < m; j++) {
        claves_temp[j + 1] = nodo->get_clave(j);
        hijos_temp[j + 2] = nodo->get_hijo(j + 1);
    }

    int nueva_clave_promo = claves_temp[mitad];
    NodoBInterno* nuevo_interno = new NodoBInterno(m);

    nodo->set_num_claves(mitad);
    for (int j = 0; j < mitad; j++) {
        nodo->set_clave(j, claves_temp[j]);
        nodo->set_hijo(j, hijos_temp[j]);
    }
    nodo->set_hijo(mitad, hijos_temp[mitad]);

    nuevo_interno->set_num_claves(m - mitad);
    for (int j = 0; j < m - mitad; j++) {
        nuevo_interno->set_clave(j, claves_temp[mitad + 1 + j]);
        nuevo_interno->set_hijo(j, hijos_temp[mitad + 1 + j]);
    }
    nuevo_interno->set_hijo(m - mitad, hijos_temp[m + 1]);

    clave_promo = nueva_clave_promo;
    hijo_derecho_promo = nuevo_interno;

    delete[] claves_temp;
    delete[] hijos_temp;
}

void Sistema::insertar_recursivo(NodoBPlusBase* actual, int clave, NodoGrafo* info,int& clave_promo, NodoBPlusBase*& hijo_derecho_promo) {
    if (actual->get_es_hoja()) {
        NodoBHoja* hoja = (NodoBHoja*)actual;

        if (hoja->get_num_claves() < orden) {
            insertar_en_hoja(hoja, clave, info);
            clave_promo = -1;
            hijo_derecho_promo = nullptr;
        } else {
            hijo_derecho_promo = dividir_hoja(hoja, clave, info, clave_promo);
        }
        return;
    }

    NodoBInterno* interno = (NodoBInterno*)actual;
    int pos = interno->buscar_siguiente(clave);

    insertar_recursivo(interno->get_hijo(pos), clave, info, clave_promo, hijo_derecho_promo);

    if (clave_promo != -1) {
        if (interno->get_num_claves() < orden) {
            insertar_en_interno(interno, clave_promo, hijo_derecho_promo);
            clave_promo = -1;
            hijo_derecho_promo = nullptr;
        } else {
            dividir_interno(interno, clave_promo, hijo_derecho_promo);
        }
    }
}

void Sistema::insertar_nodo_grafo(int clave, NodoGrafo* info) {
    int clave_promo = -1;
    NodoBPlusBase* hijo_derecho_promo = nullptr;

    insertar_recursivo(raiz, clave, info, clave_promo, hijo_derecho_promo);

    if (clave_promo != -1) {
        NodoBInterno* nueva_raiz = new NodoBInterno(orden);
        nueva_raiz->set_clave(0, clave_promo);
        nueva_raiz->set_hijo(0, raiz);
        nueva_raiz->set_hijo(1, hijo_derecho_promo);
        nueva_raiz->set_num_claves(1);
        raiz = nueva_raiz;
    }
}

bool Sistema::eliminar_del_arbol(int clave) {
    NodoBHoja* hoja = buscar_hoja(clave);
    int n = hoja->get_num_claves();

    int pos = -1;
    for (int i = 0; i < n; i++) {
        if (hoja->get_clave(i) == clave) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return false;

    for (int i = pos; i < n - 1; i++) {
        hoja->set_clave(i, hoja->get_clave(i + 1));
        hoja->set_dato(i, hoja->get_datos()[i + 1]);
    }
    hoja->set_num_claves(n - 1);
    return true;
}

void Sistema::crear_nodo(int id_padre) {
    NodoGrafo* padre = buscar_nodo_grafo(id_padre);

    if (!padre || !padre->es_directorio()) {
        cout << "Error: padre no valido\n";
        return;
    }

    int tipo;
    cout << "Crear (0 = Directorio, 1 = Archivo): ";
    cin >> tipo;

    NodoGrafo* nuevo = nullptr;
    int id = siguiente_id++;

    if (tipo == 0) {
        nuevo = new NodoDirectorio(id);
    } else {
        int tam, t;
        cout << "Tamano archivo: ";
        cin >> tam;
        cout << "Tipo archivo (0-4): ";
        cin >> t;
        nuevo = new NodoArchivo(id, tam, t);
    }

    nuevo->agregar_padre(id_padre);
    ((NodoDirectorio*)padre)->agregar_hijo(id);

    insertar_nodo_grafo(id, nuevo);

    cout << "Nodo creado con ID " << id << "\n";
    cout << "[B+] accesos (buscar padre): " << get_ultimo_accesos_bplus() << "\n";
}


void Sistema::listar_contenido(int id_directorio) {
    NodoGrafo* dir = buscar_nodo_grafo(id_directorio);

    if (!dir || !dir->es_directorio()) {
        cout << "No es un directorio valido\n";
        return;
    }

    NodoDirectorio* d = (NodoDirectorio*)dir;

    cout << "Contenido del directorio " << id_directorio << ":\n";
    if (d->get_num_hijos() == 0) {
        cout << "(vacio)\n";
        return;
    }

    for (int i = 0; i < d->get_num_hijos(); i++) {
        int id_hijo = d->lista_hijos()[i];
        NodoGrafo* hijo = buscar_nodo_grafo(id_hijo);
        if (!hijo) continue;

        cout << "- ID " << id_hijo << (hijo->es_directorio() ? " [DIR]" : " [FILE]");

        if (!hijo->es_directorio()) {
            NodoArchivo* a = (NodoArchivo*)hijo;
            cout << " tam=" << a->get_tamano() << " tipo=" << a->get_tipo();
        }
        cout << "\n";
    }
}

void Sistema::eliminar_archivo(int id_archivo, int id_directorio_padre) {
    NodoGrafo* archivo = buscar_nodo_grafo(id_archivo);
    if (!archivo || archivo->es_directorio()) {
        cout << "Archivo no valido\n";
        return;
    }

    NodoGrafo* padre = buscar_nodo_grafo(id_directorio_padre);
    if (!padre || !padre->es_directorio()) {
        cout << "Directorio padre no valido\n";
        return;
    }

    archivo->eliminar_padre(id_directorio_padre);
    ((NodoDirectorio*)padre)->eliminar_hijo(id_archivo);

    if (archivo->get_num_padres() == 0) {
        bool ok = eliminar_del_arbol(id_archivo);
        (void)ok;
        delete archivo;
        cout << "Archivo eliminado completamente (sin padres)\n";
    } else {
        cout << "Enlace eliminado. Padres restantes: " << archivo->get_num_padres() << "\n";
    }
}

static void push_ruta(std::string*& out, int& out_len, int& out_cap, const std::string& ruta) {
    if (out_len + 1 >= out_cap) {
        int nuevo_cap = (out_cap == 0) ? 8 : out_cap * 2;
        std::string* nuevo = new std::string[nuevo_cap];
        for (int i = 0; i < out_len; i++) nuevo[i] = out[i];
        delete[] out;
        out = nuevo;
        out_cap = nuevo_cap;
    }
    out[out_len++] = ruta;
}

void Sistema::rutas_dfs_por_padres(int actual,const std::string& sufijo,int* pila_ids,int depth,int max_depth,std::string*& out,int& out_len,int& out_cap) {
    if (depth >= max_depth) return;

    for (int i = 0; i < depth; i++) {
        if (pila_ids[i] == actual) return;
    }

    pila_ids[depth] = actual;
    std::string nuevo = "/" + std::to_string(actual) + sufijo;

    NodoGrafo* n = buscar_nodo_grafo(actual);
    if (!n || n->get_num_padres() == 0) {
        push_ruta(out, out_len, out_cap, nuevo);
        return;
    }

    for (int i = 0; i < n->get_num_padres(); i++) {
        int p = n->lista_padres()[i];
        rutas_dfs_por_padres(p, nuevo, pila_ids, depth + 1, max_depth, out, out_len, out_cap);
    }
}

std::string* Sistema::obtener_rutas_completas(int id_archivo) {
    NodoGrafo* nodo = buscar_nodo_grafo(id_archivo);
    if (!nodo || nodo->es_directorio()) {
        std::string* v = new std::string[1];
        v[0] = "";
        return v;
    }

    std::string* out = nullptr;
    int out_len = 0;
    int out_cap = 0;

    const int MAXD = 512;
    int pila[MAXD];

    rutas_dfs_por_padres(id_archivo, "", pila, 0, MAXD, out, out_len, out_cap);

    push_ruta(out, out_len, out_cap, "");
    return out;
}

int Sistema::dfs_espacio(int id, bool* visitado, int cap_visitado) {
    if (id < 0 || id >= cap_visitado) return 0;
    if (visitado[id]) return 0;
    visitado[id] = true;

    NodoGrafo* nodo = buscar_nodo_grafo(id);
    if (!nodo) return 0;

    if (!nodo->es_directorio()) {
        return ((NodoArchivo*)nodo)->get_tamano();
    }

    int total = 0;
    NodoDirectorio* d = (NodoDirectorio*)nodo;
    for (int i = 0; i < d->get_num_hijos(); i++) {
        total += dfs_espacio(d->lista_hijos()[i], visitado, cap_visitado);
    }
    return total;
}

int Sistema::calcular_espacio_ocupado(int id_directorio) {
    int cap = (siguiente_id < 2) ? 2 : siguiente_id + 1;
    bool* visitado = new bool[cap];
    for (int i = 0; i < cap; i++) visitado[i] = false;

    int total = dfs_espacio(id_directorio, visitado, cap);
    delete[] visitado;
    return total;
}

bool Sistema::enlazar_existente(int id_directorio_padre, int id_hijo_existente) {
    NodoGrafo* padre = buscar_nodo_grafo(id_directorio_padre);
    NodoGrafo* hijo = buscar_nodo_grafo(id_hijo_existente);

    if (!padre || !padre->es_directorio()) {
        cout << "Error: padre no es directorio\n";
        return false;
    }
    if (!hijo) {
        cout << "Error: no existe hijo con ese ID\n";
        return false;
    }

    NodoDirectorio* d = (NodoDirectorio*)padre;

    for (int i = 0; i < d->get_num_hijos(); i++) {
        if (d->lista_hijos()[i] == id_hijo_existente) {
            cout << "Ya existe ese hijo en el directorio\n";
            return false;
        }
    }

    d->agregar_hijo(id_hijo_existente);
    hijo->agregar_padre(id_directorio_padre);

    cout << "Enlace creado: " << id_directorio_padre << " -> " << id_hijo_existente << "\n";
    return true;
}

void Sistema::mostrar_nodo(int id) {
    NodoGrafo* n = buscar_nodo_grafo(id);

    if (!n) {
        cout << "No existe nodo con ID " << id << " (accesos B+: " << get_ultimo_accesos_bplus() << ")\n";
        return;
    }

    cout << "Nodo ID " << id << " -> "
         << (n->es_directorio() ? "DIRECTORIO" : "ARCHIVO")
         << " | accesos B+: " << get_ultimo_accesos_bplus() << "\n";

    cout << "Padres (" << n->get_num_padres() << "): ";
    for (int i = 0; i < n->get_num_padres(); i++) {
        cout << n->lista_padres()[i] << (i + 1 == n->get_num_padres() ? "" : ", ");
    }
    cout << "\n";

    if (!n->es_directorio()) {
        NodoArchivo* a = (NodoArchivo*)n;
        cout << "Tamano: " << a->get_tamano() << " | Tipo: " << a->get_tipo() << "\n";
    } else {
        NodoDirectorio* d = (NodoDirectorio*)n;
        cout << "Hijos: " << d->get_num_hijos() << "\n";
    }
}