#include "Carrito.h"

bool Carrito::agregar(Producto producto, int cantidad) {
    // No se agregan cantidades invalidas ni productos desactivados
    if (cantidad <= 0) {
        return false;
    }
    if (!producto.estaActivo()) {
        return false;
    }

    // Si el producto ya esta en el carrito, solo se suma la cantidad
    for (int i = 0; i < (int)articulos.size(); i++) {
        if (articulos[i].getCodigoProducto() == producto.getCodigo()) {
            articulos[i].setCantidad(articulos[i].getCantidad() + cantidad);
            return true;
        }
    }

    // Si no estaba, se agrega una linea nueva
    DetalleVenta nuevo(producto.getCodigo(), producto.getNombre(), cantidad, producto.getPrecio());
    articulos.push_back(nuevo);
    return true;
}

bool Carrito::quitar(int codigo) {
    for (int i = 0; i < (int)articulos.size(); i++) {
        if (articulos[i].getCodigoProducto() == codigo) {
            articulos.erase(articulos.begin() + i);
            return true;
        }
    }
    return false;
}

bool Carrito::cambiarCantidad(int codigo, int cantidad) {
    if (cantidad <= 0) {
        return false;
    }

    for (int i = 0; i < (int)articulos.size(); i++) {
        if (articulos[i].getCodigoProducto() == codigo) {
            articulos[i].setCantidad(cantidad);
            return true;
        }
    }
    return false;
}

qint64 Carrito::total() {
    qint64 suma = 0;
    for (int i = 0; i < (int)articulos.size(); i++) {
        suma = suma + articulos[i].subtotal();
    }
    return suma;
}

vector<DetalleVenta> Carrito::detalles() {
    return articulos;
}

void Carrito::vaciar() {
    articulos.clear();
}
