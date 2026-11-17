
#pragma once

#include <string>

#include "Usuario.h"
#include "GestorUsuarios.h"
#include "GestorActividades.h"

using namespace std;

// ==========================================
// PERMISOS DISPONIBLES EN EL SISTEMA
// ==========================================

enum class Permiso {
    GestionarUsuarios,
    GestionarProductos,
    RegistrarVenta,
    ConsultarHistorial,
    GenerarTickets
};

// ==========================================
// CLASE AUTENTICACION
// ==========================================

class Autenticacion {
private:

    // Gestor donde se encuentran los usuarios
    GestorUsuarios* gestor;

    // Gestor encargado del registro de actividades
    GestorActividades* gestorActividades;

    // ID del usuario que inicio sesion
    // -1 significa que no hay sesion activa
    int idSesion;

public:

    // Constructor
    Autenticacion(GestorUsuarios* gestor);

    // Conectar el gestor de actividades
    void setGestorActividades(
        GestorActividades* gestorActividades
    );

    // Iniciar sesion
    bool iniciarSesion(
        string nombreUsuario,
        string clave
    );

    // Cerrar sesion
    void cerrarSesion();

    // Comprobar si existe una sesion activa
    bool haySesionActiva();

    // Obtener el usuario conectado
    Usuario* getUsuarioActual();

    // Consultar el rol del usuario conectado
    Rol getRolActual();

    // Comprobar si tiene un permiso
    bool tienePermiso(Permiso permiso);
};
