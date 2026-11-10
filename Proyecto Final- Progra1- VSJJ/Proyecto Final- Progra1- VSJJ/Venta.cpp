#include "Venta.h"

Venta::Venta() {
    numero = 0;
    fecha = "";
    total = 0;
    efectivo = 0;
    cambio = 0;
}

Venta::Venta(int numero, string fecha, vector<DetalleVenta> detalles, qint64 efectivo) {
    this->numero = numero;
    this->fecha = fecha;
    this->detalles = detalles;
    this->efectivo = efectivo;

    // El total es la suma de los subtotales
    total = 0;
    for (int i = 0; i < (int)detalles.size(); i++) {
        total = total + detalles[i].subtotal();
    }

    cambio = efectivo - total;
}

int Venta::getNumero() const {
    return numero;
}

string Venta::getFecha() const {
    return fecha;
}

vector<DetalleVenta> Venta::getDetalles() const {
    return detalles;
}

qint64 Venta::getTotal() const {
    return total;
}

qint64 Venta::getEfectivo() const {
    return efectivo;
}

qint64 Venta::getCambio() const {
    return cambio;
}
