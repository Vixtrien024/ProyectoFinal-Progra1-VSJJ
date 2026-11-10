
#include "GestorUsuarios.h"

#include <algorithm>

using namespace std;

// Constructor
GestorUsuarios::GestorUsuarios(ArchivoUsuarios* archivo) {
    this->archivo = archivo;
}

// Agregar un usuario
bool GestorUsuarios::agregarUsuario(Usuario usuario) {

    if (archivo == nullptr) {
        return false;
    }

    if (usuario.getNombreUsuario().empty()) {
        return false;
    }

    if (buscarPorNombre(usuario.getNombreUsuario()) != nullptr) {
        return false;
    }

    if (usuario.getId() <= 0 ||
        buscarPorId(usuario.getId()) != nullptr) {

        usuario.setId(generarId());
    }

    usuarios.push_back(usuario);

    if (!guardarUsuarios()) {
        usuarios.pop_back();
        return false;
    }

    return true;
}

// Buscar un usuario por su ID
Usuario* GestorUsuarios::buscarPorId(int id) {

    for (Usuario& usuario : usuarios) {
        if (usuario.getId() == id) {
            return &usuario;
        }
    }

    return nullptr;
}

// Buscar un usuario por su nombre de usuario
Usuario* GestorUsuarios::buscarPorNombre(
    string nombreUsuario) {

    for (Usuario& usuario : usuarios) {
        if (usuario.getNombreUsuario() == nombreUsuario) {
            return &usuario;
        }
    }

    return nullptr;
}

// Modificar los datos de un usuario
bool GestorUsuarios::modificarUsuario(Usuario usuario) {

    if (archivo == nullptr) {
        return false;
    }

    Usuario* actual = buscarPorId(usuario.getId());

    if (actual == nullptr) {
        return false;
    }

    Usuario* mismoNombre =
        buscarPorNombre(usuario.getNombreUsuario());

    if (mismoNombre != nullptr &&
        mismoNombre->getId() != usuario.getId()) {
        return false;
    }

    Usuario anterior = *actual;
    *actual = usuario;

    if (!guardarUsuarios()) {
        *actual = anterior;
        return false;
    }

    return true;
}

// Desactivar un usuario
bool GestorUsuarios::desactivarUsuario(int id) {

    if (archivo == nullptr) {
        return false;
    }

    Usuario* usuario = buscarPorId(id);

    if (usuario == nullptr || !usuario->estaActivo()) {
        return false;
    }

    usuario->setActivo(false);

    if (!guardarUsuarios()) {
        usuario->setActivo(true);
        return false;
    }

    return true;
}

// Obtener todos los usuarios
vector<Usuario> GestorUsuarios::listarUsuarios() const {
    return usuarios;
}

// Guardar los usuarios en JSON
bool GestorUsuarios::guardarUsuarios() {

    if (archivo == nullptr) {
        return false;
    }

    return archivo->guardar(usuarios);
}

// Cargar los usuarios desde JSON
bool GestorUsuarios::cargarUsuarios() {

    if (archivo == nullptr) {
        return false;
    }

    usuarios = archivo->cargar();

    return true;
}

// Generar un ID para un usuario nuevo
int GestorUsuarios::generarId() const {

    int mayor = 0;

    for (const Usuario& usuario : usuarios) {
        mayor = max(mayor, usuario.getId());
    }

    return mayor + 1;
}
