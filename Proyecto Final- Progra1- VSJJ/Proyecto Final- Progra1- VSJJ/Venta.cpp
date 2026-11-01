#include "Venta.h"

Venta::Venta() {
    numero = 0;
    total = 0;
    efectivo = 0;
    cambio = 0;
}

Venta::Venta(int numero, vector<DetalleVenta> detalles, int efectivo) {
    this->numero = numero;
    this->detalles = detalles;
    this->efectivo = efectivo;

    // El total es la suma de los subtotales
    total = 0;
    for (int i = 0; i < (int)detalles.size(); i++) {
        total = total + detalles[i].calcularSubtotal();
    }

    cambio = efectivo - total;
}

int Venta::getNumero() {
    return numero;
}

vector<DetalleVenta> Venta::getDetalles() {
    return detalles;
}

int Venta::getTotal() {
    return total;
}

int Venta::getEfectivo() {
    return efectivo;
}

int Venta::getCambio() {
    return cambio;
}
