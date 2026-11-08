#pragma once
#include <vector>
#include <QtGlobal>
#include "DetalleVenta.h"
#include "Producto.h"
using namespace std;

// Lo que el cliente quiere comprar, antes de pagar.
class Carrito {
private:
    vector<DetalleVenta> articulos;

public:
    // Devuelve false si no se pudo (cantidad invalida o producto desactivado)
    bool agregar(Producto producto, int cantidad);

    // Devuelve false si el codigo no esta en el carrito
    bool quitar(int codigo);

    // Devuelve false si la cantidad es invalida o el codigo no esta
    bool cambiarCantidad(int codigo, int cantidad);

    qint64 total();
    vector<DetalleVenta> detalles();
    void vaciar();
};
