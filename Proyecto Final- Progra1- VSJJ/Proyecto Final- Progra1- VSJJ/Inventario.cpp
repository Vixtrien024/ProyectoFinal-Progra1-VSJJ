
#include "Inventario.h"

using namespace std;

// ==========================================
// CONSTRUCTOR
// ==========================================

Inventario::Inventario(GestorProductos* gestor)
    : gestor(gestor)
{
}

// ==========================================
// COMPROBAR EXISTENCIAS
// ==========================================

bool Inventario::hayExistencias(
    int codigo,
    int cantidad) const
{
    // Verificar gestor y cantidad
    if (gestor == nullptr || cantidad <= 0)
    {
        return false;
    }

    // Buscar el producto
    optional<Producto> producto =
        gestor->buscar(codigo);

    // Verificar que exista y este activo
    if (!producto.has_value() ||
        !producto->estaActivo())
    {
        return false;
    }

    // Verificar cantidad disponible
    return producto->getExistencias() >= cantidad;
}

// ==========================================
// DESCONTAR EXISTENCIAS POR VENTA
// ==========================================

bool Inventario::descontar(
    int codigo,
    int cantidad)
{
    // Verificar que existan suficientes unidades
    if (!hayExistencias(codigo, cantidad))
    {
        return false;
    }

    // Obtener producto actual
    optional<Producto> encontrado =
        gestor->buscar(codigo);

    if (!encontrado.has_value())
    {
        return false;
    }

    // Calcular nuevas existencias
    int nuevasExistencias =
        encontrado->getExistencias() - cantidad;

    // Actualizar y guardar sin registrar
    // una modificacion manual del producto
    return gestor->actualizarExistenciasInterno(
        codigo,
        nuevasExistencias
    );
}

// ==========================================
// REPONER EXISTENCIAS
// ==========================================

bool Inventario::reponer(
    int codigo,
    int cantidad)
{
    // Verificar gestor y cantidad
    if (gestor == nullptr || cantidad <= 0)
    {
        return false;
    }

    // Buscar producto
    optional<Producto> encontrado =
        gestor->buscar(codigo);

    // Verificar que exista y este activo
    if (!encontrado.has_value() ||
        !encontrado->estaActivo())
    {
        return false;
    }

    // Calcular existencias restauradas
    int nuevasExistencias =
        encontrado->getExistencias() + cantidad;

    // Guardar sin generar actividades
    // de modificacion manual
    return gestor->actualizarExistenciasInterno(
        codigo,
        nuevasExistencias
    );
}
