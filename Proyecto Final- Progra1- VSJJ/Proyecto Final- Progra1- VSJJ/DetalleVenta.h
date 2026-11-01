#pragma once
#include <string>
using namespace std;

// Una linea de la venta: un producto y cuantas unidades se compran.
class DetalleVenta {
private:
    int idProducto;
    string nombre;
    int precio;
    int cantidad;

public:
    DetalleVenta();
    DetalleVenta(int idProducto, string nombre, int precio, int cantidad);

    int getIdProducto();
    string getNombre();
    int getPrecio();
    int getCantidad();

    void setCantidad(int nuevaCantidad);

    int calcularSubtotal();
};
