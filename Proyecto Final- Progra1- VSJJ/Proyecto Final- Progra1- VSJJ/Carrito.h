#pragma once

#include <vector>
#include <QtGlobal>
#include "DetalleVenta.h"
#include "Producto.h"

using namespace std;

// Compra en preparacion, aun sin cobrar.
class Carrito {
private:
    vector<DetalleVenta> articulos;

public:
    // Suma la cantidad si el producto ya esta. false si el producto esta
    // inactivo o la cantidad no es positiva.
    bool agregarProducto(const Producto& producto, int cantidad);
    bool quitarProducto(int idProducto);
    void vaciar();

    const vector<DetalleVenta>& getArticulos() const;
    bool estaVacio() const;
    qint64 calcularTotal() const;
};
