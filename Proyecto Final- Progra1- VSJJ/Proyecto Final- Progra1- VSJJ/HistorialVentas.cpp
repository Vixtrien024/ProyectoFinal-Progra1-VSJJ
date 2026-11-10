#include "HistorialVentas.h"
#include "GeneradorTicket.h"

using namespace std;

// Constructor
HistorialVentas::HistorialVentas(ArchivoVentas* archivoVentas)
{
    archivo = archivoVentas;
}

// Devuelve todas las ventas registradas
vector<Venta> HistorialVentas::listar()
{
    if (archivo == nullptr)
    {
        return {};
    }

    return archivo->cargar();
}

// Busca una venta por su número
optional<Venta> HistorialVentas::buscar(int numero)
{
    if (archivo == nullptr)
    {
        return nullopt;
    }

    return archivo->buscar(numero);
}

// Busca una venta y genera su ticket
bool HistorialVentas::generarTicket(
    int numero,
    GeneradorTicket& generador)
{
    if (archivo == nullptr)
    {
        return false;
    }

    optional<Venta> venta = archivo->buscar(numero);

    if (!venta.has_value())
    {
        return false;
    }

    return generador.generar(venta.value());
}