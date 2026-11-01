#pragma once

#include <string>
#include <vector>
#include <QtGlobal>
#include "Carrito.h"
#include "Venta.h"
#include "IInventario.h"
#include "ArchivoVentas.h"

using namespace std;

// Resultado de intentar cobrar: la venta solo existe si fue exitosa.
enum class ResultadoVenta {
    Exitosa,
    CarritoVacio,
    SinExistencias,
    PagoInsuficiente,
    ErrorGuardado
};

// Coordina el cobro: valida, guarda, descuenta inventario y registra.
class GestorVentas {
private:
    IInventario& inventario;
    ArchivoVentas& archivo;
    vector<Venta> ventas;
    int siguienteNumero;

public:
    GestorVentas(IInventario& inventario, ArchivoVentas& archivo);

    // Carga ventas guardadas; false si el archivo no existe o esta danado
    bool cargar();

    ResultadoVenta procesarVenta(const Carrito& carrito, qint64 efectivo,
        const string& cajero, Venta* ventaGenerada = nullptr);

    const vector<Venta>& getVentas() const;
    static string mensaje(ResultadoVenta resultado);
};
