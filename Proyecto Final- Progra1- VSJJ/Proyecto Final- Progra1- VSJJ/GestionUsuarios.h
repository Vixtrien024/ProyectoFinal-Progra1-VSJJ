
#pragma once

#include <QDialog>
#include <string>
#include <vector>

#include "Usuario.h"
#include "GestorUsuarios.h"
#include "Autenticacion.h"
#include "ui_GestionUsuarios.h"

using namespace std;

class GestionUsuarios : public QDialog
{
    Q_OBJECT

private:
    // Interfaz grafica
    Ui::GestionUsuarios ui;

    // Clases para manejar usuarios y permisos
    GestorUsuarios* gestor;
    Autenticacion* autenticacion;

    // ID del usuario seleccionado
    int idSeleccionado;

public:
    // Constructor
    GestionUsuarios(
        GestorUsuarios* gestor,
        Autenticacion* autenticacion,
        QWidget* parent = nullptr
    );

    // Destructor
    ~GestionUsuarios();

private slots:

    // Registrar un usuario nuevo
    void agregarUsuario();

    // Modificar un usuario existente
    void modificarUsuario();

    // Desactivar un usuario
    void desactivarUsuario();

    // Seleccionar un usuario de la tabla
    void seleccionarUsuario(int fila, int columna);

    // Limpiar los campos
    void limpiarFormulario();

    // Cerrar la ventana
    void cerrarVentana();

private:

    // Mostrar los usuarios en la tabla
    void cargarTabla();

    // Validar los datos del formulario
    bool validarCampos();

    // Comprobar permisos de administrador
    bool verificarPermisos();

    // Obtener el rol seleccionado
    Rol obtenerRolSeleccionado();

    // Mostrar los datos de un usuario
    void mostrarDatosUsuario(Usuario* usuario);
};
