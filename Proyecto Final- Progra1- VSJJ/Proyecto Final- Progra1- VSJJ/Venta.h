#pragma once
#include <string>
#include <vector>
#include "DetalleVenta.h"
using namespace std;

// Una venta ya cobrada.
class Venta {
private:
    int numero;
    vector<DetalleVenta> detalles;
    int total;
    int efectivo;
    int cambio;

public:
    Venta();
    Venta(int numero, vector<DetalleVenta> detalles, int efectivo);

    int getNumero();
    vector<DetalleVenta> getDetalles();
    int getTotal();
    int getEfectivo();
    int getCambio();

    // pendiente: fecha y cajero de la venta
    // pendiente: guardar la venta en archivo (se coordina con Moises)
};
