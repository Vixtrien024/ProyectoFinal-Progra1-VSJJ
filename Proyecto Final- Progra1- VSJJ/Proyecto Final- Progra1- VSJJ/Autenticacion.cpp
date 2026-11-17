
#include "Autenticacion.h"
#include "Seguridad.h"

using namespace std;

// ==========================================
// CONSTRUCTOR
// ==========================================

Autenticacion::Autenticacion(GestorUsuarios* gestor)
{
    this->gestor = gestor;
    this->gestorActividades = nullptr;
    this->idSesion = -1;
}

// ==========================================
// CONECTAR GESTOR DE ACTIVIDADES
// ==========================================

void Autenticacion::setGestorActividades(
    GestorActividades* gestorActividades)
{
    this->gestorActividades = gestorActividades;
}

// ==========================================
// INICIAR SESION
// ==========================================

bool Autenticacion::iniciarSesion(
    string nombreUsuario,
    string clave)
{
    // Cerrar una sesion anterior, si existe
    cerrarSesion();

    if (gestor == nullptr)
    {
        return false;
    }

    if (nombreUsuario.empty() || clave.empty())
    {
        return false;
    }

    // Buscar usuario
    Usuario* usuario =
        gestor->buscarPorNombre(nombreUsuario);

    if (usuario == nullptr)
    {
        return false;
    }

    // Comprobar que el usuario este activo
    if (!usuario->estaActivo())
    {
        return false;
    }

    // Verificar la contrasena
    if (!Seguridad::verificarClave(
        clave,
        usuario->getClaveHash()))
    {
        return false;
    }

    // Iniciar sesion
    idSesion = usuario->getId();

    // Registrar inicio de sesion exitoso
    if (gestorActividades != nullptr)
    {
        gestorActividades->registrarActividad(
            usuario->getId(),
            usuario->getNombreUsuario(),
            "LOGIN",
            "Inicio de sesion exitoso"
        );
    }

    return true;
}

// ==========================================
// CERRAR SESION
// ==========================================

void Autenticacion::cerrarSesion()
{
    // Solo registrar si existe una sesion
    if (idSesion != -1 && gestor != nullptr)
    {
        Usuario* usuario =
            gestor->buscarPorId(idSesion);

        if (usuario != nullptr &&
            gestorActividades != nullptr)
        {
            gestorActividades->registrarActividad(
                usuario->getId(),
                usuario->getNombreUsuario(),
                "LOGOUT",
                "Cierre de sesion"
            );
        }
    }

    // Finalizar sesion
    idSesion = -1;
}

// ==========================================
// COMPROBAR SESION ACTIVA
// ==========================================

bool Autenticacion::haySesionActiva()
{
    return getUsuarioActual() != nullptr;
}

// ==========================================
// OBTENER USUARIO ACTUAL
// ==========================================

Usuario* Autenticacion::getUsuarioActual()
{
    if (gestor == nullptr || idSesion == -1)
    {
        return nullptr;
    }

    Usuario* usuario =
        gestor->buscarPorId(idSesion);

    if (usuario == nullptr ||
        !usuario->estaActivo())
    {
        idSesion = -1;
        return nullptr;
    }

    return usuario;
}

// ==========================================
// OBTENER ROL ACTUAL
// ==========================================

Rol Autenticacion::getRolActual()
{
    Usuario* usuario = getUsuarioActual();

    if (usuario == nullptr)
    {
        return Rol::Cajero;
    }

    return usuario->getRol();
}

// ==========================================
// VERIFICAR PERMISOS
// ==========================================

bool Autenticacion::tienePermiso(Permiso permiso)
{
    Usuario* usuario = getUsuarioActual();

    if (usuario == nullptr)
    {
        return false;
    }

    // Administrador tiene todos los permisos
    if (usuario->getRol() == Rol::Administrador)
    {
        return true;
    }

    // Permisos del cajero
    if (usuario->getRol() == Rol::Cajero)
    {
        switch (permiso)
        {
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
