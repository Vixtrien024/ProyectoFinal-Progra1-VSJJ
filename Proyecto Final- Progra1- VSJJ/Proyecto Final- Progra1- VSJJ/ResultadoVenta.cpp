#include "ResultadoVenta.h"

ResultadoVenta::ResultadoVenta(bool exitosa, string mensaje, Venta venta) {
    this->exitosa = exitosa;
    this->mensaje = mensaje;
    this->venta = venta;
}

bool ResultadoVenta::fueExitosa() {
    return exitosa;
}

string ResultadoVenta::getMensaje() {
    return mensaje;
}

Venta ResultadoVenta::getVenta() {
    return venta;
}
