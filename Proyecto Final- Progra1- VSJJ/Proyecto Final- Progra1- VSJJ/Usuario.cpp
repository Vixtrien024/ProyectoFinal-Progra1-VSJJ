
#include "Usuario.h"

using namespace std;

// Constructor vacío
Usuario::Usuario() {
    id = 0;
    nombre = "";
    nombreUsuario = "";
    claveHash = "";
    rol = Rol::Cajero;
    activo = true;
}

// Constructor con parámetros
Usuario::Usuario(int id, string nombre, string nombreUsuario,
    string claveHash, Rol rol, bool activo) {
    this->id = id;
    this->nombre = nombre;
    this->nombreUsuario = nombreUsuario;
    this->claveHash = claveHash;
    this->rol = rol;
    this->activo = activo;
}

// Getters

int Usuario::getId() const {
    return id;
}

string Usuario::getNombre() const {
    return nombre;
}

string Usuario::getNombreUsuario() const {
    return nombreUsuario;
}

string Usuario::getClaveHash() const {
    return claveHash;
}

Rol Usuario::getRol() const {
    return rol;
}

bool Usuario::estaActivo() const {
    return activo;
}

// Setters

void Usuario::setId(int id) {
    this->id = id;
}

void Usuario::setNombre(string nombre) {
    this->nombre = nombre;
}

void Usuario::setNombreUsuario(string nombreUsuario) {
    this->nombreUsuario = nombreUsuario;
}

void Usuario::setClaveHash(string claveHash) {
    this->claveHash = claveHash;
}

void Usuario::setRol(Rol rol) {
    this->rol = rol;
}

void Usuario::setActivo(bool activo) {
    this->activo = activo;
}
