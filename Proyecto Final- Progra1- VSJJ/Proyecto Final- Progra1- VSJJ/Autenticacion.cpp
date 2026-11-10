
#include "Autenticacion.h"
#include "Seguridad.h"

using namespace std;

// Constructor
Autenticacion::Autenticacion(GestorUsuarios* gestor) {
    this->gestor = gestor;
    idSesion = -1;
}

// Iniciar sesion
bool Autenticacion::iniciarSesion(string nombreUsuario,
    string clave) {

    // Cerrar cualquier sesion anterior
    cerrarSesion();

    if (gestor == nullptr) {
        return false;
    }

    // Validar campos vacios
    if (nombreUsuario.empty() || clave.empty()) {
        return false;
    }

    // Buscar usuario registrado
    Usuario* usuario = gestor->buscarPorNombre(nombreUsuario);

    if (usuario == nullptr) {
        return false;
    }

    // Verificar que este activo
    if (!usuario->estaActivo()) {
        return false;
    }

    // Verificar la clave contra el hash almacenado
    if (!Seguridad::verificarClave(
        clave, usuario->getClaveHash())) {
        return false;
    }

    // Registrar la sesion activa
    idSesion = usuario->getId();

    return true;
}

// Cerrar sesion
void Autenticacion::cerrarSesion() {
    idSesion = -1;
}

// Comprobar si hay una sesion activa
bool Autenticacion::haySesionActiva() {

    return getUsuarioActual() != nullptr;
}

// Obtener el usuario conectado
Usuario* Autenticacion::getUsuarioActual() {

    if (gestor == nullptr || idSesion == -1) {
        return nullptr;
    }

    Usuario* usuario = gestor->buscarPorId(idSesion);

    if (usuario == nullptr || !usuario->estaActivo()) {
        idSesion = -1;
        return nullptr;
    }

    return usuario;
}

// Obtener el rol del usuario conectado
Rol Autenticacion::getRolActual() {

    Usuario* usuario = getUsuarioActual();

    if (usuario == nullptr) {
        return Rol::Cajero;
    }

    return usuario->getRol();
}

// Comprobar permisos
bool Autenticacion::tienePermiso(Permiso permiso) {

    Usuario* usuario = getUsuarioActual();

    if (usuario == nullptr) {
        return false;
    }

    // El administrador tiene todos los permisos
    if (usuario->getRol() == Rol::Administrador) {
        return true;
    }

    // Permisos del cajero
    if (usuario->getRol() == Rol::Cajero) {

        switch (permiso) {

        case Permiso::RegistrarVenta:
            return true;

        case Permiso::ConsultarHistorial:
            return true;

        case Permiso::GenerarTickets:
            return true;

        case Permiso::GestionarUsuarios:
            return false;

        case Permiso::GestionarProductos:
            return false;
        }
    }

    return false;
}

