
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

    // ======================================
    // COLECCION DE PRODUCTOS
    // ======================================

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

    // ======================================
    // METODOS INTERNOS
    // ======================================

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

    GestorProductos(
        ArchivoProductos* archivo
    );

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
    bool agregar(
        const Producto& producto
    );

    // Modificar producto existente
    // Registra la modificacion manual
    bool modificar(
        const Producto& producto
    );

    // Desactivar producto sin eliminarlo
    bool desactivar(
        int codigo
    );

    // ======================================
    // ACTUALIZACION INTERNA DE INVENTARIO
    // ======================================

    // Actualizar existencias sin generar
    // actividades de modificacion manual.
    //
    // Se utilizara desde Inventario para
    // descontar productos durante una venta
    // o restaurarlos cuando sea necesario.
    //
    // El cambio se guarda en productos.txt.
    // Si falla el guardado, se restaura
    // el valor anterior en memoria.
    //
    // El registro de ventas exitosas se
    // realiza desde GestorVentas.

    bool actualizarExistenciasInterno(
        int codigo,
        int nuevasExistencias
    );

    // ======================================
    // CONSULTAS DE PRODUCTOS
    // ======================================

    // Buscar producto por codigo
    optional<Producto> buscar(
        int codigo
    ) const;

    // Obtener todos los productos
    vector<Producto> listar() const;

    // ======================================
    // GUARDAR PRODUCTOS
    // ======================================

    // Guardar productos en el archivo
    bool guardar();
};
