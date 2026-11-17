
#include "Actividad.h"

using namespace std;

// ==========================================
// CONSTRUCTOR VACIO
// ==========================================

Actividad::Actividad() {
    id = 0;
    idUsuario = 0;
    nombreUsuario = "";
    tipo = "";
    descripcion = "";
    fechaHora = "";
}

// ==========================================
// CONSTRUCTOR CON PARAMETROS
// ==========================================

Actividad::Actividad(
    int id,
    int idUsuario,
    string nombreUsuario,
    string tipo,
    string descripcion,
    string fechaHora
) {
    this->id = id;
    this->idUsuario = idUsuario;
    this->nombreUsuario = nombreUsuario;
    this->tipo = tipo;
    this->descripcion = descripcion;
    this->fechaHora = fechaHora;
}

// ==========================================
// GETTERS
// ==========================================

int Actividad::getId() const {
    return id;
}

int Actividad::getIdUsuario() const {
    return idUsuario;
}

string Actividad::getNombreUsuario() const {
    return nombreUsuario;
}

string Actividad::getTipo() const {
    return tipo;
}

string Actividad::getDescripcion() const {
    return descripcion;
}

string Actividad::getFechaHora() const {
    return fechaHora;
}

// ==========================================
// SETTERS
// ==========================================

void Actividad::setId(int id) {
    this->id = id;
}

void Actividad::setIdUsuario(int idUsuario) {
    this->idUsuario = idUsuario;
}

void Actividad::setNombreUsuario(string nombreUsuario) {
    this->nombreUsuario = nombreUsuario;
}

void Actividad::setTipo(string tipo) {
    this->tipo = tipo;
}

void Actividad::setDescripcion(string descripcion) {
    this->descripcion = descripcion;
}

void Actividad::setFechaHora(string fechaHora) {
    this->fechaHora = fechaHora;
}
