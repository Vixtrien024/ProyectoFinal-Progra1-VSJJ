#include "Venta.h"
#include <QJsonArray>
#include <QString>

Venta::Venta()
    : numero(0), fecha(""), cajero(""), total(0),
    efectivoRecibido(0), cambio(0) {
}

Venta::Venta(int numero, const string& fecha, const string& cajero,
    const vector<DetalleVenta>& detalles, qint64 efectivoRecibido)
    : numero(numero), fecha(fecha), cajero(cajero), detalles(detalles),
    total(0), efectivoRecibido(efectivoRecibido), cambio(0) {
    for (const DetalleVenta& d : this->detalles) total += d.calcularSubtotal();
    cambio = efectivoRecibido - total;
}

int Venta::getNumero() const { return numero; }
string Venta::getFecha() const { return fecha; }
string Venta::getCajero() const { return cajero; }
const vector<DetalleVenta>& Venta::getDetalles() const { return detalles; }
qint64 Venta::getTotal() const { return total; }
qint64 Venta::getEfectivoRecibido() const { return efectivoRecibido; }
qint64 Venta::getCambio() const { return cambio; }

QJsonObject Venta::aJson() const {
    QJsonArray arr;
    for (const DetalleVenta& d : detalles) {
        QJsonObject o;
        o["idProducto"] = d.getIdProducto();
        o["nombre"] = QString::fromStdString(d.getNombreProducto());
        o["precioUnitario"] = d.getPrecioUnitario();
        o["cantidad"] = d.getCantidad();
        arr.append(o);
    }
    QJsonObject v;
    v["numero"] = numero;
    v["fecha"] = QString::fromStdString(fecha);
    v["cajero"] = QString::fromStdString(cajero);
    v["detalles"] = arr;
    v["total"] = total;
    v["efectivoRecibido"] = efectivoRecibido;
    v["cambio"] = cambio;
    return v;
}

Venta Venta::desdeJson(const QJsonObject& obj) {
    vector<DetalleVenta> detalles;
    for (const QJsonValue& val : obj["detalles"].toArray()) {
        QJsonObject o = val.toObject();
        detalles.push_back(DetalleVenta(
            o["idProducto"].toInt(),
            o["nombre"].toString().toStdString(),
            o["precioUnitario"].toInteger(),
            o["cantidad"].toInt()));
    }
    return Venta(obj["numero"].toInt(),
        obj["fecha"].toString().toStdString(),
        obj["cajero"].toString().toStdString(),
        detalles,
        obj["efectivoRecibido"].toInteger());
}
