#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProyectoFinalProgra1VSJJ.h"
#include "ArchivoProductos.h"
#include "GestorProductos.h"
#include "Inventario.h"
#include "GestionProductos.h"

// Ventana principal de La Pulpe.
// Crea el archivo, el gestor y el inventario, y dentro de ella se muestra
// la pantalla que se dibujo en GestionProductos.ui.
class ProyectoFinalProgra1VSJJ : public QMainWindow
{
    Q_OBJECT

public:
    ProyectoFinalProgra1VSJJ(QWidget *parent = nullptr);
    ~ProyectoFinalProgra1VSJJ();

private:
    Ui::ProyectoFinalProgra1VSJJClass ui;//Interfaz dibujada en ProyectoFinalProgra1VSJJ.ui

    ArchivoProductos archivo;//El archivo de texto donde se guardan los productos
    GestorProductos gestor;//La coleccion de productos (carga lo que haya en el archivo)
    Inventario inventario;//El control de existencias (lo usara el modulo de ventas)
    GestionProductos* pantallaProductos;//La pantalla dibujada en GestionProductos.ui
};
