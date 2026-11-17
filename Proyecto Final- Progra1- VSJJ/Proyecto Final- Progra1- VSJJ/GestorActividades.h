
#pragma once

#include <string>
#include <vector>

#include "Actividad.h"

using namespace std;

// ==========================================
// GESTOR DE ACTIVIDADES DE LA PULPE
// ==========================================

class GestorActividades
{
private:

    // Actividades del dia seleccionado
    vector<Actividad> actividades;

    // Carpeta donde se guardan los registros
    string carpetaRegistros;

    // Fecha del archivo cargado
    string fechaArchivoActual;

    // Obtener ruta del archivo de una fecha
    string obtenerRutaArchivo(
        const string& fecha
    ) const;

    // Obtener fecha actual
    string obtenerFechaActual() const;

    // Obtener fecha y hora actual
    string obtenerFechaHora() const;

    // Obtener el siguiente ID
    int siguienteId() const;

    // Crear carpeta si no existe
    bool crearCarpeta() const;

    // Cargar registros de una fecha
    bool cargarActividades(
        const string& fecha
    );

    // Guardar registros de una fecha
    bool guardarActividades() const;

public:

    // Constructor
    GestorActividades();

    // Registrar nueva actividad
    bool registrarActividad(
        int idUsuario,
        string nombreUsuario,
        string tipo,
        string descripcion
    );

    // Listar actividades cargadas
    vector<Actividad> listarActividades() const;

    // Buscar actividades por usuario
    vector<Actividad> buscarPorUsuario(
        int idUsuario
    ) const;

    // Buscar actividades por tipo
    vector<Actividad> buscarPorTipo(
        string tipo
    ) const;

    // Actualizar actividades del dia actual
    bool actualizarActividades();

    // Consultar actividades de una fecha
    vector<Actividad> listarPorFecha(
        string fecha
    );
};
