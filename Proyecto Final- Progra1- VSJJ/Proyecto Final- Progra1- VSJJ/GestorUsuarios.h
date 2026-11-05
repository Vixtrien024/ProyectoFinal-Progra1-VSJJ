
#pragma once

#include <string>
#include <vector>
#include "Usuario.h"
#include "ArchivoUsuarios.h"

using namespace std;

class GestorUsuarios {
private:
    // Lista de usuarios registrados
    vector<Usuario> usuarios;

    // Archivo para guardar y cargar usuarios
    ArchivoUsuarios* archivo;

public:
    // Constructor
    GestorUsuarios(ArchivoUsuarios* archivo);

    // Agregar un usuario
    bool agregarUsuario(Usuario usuario);

    // Buscar un usuario por su ID
    Usuario* buscarPorId(int id);

    // Buscar un usuario por su nombre de usuario
    Usuario* buscarPorNombre(string nombreUsuario);

    // Modificar los datos de un usuario
    bool modificarUsuario(Usuario usuario);

    // Desactivar un usuario
    bool desactivarUsuario(int id);

    // Obtener todos los usuarios
    vector<Usuario> listarUsuarios() const;

    // Guardar los usuarios en JSON
    bool guardarUsuarios();

    // Cargar los usuarios desde JSON
    bool cargarUsuarios();

    // Generar un ID para un usuario nuevo
    int generarId() const;
};
