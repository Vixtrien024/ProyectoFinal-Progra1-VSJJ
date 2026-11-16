
#pragma once

#include <string>
#include <vector>
#include <QtGlobal>

#include "Carrito.h"
#include "Venta.h"
#include "ResultadoVenta.h"
#include "ArchivoVentas.h"

using namespace std;

// Se encarga de validar, cobrar y guardar las ventas.
class GestorVentas
{
private:

    // Ventas registradas en el sistema
    vector<Venta> ventas;

    // Archivo donde se guardan las ventas
    ArchivoVentas archivoVentas;

    // Cargar ventas guardadas en el archivo JSON
    void cargarVentas();

    // Guardar las ventas en el archivo JSON
    bool guardarVentas();

public:

    // Constructor
    GestorVentas();

    // Verifica que el carrito tenga productos
    // y que el efectivo sea suficiente.
    bool validar(Carrito carrito, qint64 efectivo);

    // Confirma una venta y devuelve su resultado.
    ResultadoVenta confirmar(Carrito carrito, qint64 efectivo);

    // Obtiene el numero de la siguiente venta.
    int siguienteNumero();

    // Devuelve las ventas registradas.
    vector<Venta> listarVentas() const;
};
