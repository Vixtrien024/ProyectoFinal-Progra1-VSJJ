
#pragma once

#include <QtWidgets/QMainWindow>
#include <QStackedWidget>
#include <string>

#include "ui_ProyectoFinalProgra1VSJJ.h"

// Modulo de usuarios
#include "Autenticacion.h"
#include "GestorUsuarios.h"

// Modulo de registro de actividades
#include "GestorActividades.h"

// Modulo de productos e inventario
#include "ArchivoProductos.h"
#include "GestorProductos.h"
#include "Inventario.h"
#include "GestionProductos.h"

// Modulo de ventas
#include "GestorVentas.h"

using namespace std;

// ==========================================
// VENTANA PRINCIPAL DEL SISTEMA LA PULPE
// ==========================================

class ProyectoFinalProgra1VSJJ : public QMainWindow
{
    Q_OBJECT

private:

    // ======================================
    // INTERFAZ GRAFICA PRINCIPAL
    // ======================================

    Ui::ProyectoFinalProgra1VSJJClass ui;

    // ======================================
    // AUTENTICACION Y USUARIOS
    // ======================================

    Autenticacion* autenticacion;
    GestorUsuarios* gestorUsuarios;

    // ======================================
    // REGISTRO DE ACTIVIDADES
    // ======================================

    // Se comparte el gestor creado en main.cpp
    // No se crea una segunda instancia.
    GestorActividades* gestorActividades;

    // ======================================
    // PRODUCTOS E INVENTARIO
    // ======================================

    ArchivoProductos archivo;
    GestorProductos gestor;
    Inventario inventario;

    // ======================================
    // VENTAS Y COBROS
    // ======================================

    GestorVentas gestorVentas;

    // ======================================
    // NAVEGACION ENTRE PANTALLAS
    // ======================================

    QStackedWidget* paginas;
    QWidget* paginaMenu;
    QWidget* paginaProductos;

    // Interfaz de productos de Julian
    GestionProductos* pantallaProductos;

public:

    // ======================================
    // CONSTRUCTOR
    // ======================================

    ProyectoFinalProgra1VSJJ(
        Autenticacion* autenticacion,
        GestorUsuarios* gestorUsuarios,
        GestorActividades* gestorActividades,
        QWidget* parent = nullptr
    );

    // ======================================
    // DESTRUCTOR
    // ======================================

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
