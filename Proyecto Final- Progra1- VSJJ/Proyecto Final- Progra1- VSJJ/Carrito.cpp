#include "Carrito.h"

void Carrito::agregarProducto(Producto producto, int cantidad) {
    // No se agregan cantidades invalidas ni productos desactivados
    if (cantidad <= 0) {
        return;
    }
    if (!producto.estaActivo()) {
        return;
    }

    // Si el producto ya esta en el carrito, solo se suma la cantidad
    for (int i = 0; i < (int)articulos.size(); i++) {
        if (articulos[i].getIdProducto() == producto.getId()) {
            articulos[i].setCantidad(articulos[i].getCantidad() + cantidad);
            return;
        }
    }

    // Si no estaba, se agrega una linea nueva
    DetalleVenta nuevo(producto.getId(), producto.getNombre(), producto.getPrecio(), cantidad);
    articulos.push_back(nuevo);
}

int Carrito::calcularTotal() {
    int total = 0;
    for (int i = 0; i < (int)articulos.size(); i++) {
        total = total + articulos[i].calcularSubtotal();
    }
    return total;
}

vector<DetalleVenta> Carrito::getArticulos() {
    return articulos;
}

bool Carrito::estaVacio() {
    return articulos.empty();
}

void Carrito::vaciar() {
    articulos.clear();
}
