
#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include <string>

#include "ui_ProyectoFinalProgra1VSJJ.h"

// Modulo de usuarios
#include "Autenticacion.h"
#include "GestorUsuarios.h"

// Modulo de productos e inventario
#include "ArchivoProductos.h"
#include "GestorProductos.h"
#include "Inventario.h"
#include "GestionProductos.h"

// Modulo de ventas
#include "GestorVentas.h"

using namespace std;

// Ventana principal del sistema La Pulpe
class ProyectoFinalProgra1VSJJ : public QMainWindow
{
    Q_OBJECT

private:

    // Interfaz grafica principal
    Ui::ProyectoFinalProgra1VSJJClass ui;

    // Autenticacion y usuarios
    Autenticacion* autenticacion;
    GestorUsuarios* gestorUsuarios;

    // Productos e inventario
    ArchivoProductos archivo;
    GestorProductos gestor;
    Inventario inventario;

    // Ventas y cobros
    // Se mantiene una sola instancia para compartir
    // las ventas realizadas entre los modulos.
    GestorVentas gestorVentas;

    // Navegacion entre pantallas
    QStackedWidget* paginas;
    QWidget* paginaMenu;
    QWidget* paginaProductos;

    // Interfaz de productos de Julian
    GestionProductos* pantallaProductos;

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

    // Abrir productos e inventario
    void abrirProductos();

    // Abrir ventas y cobros
    void abrirVentas();

    // Abrir historial y tickets
    void abrirHistorial();

    // Abrir gestion de usuarios
    void abrirUsuarios();

    // Cerrar sesion
    void cerrarSesion();

    // Regresar al menu principal
    void mostrarMenu();

private:

    // Mostrar informacion del usuario conectado
    void mostrarUsuarioActual();

    // Configurar permisos segun el rol
    void configurarPermisos();

    // Verificar permisos de acceso
    bool verificarAcceso(Permiso permiso);
};
