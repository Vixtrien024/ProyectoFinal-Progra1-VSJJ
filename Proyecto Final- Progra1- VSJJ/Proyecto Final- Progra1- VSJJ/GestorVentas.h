#pragma once
#include <string>
#include <vector>
#include <QtGlobal>
#include "Carrito.h"
#include "Venta.h"
#include "ResultadoVenta.h"
using namespace std;

// Se encarga de validar, cobrar y guardar las ventas en memoria.
class GestorVentas {
private:
    vector<Venta> ventas;

    // pendiente: Inventario* inventario;   (modulo de Julian)
    // pendiente: ArchivoVentas* archivo;   (modulo de Moises)

public:
    // true si el carrito tiene productos y el efectivo alcanza
    bool validar(Carrito carrito, qint64 efectivo);

    // Cobra la venta y devuelve el resultado (exitosa o rechazada)
    ResultadoVenta confirmar(Carrito carrito, qint64 efectivo);

    // Numero que le toca a la proxima venta
    int siguienteNumero();
};
