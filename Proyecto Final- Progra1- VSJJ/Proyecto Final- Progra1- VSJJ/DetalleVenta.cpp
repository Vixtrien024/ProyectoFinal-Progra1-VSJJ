#include "DetalleVenta.h"

DetalleVenta::DetalleVenta()
    : idProducto(0), nombreProducto(""), precioUnitario(0), cantidad(0) {
}

DetalleVenta::DetalleVenta(int idProducto, const string& nombreProducto,
    qint64 precioUnitario, int cantidad)
    : idProducto(idProducto),
    nombreProducto(nombreProducto),
    precioUnitario(precioUnitario > 0 ? precioUnitario : 0),
    cantidad(cantidad > 0 ? cantidad : 0) {
}

int DetalleVenta::getIdProducto() const { return idProducto; }
string DetalleVenta::getNombreProducto() const { return nombreProducto; }
qint64 DetalleVenta::getPrecioUnitario() const { return precioUnitario; }
int DetalleVenta::getCantidad() const { return cantidad; }

bool DetalleVenta::setCantidad(int nuevaCantidad) {
    if (nuevaCantidad <= 0) return false;
    cantidad = nuevaCantidad;
    return true;
}

qint64 DetalleVenta::calcularSubtotal() const {
    return precioUnitario * cantidad;
}
