#pragma once

#include <string>
#include <vector>
#include <QtGlobal>
#include <QJsonObject>
#include "DetalleVenta.h"

using namespace std;

// Venta confirmada. Contiene (composicion) sus detalles.
class Venta {
private:
    int numero;
    string fecha;
    string cajero;
    vector<DetalleVenta> detalles;
    qint64 total;
    qint64 efectivoRecibido;
    qint64 cambio;

public:
    Venta();
    Venta(int numero, const string& fecha, const string& cajero,
        const vector<DetalleVenta>& detalles,
        qint64 efectivoRecibido);

    int getNumero() const;
    string getFecha() const;
    string getCajero() const;
    const vector<DetalleVenta>& getDetalles() const;
    qint64 getTotal() const;
    qint64 getEfectivoRecibido() const;
    qint64 getCambio() const;

    // Conversion para ArchivoVentas (Moises), que guarda QJsonArray
    QJsonObject aJson() const;
    static Venta desdeJson(const QJsonObject& obj);
};
