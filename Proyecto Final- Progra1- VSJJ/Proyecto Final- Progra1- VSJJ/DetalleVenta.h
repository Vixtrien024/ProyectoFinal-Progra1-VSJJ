#pragma once
#include <string>
#include <QtGlobal>
using namespace std;

// Una linea de la venta: un producto y cuantas unidades se compran.
// Guarda el nombre y el precio del momento de la compra.
class DetalleVenta {
private:
    int codigoProducto;
    string nombreProducto;
    int cantidad;
    qint64 precioUnitario;

public:
    DetalleVenta();
    DetalleVenta(int codigoProducto, string nombreProducto, int cantidad, qint64 precioUnitario);

    int getCodigoProducto() const;
    string getNombreProducto() const;
    int getCantidad() const;
    qint64 getPrecioUnitario() const;

    void setCantidad(int nuevaCantidad);

    qint64 subtotal() const;
};
