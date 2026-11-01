#include "GestorVentas.h"

string GestorVentas::procesarVenta(Carrito carrito, int efectivo) {
    if (carrito.estaVacio()) {
        return "El carrito esta vacio";
    }

    if (efectivo < carrito.calcularTotal()) {
        return "El efectivo no alcanza para pagar";
    }

    // El numero de venta es la cantidad de ventas + 1
    int numero = (int)ventas.size() + 1;
    Venta nueva(numero, carrito.getArticulos(), efectivo);
    ventas.push_back(nueva);

    return "OK";
}

int GestorVentas::getCantidadVentas() {
    return (int)ventas.size();
}

Venta GestorVentas::getUltimaVenta() {
    return ventas[ventas.size() - 1];
}
