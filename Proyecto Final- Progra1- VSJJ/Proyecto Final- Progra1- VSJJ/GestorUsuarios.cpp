
#include "GestorUsuarios.h"
#include "Autenticacion.h"

#include <algorithm>

using namespace std;

// ==========================================
// CONSTRUCTOR
// ==========================================

GestorUsuarios::GestorUsuarios(ArchivoUsuarios* archivo)
{
    this->archivo = archivo;
    this->gestorActividades = nullptr;
    this->autenticacion = nullptr;
}

// ==========================================
// CONECTAR REGISTRO DE ACTIVIDADES
// ==========================================

void GestorUsuarios::setGestorActividades(
    GestorActividades* gestorActividades)
{
    this->gestorActividades = gestorActividades;
}

// ==========================================
// CONECTAR AUTENTICACION
// ==========================================

void GestorUsuarios::setAutenticacion(
    Autenticacion* autenticacion)
{
    this->autenticacion = autenticacion;
}

// ==========================================
// REGISTRAR ACTIVIDAD DE USUARIOS
// ==========================================

void GestorUsuarios::registrarActividadUsuario(
    string tipo,
    string descripcion)
{
    if (gestorActividades == nullptr ||
        autenticacion == nullptr)
    {
        return;
    }

    Usuario* usuarioActual =
        autenticacion->getUsuarioActual();

    if (usuarioActual == nullptr)
    {
        return;
    }

    gestorActividades->registrarActividad(
        usuarioActual->getId(),
        usuarioActual->getNombreUsuario(),
        tipo,
        descripcion
    );
}

// ==========================================
// CONTAR ADMINISTRADORES ACTIVOS
// ==========================================

int GestorUsuarios::contarAdministradoresActivos() const
{
    int cantidad = 0;

    for (const Usuario& usuario : usuarios)
    {
        if (usuario.getRol() == Rol::Administrador &&
            usuario.estaActivo())
        {
            cantidad++;
        }
    }

    return cantidad;
}

// ==========================================
// AGREGAR USUARIO
// ==========================================

bool GestorUsuarios::agregarUsuario(Usuario usuario)
{
    if (archivo == nullptr)
    {
        return false;
    }

    if (usuario.getNombreUsuario().empty())
    {
        return false;
    }

    if (buscarPorNombre(
        usuario.getNombreUsuario()) != nullptr)
    {
        return false;
    }

    if (usuario.getId() <= 0 ||
        buscarPorId(usuario.getId()) != nullptr)
    {
        usuario.setId(generarId());
    }

    usuarios.push_back(usuario);

    if (!guardarUsuarios())
    {
        usuarios.pop_back();
        return false;
    }

    registrarActividadUsuario(
        "USUARIO_CREADO",
        "Se creo el usuario: " +
        usuario.getNombreUsuario()
    );

    return true;
}

// ==========================================
// BUSCAR USUARIO POR ID
// ==========================================

Usuario* GestorUsuarios::buscarPorId(int id)
{
    for (Usuario& usuario : usuarios)
    {
        if (usuario.getId() == id)
        {
            return &usuario;
        }
    }

    return nullptr;
}

// ==========================================
// BUSCAR USUARIO POR NOMBRE
// ==========================================

Usuario* GestorUsuarios::buscarPorNombre(
    string nombreUsuario)
{
    for (Usuario& usuario : usuarios)
    {
        if (usuario.getNombreUsuario() ==
            nombreUsuario)
        {
            return &usuario;
        }
    }

    return nullptr;
}

// ==========================================
// MODIFICAR USUARIO
// ==========================================

bool GestorUsuarios::modificarUsuario(Usuario usuario)
{
    if (archivo == nullptr)
    {
        return false;
    }

    Usuario* actual = buscarPorId(usuario.getId());

    if (actual == nullptr)
    {
        return false;
    }

    Usuario* mismoNombre =
        buscarPorNombre(usuario.getNombreUsuario());

    if (mismoNombre != nullptr &&
        mismoNombre->getId() != usuario.getId())
    {
        return false;
    }

    // No permitir quitar el ultimo administrador activo
    if (actual->getRol() == Rol::Administrador &&
        actual->estaActivo() &&
        (usuario.getRol() != Rol::Administrador ||
         !usuario.estaActivo()) &&
        contarAdministradoresActivos() <= 1)
    {
        return false;
    }

    // No permitir desactivar la propia sesion
    if (autenticacion != nullptr)
    {
        Usuario* sesion = autenticacion->getUsuarioActual();

        if (sesion != nullptr &&
            sesion->getId() == usuario.getId() &&
            !usuario.estaActivo())
        {
            return false;
        }
    }

    Usuario anterior = *actual;
    *actual = usuario;

    if (!guardarUsuarios())
    {
        *actual = anterior;
        return false;
    }

    registrarActividadUsuario(
        "USUARIO_MODIFICADO",
        "Se modifico el usuario: " +
        usuario.getNombreUsuario()
    );

    return true;
}

