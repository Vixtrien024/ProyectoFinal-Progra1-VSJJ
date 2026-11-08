#include "GestorVentas.h"
#include <ctime>

// Devuelve la fecha y hora de ahora, por ejemplo "2026-11-02 14:30:00"
static string fechaActual() {
    time_t ahora = time(nullptr);
    tm hora;
    localtime_s(&hora, &ahora);

    char texto[30];
    strftime(texto, 30, "%Y-%m-%d %H:%M:%S", &hora);
    return texto;
}

bool GestorVentas::validar(Carrito carrito, qint64 efectivo) {
    if (carrito.detalles().empty()) {
        return false;
    }
    if (efectivo < carrito.total()) {
        return false;
    }

    // pendiente: revisar existencias con inventario->hayExistencias(...)
    return true;
}

ResultadoVenta GestorVentas::confirmar(Carrito carrito, qint64 efectivo) {
    if (!validar(carrito, efectivo)) {
        // Se busca el motivo del rechazo para el mensaje
        if (carrito.detalles().empty()) {
            return ResultadoVenta(false, "El carrito esta vacio", Venta());
        }
        return ResultadoVenta(false, "El efectivo no alcanza para pagar", Venta());
    }

    Venta nueva(siguienteNumero(), fechaActual(), carrito.detalles(), efectivo);
    ventas.push_back(nueva);

    // pendiente: descontar existencias con inventario->descontar(...)
    // pendiente: guardar la venta con archivo->guardarVenta(nueva)

    return ResultadoVenta(true, "Venta realizada", nueva);
}

int GestorVentas::siguienteNumero() {
    return (int)ventas.size() + 1;
}
