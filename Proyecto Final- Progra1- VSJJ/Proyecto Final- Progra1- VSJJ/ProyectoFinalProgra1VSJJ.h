
#pragma once

#include <QtWidgets/QMainWindow>
#include <string>

#include "ui_ProyectoFinalProgra1VSJJ.h"
#include "Autenticacion.h"
#include "GestorUsuarios.h"
#include "ArchivoProductos.h"
#include "GestorProductos.h"
#include "Inventario.h"

using namespace std;

// Ventana principal de La Pulpe.
// Muestra el menu de modulos y administra el acceso segun el rol.
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

    // Modulo de productos e inventario
    ArchivoProductos archivo;//El archivo de texto donde se guardan los productos
    GestorProductos gestor;//La coleccion de productos (carga lo que haya en el archivo)
    Inventario inventario;//El control de existencias (lo usara el modulo de ventas)

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
