
#pragma once

#include <string>
#include <vector>
#include <QtGlobal>

#include "Carrito.h"
#include "Venta.h"
#include "ResultadoVenta.h"
#include "ArchivoVentas.h"
#include "Inventario.h"

using namespace std;

// Declaraciones anticipadas
class GestorActividades;
class Autenticacion;

// ==========================================
// GESTOR DE VENTAS - LA PULPE
// ==========================================

// Se encarga de validar, cobrar y guardar
// las ventas realizadas en el sistema.

class GestorVentas
{
private:

    // ======================================
    // DATOS DE VENTAS
    // ======================================

    // Ventas registradas en el sistema
    vector<Venta> ventas;

    // Archivo donde se guardan las ventas
    ArchivoVentas archivoVentas;

    // Inventario compartido con productos
    Inventario* inventario = nullptr;

    // ======================================
    // REGISTRO DE ACTIVIDADES
    // ======================================

    // Gestor compartido con toda la aplicacion
    GestorActividades* gestorActividades = nullptr;

    // Permite identificar al usuario conectado
    Autenticacion* autenticacion = nullptr;

    // Registrar una actividad de ventas
    void registrarActividadVenta(
        const string& tipo,
        const string& descripcion
    );

    // ======================================
    // ARCHIVOS
    // ======================================

    // Cargar ventas guardadas en JSON
    void cargarVentas();

    // Guardar ventas en JSON
    bool guardarVentas();

public:

    // ======================================
    // CONSTRUCTOR
    // ======================================

    GestorVentas();

    // ======================================
    // CONEXIONES
    // ======================================

    // Asignar inventario compartido
    void setInventario(
        Inventario* inventarioActual
    );

    // Conectar registro de actividades
    void setGestorActividades(
        GestorActividades* gestorActividades
    );

    // Conectar autenticacion
    void setAutenticacion(
        Autenticacion* autenticacion
    );

    // ======================================
    // OPERACIONES DE VENTAS
    // ======================================

    // Verificar carrito y efectivo
    bool validar(
        Carrito carrito,
        qint64 efectivo
    );

    // Confirmar venta y devolver resultado
    ResultadoVenta confirmar(
        Carrito carrito,
        qint64 efectivo
    );

    // Obtener siguiente numero de venta
    int siguienteNumero();

    // Obtener ventas registradas
    vector<Venta> listarVentas() const;
};
