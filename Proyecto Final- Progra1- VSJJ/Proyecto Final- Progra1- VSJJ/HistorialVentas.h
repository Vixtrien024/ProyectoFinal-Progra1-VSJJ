#pragma once
#include <vector>
#include <optional>
#include "Venta.h"
#include "ArchivoVentas.h"

using namespace std;

class GeneradorTicket;

class HistorialVentas
{
private:
    ArchivoVentas* archivo;

public:
    explicit HistorialVentas(ArchivoVentas* archivoVentas);

    vector<Venta> listar();

    optional<Venta> buscar(int numero);

    bool generarTicket(int numero, GeneradorTicket& generador);
};