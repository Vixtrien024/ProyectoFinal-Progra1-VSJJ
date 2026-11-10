#pragma once

#include <optional>
#include <vector>
#include "Producto.h"
#include "ArchivoProductos.h"

using namespace std;

class GestorProductos {
private:
    vector<Producto> productos;//Coleccion donde se guardaran los productos
    ArchivoProductos* archivo;//Archivo donde se guardan los cambios (no es dueno de el)

    int posicionDe(int codigo) const;//Posicion en el vector, o -1 si no existe

public:
    GestorProductos(ArchivoProductos* archivo);//Carga los productos del archivo al crearse

    bool agregar(const Producto& producto);//Registra un producto nuevo
    bool modificar(const Producto& producto);//Actualiza el producto que tenga el mismo codigo
    bool desactivar(int codigo);//Desactiva el producto (NO lo borra)
    optional<Producto> buscar(int codigo) const;//Devuelve una COPIA, o nada si no existe
    vector<Producto> listar() const;//Devuelve todos los productos
    bool guardar();//Escribe la coleccion en el archivo

};//fin de la clase