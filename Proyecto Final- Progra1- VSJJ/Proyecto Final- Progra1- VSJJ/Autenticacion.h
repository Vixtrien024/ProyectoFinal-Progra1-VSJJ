
#pragma once

#include <string>
#include "Usuario.h"
#include "GestorUsuarios.h"

using namespace std;

// Permisos disponibles en el sistema
enum class Permiso {
    GestionarUsuarios,
    GestionarProductos,
    RegistrarVenta,
    ConsultarHistorial,
    GenerarTickets
};

class Autenticacion {
private:
    // Gestor donde se encuentran los usuarios
    GestorUsuarios* gestor;

    // ID del usuario que inició sesión
    // -1 significa que no hay sesión activa
    int idSesion;

public:
    // Constructor
    Autenticacion(GestorUsuarios* gestor);

    // Iniciar sesión
    bool iniciarSesion(string nombreUsuario, string clave);

    // Cerrar sesión
    void cerrarSesion();

    // Comprobar si existe una sesión activa
    bool haySesionActiva();

    // Obtener el usuario conectado
    Usuario* getUsuarioActual();

    // Consultar el rol del usuario conectado
    Rol getRolActual();

    // Comprobar si tiene un permiso
    bool tienePermiso(Permiso permiso);
};
