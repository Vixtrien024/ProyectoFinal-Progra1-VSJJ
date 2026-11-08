#pragma once
#include <string>
#include <vector>
#include <QtGlobal>
#include "DetalleVenta.h"
using namespace std;

// Una venta ya cobrada.
class Venta {
private:
    int numero;
    string fecha;
    vector<DetalleVenta> detalles;
    qint64 total;
    qint64 efectivo;
    qint64 cambio;

public:
    Venta();
    Venta(int numero, string fecha, vector<DetalleVenta> detalles, qint64 efectivo);

    int getNumero() const;
    string getFecha() const;
    vector<DetalleVenta> getDetalles() const;
    qint64 getTotal() const;
    qint64 getEfectivo() const;
    qint64 getCambio() const;
};