// ==========================================
// DESACTIVAR USUARIO
// ==========================================

bool GestorUsuarios::desactivarUsuario(int id)
{
    if (archivo == nullptr)
    {
        return false;
    }

    Usuario* usuario = buscarPorId(id);

    if (usuario == nullptr || !usuario->estaActivo())
    {
        return false;
    }

    // Impedir desactivar la cuenta conectada
    if (autenticacion != nullptr)
    {
        Usuario* sesion = autenticacion->getUsuarioActual();

        if (sesion != nullptr && sesion->getId() == id)
        {
            return false;
        }
    }

    // Proteger al ultimo administrador activo
    if (usuario->getRol() == Rol::Administrador &&
        contarAdministradoresActivos() <= 1)
    {
        return false;
    }

    string nombreUsuario = usuario->getNombreUsuario();

    usuario->setActivo(false);

    if (!guardarUsuarios())
    {
        usuario->setActivo(true);
        return false;
    }

    registrarActividadUsuario(
        "USUARIO_DESACTIVADO",
        "Se desactivo el usuario: " + nombreUsuario
    );

    return true;
}

// ==========================================
// ACTIVAR USUARIO
// ==========================================

bool GestorUsuarios::activarUsuario(int id)
{
    if (archivo == nullptr)
    {
        return false;
    }

    Usuario* usuario = buscarPorId(id);

    // Solo se pueden activar usuarios inactivos
    if (usuario == nullptr || usuario->estaActivo())
    {
        return false;
    }

    string nombreUsuario = usuario->getNombreUsuario();

    usuario->setActivo(true);

    // Revertir si falla el guardado
    if (!guardarUsuarios())
    {
        usuario->setActivo(false);
        return false;
    }

    registrarActividadUsuario(
        "USUARIO_ACTIVADO",
        "Se activo el usuario: " + nombreUsuario
    );

    return true;
}

// ==========================================
// ELIMINAR USUARIO
// ==========================================

bool GestorUsuarios::eliminarUsuario(int id)
{
    if (archivo == nullptr)
    {
        return false;
    }

    Usuario* usuario = buscarPorId(id);

    if (usuario == nullptr)
    {
        return false;
    }

    // No permitir eliminar la cuenta conectada
    if (autenticacion != nullptr)
    {
        Usuario* sesion = autenticacion->getUsuarioActual();

        if (sesion != nullptr && sesion->getId() == id)
        {
            return false;
        }
    }

    // Proteger al ultimo administrador activo
    if (usuario->getRol() == Rol::Administrador &&
        usuario->estaActivo() &&
        contarAdministradoresActivos() <= 1)
    {
        return false;
    }

    string nombreUsuario = usuario->getNombreUsuario();

    // Copia de seguridad en memoria
    vector<Usuario> usuariosAnteriores = usuarios;

    // Eliminar usuario de la lista
    usuarios.erase(
        remove_if(
            usuarios.begin(),
            usuarios.end(),
            [id](const Usuario& u)
            {
                return u.getId() == id;
            }
        ),
        usuarios.end()
    );

    // Guardar cambios en JSON
    if (!guardarUsuarios())
    {
        usuarios = usuariosAnteriores;
        return false;
    }

    registrarActividadUsuario(
        "USUARIO_ELIMINADO",
        "Se elimino el usuario: " + nombreUsuario
    );

    return true;
}

// ==========================================
// LISTAR USUARIOS
// ==========================================

vector<Usuario> GestorUsuarios::listarUsuarios() const
{
    return usuarios;
}

// ==========================================
// GUARDAR USUARIOS EN JSON
// ==========================================

bool GestorUsuarios::guardarUsuarios()
{
    if (archivo == nullptr)
    {
        return false;
    }

    return archivo->guardar(usuarios);
}

// ==========================================
// CARGAR USUARIOS DESDE JSON
// ==========================================

bool GestorUsuarios::cargarUsuarios()
{
    if (archivo == nullptr)
    {
        return false;
    }

    usuarios = archivo->cargar();

    return true;
}

// ==========================================
// GENERAR ID DE USUARIO
// ==========================================

int GestorUsuarios::generarId() const
{
    int mayor = 0;

    for (const Usuario& usuario : usuarios)
    {
        mayor = max(mayor, usuario.getId());
    }

    return mayor + 1;
}
