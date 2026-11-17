
#pragma once

#include <string>
#include <vector>

#include "Usuario.h"
#include "ArchivoUsuarios.h"
#include "GestorActividades.h"

using namespace std;

// Declaracion anticipada
class Autenticacion;

// ==========================================
// GESTOR DE USUARIOS - LA PULPE
// ==========================================

class GestorUsuarios
{
private:

    // Lista de usuarios
    vector<Usuario> usuarios;

    // Archivo JSON de usuarios
    ArchivoUsuarios* archivo;

    // Registro de actividades
    GestorActividades* gestorActividades;

    // Sistema de autenticacion
    Autenticacion* autenticacion;

    // Registrar movimientos de usuarios
    void registrarActividadUsuario(
        string tipo,
        string descripcion
    );

    // Contar administradores activos
    int contarAdministradoresActivos() const;

public:

    // Constructor
    GestorUsuarios(ArchivoUsuarios* archivo);

    // Conectar registro de actividades
    void setGestorActividades(
        GestorActividades* gestorActividades
    );

    // Conectar autenticacion
    void setAutenticacion(
        Autenticacion* autenticacion
    );

    // ======================================
    // ADMINISTRACION DE USUARIOS
    // ======================================

    // Agregar usuario
    bool agregarUsuario(Usuario usuario);

    // Buscar por ID
    Usuario* buscarPorId(int id);

    // Buscar por nombre
    Usuario* buscarPorNombre(
        string nombreUsuario
    );

    // Modificar usuario
    bool modificarUsuario(Usuario usuario);

    // Desactivar usuario
    bool desactivarUsuario(int id);

    // Activar usuario
    bool activarUsuario(int id);

    // Eliminar usuario definitivamente
    bool eliminarUsuario(int id);

    // Listar usuarios
    vector<Usuario> listarUsuarios() const;

    // ======================================
    // ARCHIVOS JSON
    // ======================================

    bool guardarUsuarios();

    bool cargarUsuarios();

    // ======================================
    // IDENTIFICADORES
    // ======================================

    int generarId() const;
};
