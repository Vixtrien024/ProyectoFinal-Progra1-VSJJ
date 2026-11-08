
#pragma once

#include <string>
using namespace std;

// Roles disponibles en el sistema
enum class Rol {
    Administrador,
    Cajero
};

class Usuario {
private:
    int id;
    string nombre;
    string nombreUsuario;
    string claveHash;
    Rol rol;
    bool activo;

public:
    // Constructores
    Usuario();

    Usuario(int id, string nombre, string nombreUsuario,
        string claveHash, Rol rol, bool activo);

    // Getters
    int getId() const;
    string getNombre() const;
    string getNombreUsuario() const;
    string getClaveHash() const;
    Rol getRol() const;
    bool estaActivo() const;

    // Setters
    void setId(int id);
    void setNombre(string nombre);
    void setNombreUsuario(string nombreUsuario);
    void setClaveHash(string claveHash);
    void setRol(Rol rol);
    void setActivo(bool activo);
};
