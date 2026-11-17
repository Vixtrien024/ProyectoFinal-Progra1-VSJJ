#include "DetalleVenta.h"

DetalleVenta::DetalleVenta() {
    codigoProducto = 0;
    nombreProducto = "";
    cantidad = 0;
    precioUnitario = 0;
}

DetalleVenta::DetalleVenta(int codigoProducto, string nombreProducto, int cantidad, qint64 precioUnitario) {
    this->codigoProducto = codigoProducto;
    this->nombreProducto = nombreProducto;
    this->cantidad = cantidad;
    this->precioUnitario = precioUnitario;
}

int DetalleVenta::getCodigoProducto() const {
    return codigoProducto;
}

string DetalleVenta::getNombreProducto() const {
    return nombreProducto;
}

int DetalleVenta::getCantidad() const {
    return cantidad;
}

qint64 DetalleVenta::getPrecioUnitario() const {
    return precioUnitario;
}

void DetalleVenta::setCantidad(int nuevaCantidad) {
    cantidad = nuevaCantidad;
}

// Subtotal = precio unitario por la cantidad
qint64 DetalleVenta::subtotal() const {
    return precioUnitario * cantidad;
}
