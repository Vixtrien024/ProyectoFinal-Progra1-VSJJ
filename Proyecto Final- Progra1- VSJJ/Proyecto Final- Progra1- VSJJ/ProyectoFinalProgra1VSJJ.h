
#pragma once

#include <QtWidgets/QMainWindow>
#include <string>

#include "ui_ProyectoFinalProgra1VSJJ.h"
#include "Autenticacion.h"
#include "GestorUsuarios.h"

using namespace std;

class ProyectoFinalProgra1VSJJ : public QMainWindow
{
    Q_OBJECT

private:

    // Interfaz grafica
    Ui::ProyectoFinalProgra1VSJJClass ui;

    // Sistema de autenticacion
    Autenticacion* autenticacion;

    // Gestor de usuarios
    GestorUsuarios* gestorUsuarios;

public:

    // Constructor
    ProyectoFinalProgra1VSJJ(
        Autenticacion* autenticacion,
        GestorUsuarios* gestorUsuarios,
        QWidget* parent = nullptr
    );

    // Destructor
    ~ProyectoFinalProgra1VSJJ();

private slots:

    // Abrir modulo de productos
    void abrirProductos();

    // Abrir modulo de ventas
    void abrirVentas();

    // Abrir historial y tickets
    void abrirHistorial();

    // Abrir gestion de usuarios
    void abrirUsuarios();

    // Cerrar sesion
    void cerrarSesion();

private:

    // Mostrar informacion del usuario conectado
    void mostrarUsuarioActual();

    // Habilitar botones segun permisos
    void configurarPermisos();

    // Verificar acceso a un modulo
    bool verificarAcceso(Permiso permiso);
};
