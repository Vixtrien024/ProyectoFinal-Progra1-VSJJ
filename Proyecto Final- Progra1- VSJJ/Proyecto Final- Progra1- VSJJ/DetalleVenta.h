#pragma once

#include <string>
#include <QtGlobal>

using namespace std;

// Una linea de la venta. Guarda una copia del nombre y precio al vender,
// por eso un cambio posterior de precio no altera ventas ya hechas.
class DetalleVenta {
private:
    int idProducto;
    string nombreProducto;
    qint64 precioUnitario;
    int cantidad;

public:
    DetalleVenta();
    DetalleVenta(int idProducto, const string& nombreProducto,
        qint64 precioUnitario, int cantidad);

    int getIdProducto() const;
    string getNombreProducto() const;
    qint64 getPrecioUnitario() const;
    int getCantidad() const;

    // Devuelve false si la cantidad no es positiva
    bool setCantidad(int nuevaCantidad);

    qint64 calcularSubtotal() const;
};
