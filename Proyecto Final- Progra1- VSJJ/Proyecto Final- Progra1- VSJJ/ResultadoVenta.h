#pragma once
#include <string>
#include "Venta.h"
using namespace std;

// Lo que devuelve GestorVentas al intentar cobrar.
// Si exitosa es true, "venta" trae la venta cobrada.
// Si es false, "mensaje" dice por que se rechazo.
class ResultadoVenta {
private:
    bool exitosa;
    string mensaje;
    Venta venta;

public:
    ResultadoVenta(bool exitosa, string mensaje, Venta venta);

    bool fueExitosa();
    string getMensaje();
    Venta getVenta();
};
