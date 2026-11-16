
#include "GestorVentas.h"

#include <ctime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

using namespace std;

// Devuelve la fecha y hora actual
static string fechaActual()
{
    time_t ahora = time(nullptr);
    tm hora{};

    localtime_s(&hora, &ahora);

    char texto[30];

    strftime(
        texto,
        sizeof(texto),
        "%Y-%m-%d %H:%M:%S",
        &hora
    );

    return string(texto);
}

// Constructor
GestorVentas::GestorVentas()
    : archivoVentas("ventas.json")
{
    cargarVentas();
}

// Cargar las ventas guardadas en el archivo JSON
void GestorVentas::cargarVentas()
{
    ventas.clear();

    bool correcto = false;

    QJsonArray datos = archivoVentas.cargarVentas(&correcto);

    if (!correcto)
    {
        return;
    }

    for (const QJsonValue& valor : datos)
    {
        if (valor.isObject())
        {
            Venta venta = Venta::desdeJson(
                valor.toObject()
            );

            ventas.push_back(venta);
        }
    }
}

// Guardar todas las ventas en el archivo JSON
bool GestorVentas::guardarVentas()
{
    QJsonArray datos;

    for (const Venta& venta : ventas)
    {
        datos.append(venta.aJson());
    }

    return archivoVentas.guardarVentas(datos);
}

// Validar el carrito y el efectivo recibido
bool GestorVentas::validar(
    Carrito carrito,
    qint64 efectivo
)
{
    if (carrito.detalles().empty())
    {
        return false;
    }

    if (efectivo < carrito.total())
    {
        return false;
    }

    return true;
}

// Confirmar y guardar una venta
ResultadoVenta GestorVentas::confirmar(
    Carrito carrito,
    qint64 efectivo
)
{
    // Verificar que el carrito tenga productos
    if (carrito.detalles().empty())
    {
        return ResultadoVenta(
            false,
            "El carrito esta vacio",
            Venta()
        );
    }

    // Verificar que el efectivo alcance
    if (efectivo < carrito.total())
    {
        return ResultadoVenta(
            false,
            "El efectivo no alcanza para pagar",
            Venta()
        );
    }

    // Crear la nueva venta
    Venta nueva(
        siguienteNumero(),
        fechaActual(),
        carrito.detalles(),
        efectivo
    );

    // Agregar temporalmente la venta
    ventas.push_back(nueva);

    // Guardar en ventas.json
    if (!guardarVentas())
    {
        // Si falla el archivo, revertir el registro
        ventas.pop_back();

        return ResultadoVenta(
            false,
            "No se pudo guardar la venta en el archivo",
            Venta()
        );
    }

    return ResultadoVenta(
        true,
        "Venta realizada y guardada correctamente",
        nueva
    );
}

// Obtener el numero de la siguiente venta
int GestorVentas::siguienteNumero()
{
    int mayorNumero = 0;

    for (const Venta& venta : ventas)
    {
        if (venta.getNumero() > mayorNumero)
        {
            mayorNumero = venta.getNumero();
        }
    }

    return mayorNumero + 1;
}

// Devolver todas las ventas registradas
vector<Venta> GestorVentas::listarVentas() const
{
    return ventas;
}
