#include "VentanaVentas.h"

VentanaVentas::VentanaVentas(vector<Producto> productos, QWidget* parent) : QWidget(parent) {
    this->productos = productos;
    setWindowTitle("Ventas y cobro");
    resize(450, 420);

    // Lista con los nombres de los productos
    cmbProducto = new QComboBox(this);
    cmbProducto->setGeometry(10, 10, 250, 30);
    for (int i = 0; i < (int)productos.size(); i++) {
        cmbProducto->addItem(QString::fromStdString(productos[i].getNombre()));
    }

    // Cantidad a comprar
    spnCantidad = new QSpinBox(this);
    spnCantidad->setGeometry(270, 10, 60, 30);
    spnCantidad->setMinimum(1);

    btnAgregar = new QPushButton("Agregar", this);
    btnAgregar->setGeometry(340, 10, 100, 30);

    // Aqui se muestran los productos del carrito
    listaCarrito = new QListWidget(this);
    listaCarrito->setGeometry(10, 50, 430, 220);

    lblTotal = new QLabel("Total: 0", this);
    lblTotal->setGeometry(10, 280, 200, 30);

    txtEfectivo = new QLineEdit(this);
    txtEfectivo->setPlaceholderText("Efectivo recibido");
    txtEfectivo->setGeometry(10, 320, 200, 30);

    btnCobrar = new QPushButton("Cobrar", this);
    btnCobrar->setGeometry(220, 320, 100, 30);

    lblMensaje = new QLabel("", this);
    lblMensaje->setGeometry(10, 360, 430, 30);

    // Cuando se aprieta un boton, se llama a su funcion
    connect(btnAgregar, SIGNAL(clicked()), this, SLOT(agregar()));
    connect(btnCobrar, SIGNAL(clicked()), this, SLOT(cobrar()));
}

void VentanaVentas::agregar() {
    int posicion = cmbProducto->currentIndex();
    if (posicion < 0) {
        lblMensaje->setText("No hay productos");
        return;
    }

    carrito.agregarProducto(productos[posicion], spnCantidad->value());
    lblMensaje->setText("");
    mostrarCarrito();
}

void VentanaVentas::cobrar() {
    int efectivo = txtEfectivo->text().toInt();

    string resultado = gestor.procesarVenta(carrito, efectivo);

    if (resultado != "OK") {
        lblMensaje->setText(QString::fromStdString(resultado));
        return;
    }

    Venta venta = gestor.getUltimaVenta();
    lblMensaje->setText("Venta " + QString::number(venta.getNumero()) +
        " realizada. Cambio: " + QString::number(venta.getCambio()));

    carrito.vaciar();
    txtEfectivo->clear();
    mostrarCarrito();
}

// Vuelve a escribir la lista del carrito y el total
void VentanaVentas::mostrarCarrito() {
    listaCarrito->clear();

    vector<DetalleVenta> articulos = carrito.getArticulos();
    for (int i = 0; i < (int)articulos.size(); i++) {
        QString linea = QString::fromStdString(articulos[i].getNombre()) +
            "  x" + QString::number(articulos[i].getCantidad()) +
            "  = " + QString::number(articulos[i].calcularSubtotal());
        listaCarrito->addItem(linea);
    }

    lblTotal->setText("Total: " + QString::number(carrito.calcularTotal()));
}
