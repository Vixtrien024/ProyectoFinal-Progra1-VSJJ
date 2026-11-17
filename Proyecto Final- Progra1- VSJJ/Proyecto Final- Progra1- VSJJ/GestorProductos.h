
#pragma once

#include <optional>
#include <vector>
#include <string>

#include "Producto.h"
#include "ArchivoProductos.h"

using namespace std;

// Declaraciones anticipadas
class Autenticacion;
class GestorActividades;

// ==========================================
// GESTOR DE PRODUCTOS - LA PULPE
// ==========================================

class GestorProductos
{
private:

    // Coleccion de productos
    vector<Producto> productos;

    // Archivo donde se guardan los productos
    // El gestor no es dueno de este objeto
    ArchivoProductos* archivo;

    // ======================================
    // REGISTRO DE ACTIVIDADES
    // ======================================

    // Gestor compartido con toda la aplicacion
    GestorActividades* gestorActividades;

    // Permite identificar al usuario conectado
    Autenticacion* autenticacion;

    // Buscar la posicion de un producto
    int posicionDe(int codigo) const;

    // Registrar operaciones de productos
    void registrarActividadProducto(
        const string& tipo,
        const string& descripcion
    );

public:

    // ======================================
    // CONSTRUCTOR
    // ======================================

    GestorProductos(ArchivoProductos* archivo);

    // ======================================
    // CONECTAR REGISTRO DE ACTIVIDADES
    // ======================================

    void setGestorActividades(
        GestorActividades* gestorActividades
    );

    void setAutenticacion(
        Autenticacion* autenticacion
    );

    // ======================================
    // ADMINISTRACION DE PRODUCTOS
    // ======================================

    // Agregar producto nuevo
    bool agregar(const Producto& producto);

    // Modificar producto existente
    bool modificar(const Producto& producto);

    // Desactivar producto sin eliminarlo
    bool desactivar(int codigo);

    // Buscar producto por codigo
    optional<Producto> buscar(int codigo) const;

    // Obtener todos los productos
    vector<Producto> listar() const;

    // Guardar productos en el archivo
    bool guardar();
};
