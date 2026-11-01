#include "GestorVentas.h"
#include <ctime>
#include <QJsonArray>

static string fechaActual() {
    time_t ahora = time(nullptr);
    tm local{};
    localtime_s(&local, &ahora);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local);
    return buf;
}

GestorVentas::GestorVentas(IInventario& inventario, ArchivoVentas& archivo)
    : inventario(inventario), archivo(archivo), siguienteNumero(1) {
}

bool GestorVentas::cargar() {
    bool ok = false;
    QJsonArray arr = archivo.cargarVentas(&ok);
    ventas.clear();
    siguienteNumero = 1;
    for (const QJsonValue& v : arr) {
        Venta venta = Venta::desdeJson(v.toObject());
        if (venta.getNumero() >= siguienteNumero)
            siguienteNumero = venta.getNumero() + 1;
        ventas.push_back(venta);
    }
    return ok;
}

ResultadoVenta GestorVentas::procesarVenta(const Carrito& carrito,
    qint64 efectivo, const string& cajero, Venta* ventaGenerada) {
    if (carrito.estaVacio()) return ResultadoVenta::CarritoVacio;

    for (const DetalleVenta& d : carrito.getArticulos()) {
        if (inventario.consultarExistencias(d.getIdProducto()) < d.getCantidad())
            return ResultadoVenta::SinExistencias;
    }
    if (efectivo < carrito.calcularTotal())
        return ResultadoVenta::PagoInsuficiente;

    Venta venta(siguienteNumero, fechaActual(), cajero,
        carrito.getArticulos(), efectivo);

    // Se guarda primero (con la venta incluida); si falla, no se descuenta nada.
    QJsonArray arr;
    for (const Venta& v : ventas) arr.append(v.aJson());
    arr.append(venta.aJson());
    if (!archivo.guardarVentas(arr)) return ResultadoVenta::ErrorGuardado;

    for (const DetalleVenta& d : venta.getDetalles())
        inventario.descontarExistencias(d.getIdProducto(), d.getCantidad());

    ventas.push_back(venta);
    siguienteNumero++;
    if (ventaGenerada) *ventaGenerada = venta;
    return ResultadoVenta::Exitosa;
}

const vector<Venta>& GestorVentas::getVentas() const { return ventas; }

string GestorVentas::mensaje(ResultadoVenta r) {
    switch (r) {
    case ResultadoVenta::Exitosa: return "Venta realizada";
    case ResultadoVenta::CarritoVacio: return "El carrito esta vacio";
    case ResultadoVenta::SinExistencias: return "No hay existencias suficientes";
    case ResultadoVenta::PagoInsuficiente: return "El efectivo no cubre el total";
    case ResultadoVenta::ErrorGuardado: return "No se pudo guardar la venta";
    }
    return "";
}
