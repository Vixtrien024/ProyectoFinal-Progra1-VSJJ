#include "VentanaVentas.h"
#include <QHeaderView>
#include <QTableWidgetItem>

VentanaVentas::VentanaVentas(GestorVentas* gestor, QWidget* parent) : QDialog(parent) {
    ui.setupUi(this);
    this->gestor = gestor;

    // Las columnas de la tabla ocupan todo el ancho
    ui.tablaCarrito->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Sin la columna de numeros de fila
    ui.tablaCarrito->verticalHeader()->setVisible(false);

    // Cuando se aprieta un boton, se llama a su funcion
    connect(ui.btnAgregar, &QPushButton::clicked, this, &VentanaVentas::agregarProducto);
    connect(ui.btnQuitar, &QPushButton::clicked, this, &VentanaVentas::quitarProducto);
    connect(ui.btnCobrar, &QPushButton::clicked, this, &VentanaVentas::cobrar);
    connect(ui.btnRegresar, &QPushButton::clicked, this, &VentanaVentas::close);

    actualizarTotal();
}

void VentanaVentas::setProductos(vector<Producto> productos) {
    this->productos = productos;

    ui.cmbProducto->clear();
    for (int i = 0; i < (int)productos.size(); i++) {
        ui.cmbProducto->addItem(QString::fromStdString(productos[i].getNombre()));
    }
}

void VentanaVentas::agregarProducto() {
    int posicion = ui.cmbProducto->currentIndex();
    if (posicion < 0) {
        ui.lblMensaje->setText("No hay productos disponibles");
        return;
    }

    bool seAgrego = carrito.agregar(productos[posicion], ui.spnCantidad->value());
    if (!seAgrego) {
        ui.lblMensaje->setText("No se pudo agregar el producto");
        return;
    }

    ui.spnCantidad->setValue(1);
    ui.lblMensaje->setText("");
    actualizarTotal();
}

void VentanaVentas::quitarProducto() {
    int fila = ui.tablaCarrito->currentRow();
    if (fila < 0) {
        ui.lblMensaje->setText("Seleccione un producto de la tabla");
        return;
    }

    // La fila de la tabla es la misma posicion que en el carrito
    vector<DetalleVenta> articulos = carrito.detalles();
    carrito.quitar(articulos[fila].getCodigoProducto());

    ui.lblMensaje->setText("");
    actualizarTotal();
}

void VentanaVentas::cobrar() {
    qint64 efectivo = ui.txtEfectivo->text().toLongLong();

    ResultadoVenta resultado = gestor->confirmar(carrito, efectivo);

    if (!resultado.fueExitosa()) {
        ui.lblMensaje->setText(QString::fromStdString(resultado.getMensaje()));
        return;
    }

    Venta venta = resultado.getVenta();
    ui.lblMensaje->setText("Venta " + QString::number(venta.getNumero()) +
        " realizada. Cambio: ₡" + QString::number(venta.getCambio()));

    carrito.vaciar();
    ui.txtEfectivo->clear();
    actualizarTotal();
}

void VentanaVentas::actualizarTotal() {
    vector<DetalleVenta> articulos = carrito.detalles();

    ui.tablaCarrito->setRowCount((int)articulos.size());
    for (int i = 0; i < (int)articulos.size(); i++) {
        ui.tablaCarrito->setItem(i, 0, new QTableWidgetItem(
            QString::fromStdString(articulos[i].getNombreProducto())));
        ui.tablaCarrito->setItem(i, 1, new QTableWidgetItem(
            QString::number(articulos[i].getCantidad())));
        ui.tablaCarrito->setItem(i, 2, new QTableWidgetItem(
            "₡" + QString::number(articulos[i].getPrecioUnitario())));
        ui.tablaCarrito->setItem(i, 3, new QTableWidgetItem(
            "₡" + QString::number(articulos[i].subtotal())));
    }

    ui.lblTotal->setText("Total: ₡" + QString::number(carrito.total()));
}
