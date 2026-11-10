#pragma once

#include <string>
#include <vector>
#include "Producto.h"

using namespace std;

class ArchivoProductos {
private:
    string ruta;//Ruta del archivo, por ejemplo "productos.txt"

public:
    ArchivoProductos(const string& ruta);

    vector<Producto> cargar();//Lee el archivo (vacio si no existe)
    bool guardar(const vector<Producto>& productos);//Reescribe el archivo completo
};//fin de la clase