#include "DetalleVenta.h"

DetalleVenta::DetalleVenta() {
    idProducto = 0;
    nombre = "";
    precio = 0;
    cantidad = 0;
}

DetalleVenta::DetalleVenta(int idProducto, string nombre, int precio, int cantidad) {
    this->idProducto = idProducto;
    this->nombre = nombre;
    this->precio = precio;
    this->cantidad = cantidad;
}

int DetalleVenta::getIdProducto() {
    return idProducto;
}

string DetalleVenta::getNombre() {
    return nombre;
}

int DetalleVenta::getPrecio() {
    return precio;
}

int DetalleVenta::getCantidad() {
    return cantidad;
}

void DetalleVenta::setCantidad(int nuevaCantidad) {
    cantidad = nuevaCantidad;
}

// Subtotal = precio por la cantidad
int DetalleVenta::calcularSubtotal() {
    return precio * cantidad;
}
