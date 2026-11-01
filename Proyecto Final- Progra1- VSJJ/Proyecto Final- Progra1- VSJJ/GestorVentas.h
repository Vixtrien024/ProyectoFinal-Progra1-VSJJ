#pragma once
#include <string>
#include <vector>
#include "Carrito.h"
#include "Venta.h"
using namespace std;

// Se encarga de cobrar y guardar las ventas en memoria.
class GestorVentas {
private:
    vector<Venta> ventas;

public:
    // Devuelve "OK" si se cobro, o el motivo si fue rechazada
    string procesarVenta(Carrito carrito, int efectivo);

    int getCantidadVentas();
    Venta getUltimaVenta();

    // pendiente: revisar y descontar existencias (Inventario de Julian)
    // pendiente: guardar las ventas en archivo (Moises)
};
