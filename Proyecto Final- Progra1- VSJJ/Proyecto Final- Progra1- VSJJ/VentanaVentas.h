#pragma once
#include <vector>
#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include "Carrito.h"
#include "GestorVentas.h"
#include "Producto.h"
using namespace std;

// Ventana de ventas: elegir producto, agregarlo al carrito y cobrar.
class VentanaVentas : public QWidget {
    Q_OBJECT

private:
    vector<Producto> productos;
    Carrito carrito;
    GestorVentas gestor;

    QComboBox* cmbProducto;
    QSpinBox* spnCantidad;
    QPushButton* btnAgregar;
    QListWidget* listaCarrito;
    QLabel* lblTotal;
    QLineEdit* txtEfectivo;
    QPushButton* btnCobrar;
    QLabel* lblMensaje;

    void mostrarCarrito();

private slots:
    void agregar();
    void cobrar();

public:
    VentanaVentas(vector<Producto> productos, QWidget* parent = nullptr);
};
