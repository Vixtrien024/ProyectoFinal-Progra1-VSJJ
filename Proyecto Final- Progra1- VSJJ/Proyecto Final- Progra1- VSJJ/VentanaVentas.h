#pragma once

#include <string>
#include <vector>
#include <QWidget>
#include "Carrito.h"
#include "GestorVentas.h"
#include "Producto.h"

using namespace std;

class QComboBox;
class QSpinBox;
class QTableWidget;
class QLabel;
class QLineEdit;

// Pantalla de ventas y cobro. Solo muestra y recoge datos;
// las reglas viven en Carrito y GestorVentas.
// Sin Q_OBJECT: usa lambdas, asi no necesita moc.
class VentanaVentas : public QWidget {
private:
    GestorVentas& gestor;
    const vector<Producto>& catalogo;
    string cajero;
    Carrito carrito;

    QComboBox* cmbProducto;
    QSpinBox* spnCantidad;
    QTableWidget* tablaCarrito;
    QLabel* lblTotal;
    QLineEdit* txtEfectivo;
    QLabel* lblMensaje;

    void agregar();
    void quitarSeleccionado();
    void cobrar();
    void refrescarCarrito();
    void mostrarMensaje(const QString& texto, bool error);

public:
    VentanaVentas(GestorVentas& gestor, const vector<Producto>& catalogo,
        const string& cajero, QWidget* parent = nullptr);
};
