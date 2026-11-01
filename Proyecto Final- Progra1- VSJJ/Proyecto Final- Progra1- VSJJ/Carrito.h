#pragma once
#include <vector>
#include "DetalleVenta.h"
#include "Producto.h"
using namespace std;

// Lo que el cliente quiere comprar, antes de pagar.
class Carrito {
private:
    vector<DetalleVenta> articulos;

public:
    void agregarProducto(Producto producto, int cantidad);
    int calcularTotal();
    vector<DetalleVenta> getArticulos();
    bool estaVacio();
    void vaciar();

    // pendiente: quitar un producto del carrito
};
