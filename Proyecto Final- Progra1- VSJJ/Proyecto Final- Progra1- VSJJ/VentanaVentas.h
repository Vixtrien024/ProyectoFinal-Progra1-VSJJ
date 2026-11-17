#pragma once
#include <vector>
#include <QDialog>
#include "Carrito.h"
#include "GestorVentas.h"
#include "Producto.h"
#include "ui_VentanaVentas.h"
using namespace std;

// Ventana de ventas: elegir producto, agregarlo al carrito y cobrar.
class VentanaVentas : public QDialog {
    Q_OBJECT

private:
    Ui::VentanaVentas ui;

    GestorVentas* gestor;
    Carrito carrito;
    vector<Producto> productos;

    // Vuelve a escribir la tabla del carrito y el total
    void actualizarTotal();

private slots:
    void agregarProducto();
    void quitarProducto();
    void cobrar();

public:
    VentanaVentas(GestorVentas* gestor, QWidget* parent = nullptr);

    // Llena la lista de productos que se pueden vender.
    void setProductos(vector<Producto> productos);
};
