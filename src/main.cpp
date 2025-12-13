#include "Sistema.h"
#include <iostream>

using namespace std;

static void menu() {
    cout << "\n===== TALLER 3 - MENU =====\n";
    cout << "1) Crear nodo (archivo/directorio)\n";
    cout << "2) Listar contenido de directorio\n";
    cout << "3) Calcular espacio ocupado de directorio\n";
    cout << "4) Buscar/mostrar nodo por ID (incluye accesos B+)\n";
    cout << "5) Eliminar archivo (desenlazar de un padre; si queda sin padres se borra del B+)\n";
    cout << "6) Obtener rutas completas (TODAS) de un archivo (devuelve string*)\n";
    cout << "7) Enlazar nodo existente en directorio (crear padre extra)\n";
    cout << "0) Salir\n";
    cout << "Opcion: ";
}

int main() {
    Sistema s(4);
    cout << "Sistema iniciado\n";

    NodoDirectorio* root = new NodoDirectorio(0);
    s.insertar_nodo_grafo(0, root);

    int op = -1;
    while (op != 0) {
        menu();
        cin >> op;

        if (op == 1) {
            int padre;
            cout << "ID directorio padre: ";
            cin >> padre;
            s.crear_nodo(padre);

        } else if (op == 2) {
            int dir;
            cout << "ID directorio: ";
            cin >> dir;
            s.listar_contenido(dir);

        } else if (op == 3) {
            int dir;
            cout << "ID directorio: ";
            cin >> dir;
            cout << "Espacio ocupado (sin doble conteo): " << s.calcular_espacio_ocupado(dir) << "\n";

        } else if (op == 4) {
            int id;
            cout << "ID nodo: ";
            cin >> id;
            s.mostrar_nodo(id);

        } else if (op == 5) {
            int id_arch, id_padre;
            cout << "ID archivo: ";
            cin >> id_arch;
            cout << "ID directorio padre: ";
            cin >> id_padre;
            s.eliminar_archivo(id_arch, id_padre);

        } else if (op == 6) {
            int id_arch;
            cout << "ID archivo: ";
            cin >> id_arch;

            string* rutas = s.obtener_rutas_completas(id_arch);

            cout << "Rutas completas (todas):\n";
            int i = 0;
            while (!rutas[i].empty()) {
                cout << "  - " << rutas[i] << "\n";
                i++;
            }

            delete[] rutas;

        } else if (op == 7) {
            int padre, hijo;
            cout << "ID directorio padre: ";
            cin >> padre;
            cout << "ID nodo existente a enlazar: ";
            cin >> hijo;
            s.enlazar_existente(padre, hijo);

        } else if (op == 0) {
            cout << "Saliendo...\n";
        } else {
            cout << "Opcion invalida.\n";
        }
    }

    return 0;
}