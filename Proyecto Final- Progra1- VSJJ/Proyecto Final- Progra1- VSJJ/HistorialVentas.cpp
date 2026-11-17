
#include "HistorialVentas.h"
#include "GeneradorTicket.h"

#include <QJsonArray>
#include <QJsonObject>

using namespace std;

// Constructor
HistorialVentas::HistorialVentas(ArchivoVentas* archivoVentas)
{
    archivo = archivoVentas;
}

// Devuelve todas las ventas registradas
vector<Venta> HistorialVentas::listar()
{
    vector<Venta> ventas;

    if (archivo == nullptr)
    {
        return ventas;
    }

    bool correcto = false;

    QJsonArray datos = archivo->cargarVentas(&correcto);

    if (!correcto)
    {
        return ventas;
    }

    for (const QJsonValue& valor : datos)
    {
        if (valor.isObject())
        {
            QJsonObject objeto = valor.toObject();

            Venta venta = Venta::desdeJson(objeto);

            ventas.push_back(venta);
        }
    }

    return ventas;
}

// Busca una venta por su numero
optional<Venta> HistorialVentas::buscar(int numero)
{
    vector<Venta> ventas = listar();

    for (const Venta& venta : ventas)
    {
        if (venta.getNumero() == numero)
        {
            return venta;
        }
    }

    return nullopt;
}

// Busca una venta y genera su ticket
bool HistorialVentas::generarTicket(
    int numero,
    GeneradorTicket& generador)
{
    optional<Venta> venta = buscar(numero);

    if (!venta.has_value())
    {
        return false;
    }

    return generador.generar(venta.value());
}
