
#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include <string>

#include "ui_ProyectoFinalProgra1VSJJ.h"
#include "Autenticacion.h"
#include "GestorUsuarios.h"
#include "ArchivoProductos.h"
#include "GestorProductos.h"
#include "Inventario.h"
#include "GestionProductos.h"

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

    // Contenedor que alterna entre el menu y las pantallas de los modulos
    QStackedWidget* paginas;
    QWidget* paginaMenu;//El menu dibujado en ProyectoFinalProgra1VSJJ.ui
    QWidget* paginaProductos;//La pagina que contiene la pantalla de productos
    GestionProductos* pantallaProductos;//El widget promovido desde GestionProductos.ui

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

    // Volver al menu principal desde una pantalla de modulo
    void mostrarMenu();

private:

    // Mostrar informacion del usuario conectado
    void mostrarUsuarioActual();

    // Habilitar botones segun permisos
    void configurarPermisos();

    // Verificar acceso a un modulo
    bool verificarAcceso(Permiso permiso);
};
