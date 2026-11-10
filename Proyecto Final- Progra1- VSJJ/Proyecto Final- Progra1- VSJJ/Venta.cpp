#include "Venta.h"
#include <QJsonArray>
#include <QString>

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

QJsonObject Venta::aJson() const {
    // Cada detalle se guarda como un objeto dentro de una lista
    QJsonArray listaDetalles;
    for (int i = 0; i < (int)detalles.size(); i++) {
        QJsonObject detalle;
        detalle["codigoProducto"] = detalles[i].getCodigoProducto();
        detalle["nombreProducto"] = QString::fromStdString(detalles[i].getNombreProducto());
        detalle["cantidad"] = detalles[i].getCantidad();
        detalle["precioUnitario"] = detalles[i].getPrecioUnitario();
        listaDetalles.append(detalle);
    }

    QJsonObject venta;
    venta["numero"] = numero;
    venta["fecha"] = QString::fromStdString(fecha);
    venta["detalles"] = listaDetalles;
    venta["total"] = total;
    venta["efectivo"] = efectivo;
    venta["cambio"] = cambio;
    return venta;
}

Venta Venta::desdeJson(const QJsonObject& objeto) {
    // Se leen los detalles uno por uno
    vector<DetalleVenta> detalles;
    QJsonArray listaDetalles = objeto["detalles"].toArray();
    for (int i = 0; i < listaDetalles.size(); i++) {
        QJsonObject detalle = listaDetalles[i].toObject();
        DetalleVenta nuevo(detalle["codigoProducto"].toInt(),
            detalle["nombreProducto"].toString().toStdString(),
            detalle["cantidad"].toInt(),
            detalle["precioUnitario"].toInteger());
        detalles.push_back(nuevo);
    }

    // El total y el cambio se calculan en el constructor
    return Venta(objeto["numero"].toInt(),
        objeto["fecha"].toString().toStdString(),
        detalles,
        objeto["efectivo"].toInteger());
}
