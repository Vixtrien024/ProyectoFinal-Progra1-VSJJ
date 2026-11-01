#include "VentanaVentas.h"
#include <QComboBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QMessageBox>

static QString colones(qint64 monto) {
    return QString("₡%1").arg(monto);
}

VentanaVentas::VentanaVentas(GestorVentas& gestor,
    const vector<Producto>& catalogo, const string& cajero, QWidget* parent)
    : QWidget(parent), gestor(gestor), catalogo(catalogo), cajero(cajero) {
    setWindowTitle("Ventas y cobro");
    resize(620, 480);

    cmbProducto = new QComboBox;
    for (const Producto& p : catalogo) {
        if (!p.estaActivo()) continue;
        cmbProducto->addItem(QString::fromStdString(p.getNombre()) + "  -  "
            + colones(p.getPrecio()), p.getId());
    }

    spnCantidad = new QSpinBox;
    spnCantidad->setRange(1, 9999);

    QPushButton* btnAgregar = new QPushButton("Agregar");
    QPushButton* btnQuitar = new QPushButton("Quitar seleccionado");

    tablaCarrito = new QTableWidget(0, 4);
    tablaCarrito->setHorizontalHeaderLabels(
        { "Producto", "Cantidad", "Precio", "Subtotal" });
    tablaCarrito->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablaCarrito->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaCarrito->setSelectionMode(QAbstractItemView::SingleSelection);
    tablaCarrito->setEditTriggers(QAbstractItemView::NoEditTriggers);

    lblTotal = new QLabel;
    lblTotal->setStyleSheet("font-size: 20px; font-weight: bold;");

    txtEfectivo = new QLineEdit;
    txtEfectivo->setPlaceholderText("Efectivo recibido");
    txtEfectivo->setValidator(new QIntValidator(0, 999999999, this));

    QPushButton* btnCobrar = new QPushButton("Cobrar");
    lblMensaje = new QLabel;

    QHBoxLayout* fila = new QHBoxLayout;
    fila->addWidget(cmbProducto, 1);
    fila->addWidget(spnCantidad);
    fila->addWidget(btnAgregar);

    QHBoxLayout* pago = new QHBoxLayout;
    pago->addWidget(lblTotal, 1);
    pago->addWidget(txtEfectivo);
    pago->addWidget(btnCobrar);

    QVBoxLayout* raiz = new QVBoxLayout(this);
    raiz->addLayout(fila);
    raiz->addWidget(tablaCarrito, 1);
    raiz->addWidget(btnQuitar);
    raiz->addLayout(pago);
    raiz->addWidget(lblMensaje);

    connect(btnAgregar, &QPushButton::clicked, this, [this] { agregar(); });
    connect(btnQuitar, &QPushButton::clicked, this, [this] { quitarSeleccionado(); });
    connect(btnCobrar, &QPushButton::clicked, this, [this] { cobrar(); });
    connect(txtEfectivo, &QLineEdit::returnPressed, this, [this] { cobrar(); });

    refrescarCarrito();
}

void VentanaVentas::agregar() {
    if (cmbProducto->currentIndex() < 0) {
        mostrarMensaje("No hay productos disponibles", true);
        return;
    }
    int id = cmbProducto->currentData().toInt();
    for (const Producto& p : catalogo) {
        if (p.getId() != id) continue;
        if (!carrito.agregarProducto(p, spnCantidad->value())) {
            mostrarMensaje("No se pudo agregar el producto", true);
            return;
        }
        spnCantidad->setValue(1);
        mostrarMensaje("", false);
        refrescarCarrito();
        return;
    }
}

void VentanaVentas::quitarSeleccionado() {
    int fila = tablaCarrito->currentRow();
    if (fila < 0) {
        mostrarMensaje("Seleccione una fila para quitar", true);
        return;
    }
    carrito.quitarProducto(carrito.getArticulos()[fila].getIdProducto());
    refrescarCarrito();
}

void VentanaVentas::cobrar() {
    if (carrito.estaVacio()) {
        mostrarMensaje(QString::fromStdString(
            GestorVentas::mensaje(ResultadoVenta::CarritoVacio)), true);
        return;
    }
    bool ok = false;
    qint64 efectivo = txtEfectivo->text().toLongLong(&ok);
    if (!ok) {
        mostrarMensaje("Escriba el efectivo recibido", true);
        return;
    }

    Venta venta;
    ResultadoVenta r = gestor.procesarVenta(carrito, efectivo, cajero, &venta);
    if (r != ResultadoVenta::Exitosa) {
        mostrarMensaje(QString::fromStdString(GestorVentas::mensaje(r)), true);
        return;
    }

    QMessageBox::information(this, "Venta realizada",
        QString("Venta #%1\nTotal: %2\nCambio: %3")
        .arg(venta.getNumero()).arg(colones(venta.getTotal()))
        .arg(colones(venta.getCambio())));
    carrito.vaciar();
    txtEfectivo->clear();
    mostrarMensaje("", false);
    refrescarCarrito();
}

void VentanaVentas::refrescarCarrito() {
    const vector<DetalleVenta>& art = carrito.getArticulos();
    tablaCarrito->setRowCount(static_cast<int>(art.size()));
    for (size_t i = 0; i < art.size(); i++) {
        const DetalleVenta& d = art[i];
        int r = static_cast<int>(i);
        tablaCarrito->setItem(r, 0, new QTableWidgetItem(
            QString::fromStdString(d.getNombreProducto())));
        tablaCarrito->setItem(r, 1, new QTableWidgetItem(
            QString::number(d.getCantidad())));
        tablaCarrito->setItem(r, 2, new QTableWidgetItem(
            colones(d.getPrecioUnitario())));
        tablaCarrito->setItem(r, 3, new QTableWidgetItem(
            colones(d.calcularSubtotal())));
    }
    lblTotal->setText("Total: " + colones(carrito.calcularTotal()));
}

void VentanaVentas::mostrarMensaje(const QString& texto, bool error) {
    lblMensaje->setStyleSheet(error ? "color: #b00020;" : "");
    lblMensaje->setText(texto);
}
