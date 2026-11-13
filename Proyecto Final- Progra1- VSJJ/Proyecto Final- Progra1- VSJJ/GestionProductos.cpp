
#include "GestionProductos.h"
#include "ui_GestionProductos.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QBrush>
#include <QColor>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QSignalBlocker>
#include <QtMath>

// ================= CONSTRUCTORES =================

GestionProductos::GestionProductos(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::GestionProductos),
      gestor(nullptr) {

    ui->setupUi(this);
    configurarInterfaz();
}

GestionProductos::GestionProductos(
    GestorProductos* gestor, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::GestionProductos),
      gestor(nullptr) {

    ui->setupUi(this);
    configurarInterfaz();
    setGestor(gestor);
}

GestionProductos::~GestionProductos() {
    delete ui;
}

// ================= CONFIGURACION =================

void GestionProductos::setGestor(GestorProductos* gestor) {

    this->gestor = gestor;

    actualizarTabla();

    if (gestor != nullptr) {
        limpiarFormulario();
    }
}

void GestionProductos::configurarInterfaz() {

    setAttribute(Qt::WA_StyledBackground, true);

    // Configurar tabla
    ui->tblProductos->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    ui->tblProductos->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->tblProductos->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->tblProductos->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->tblProductos->setAlternatingRowColors(true);
    ui->tblProductos->setFocusPolicy(Qt::NoFocus);

    // Configurar valores
    ui->spnCodigo->setMinimum(1);
    ui->spnCodigo->setMaximum(999999999);

    ui->spnPrecio->setDecimals(0);
    ui->spnPrecio->setMinimum(0);
    ui->spnPrecio->setMaximum(999999999);
    ui->spnPrecio->setPrefix(
        QString::fromUtf8("\xE2\x82\xA1") + " ");

    ui->spnExistencias->setMinimum(0);
    ui->spnExistencias->setMaximum(999999999);

    // Conectar botones
    connect(ui->btnAgregar, &QPushButton::clicked,
        this, &GestionProductos::agregar);

    connect(ui->btnEditar, &QPushButton::clicked,
        this, &GestionProductos::editar);

    connect(ui->btnDesactivar, &QPushButton::clicked,
        this, &GestionProductos::desactivar);

    connect(ui->btnNuevo, &QPushButton::clicked,
        this, &GestionProductos::limpiarFormulario);

    connect(ui->btnRegresar, &QPushButton::clicked,
        this, &GestionProductos::regresar);

    // Busqueda
    connect(ui->txtBuscar, &QLineEdit::textChanged,
        this, &GestionProductos::filtrarTabla);

    // Seleccionar producto
    connect(ui->tblProductos,
        &QTableWidget::itemSelectionChanged,
        this, &GestionProductos::cargarFormulario);
}

// ================= TABLA =================

int GestionProductos::codigoSeleccionado() const {

    int fila = ui->tblProductos->currentRow();

    if (fila < 0 ||
        ui->tblProductos->item(fila, 0) == nullptr) {
        return -1;
    }

    return ui->tblProductos->item(fila, 0)
        ->text().toInt();
}

void GestionProductos::actualizarTabla() {

    QTableWidget* tabla = ui->tblProductos;

    QSignalBlocker bloqueo(tabla);

    tabla->setRowCount(0);

    if (gestor == nullptr) {
        return;
    }

    vector<Producto> lista = gestor->listar();

    const QString colon =
        QString::fromUtf8("\xE2\x82\xA1");

    for (const Producto& p : lista) {

        int fila = tabla->rowCount();
        tabla->insertRow(fila);

        tabla->setItem(fila, 0,
            new QTableWidgetItem(
                QString::number(p.getCodigo())));

        tabla->setItem(fila, 1,
            new QTableWidgetItem(
                QString::fromStdString(p.getNombre())));

        tabla->setItem(fila, 2,
            new QTableWidgetItem(
                colon + QString::number(p.getPrecio())));

        tabla->setItem(fila, 3,
            new QTableWidgetItem(
                QString::fromStdString(p.getCategoria())));

        tabla->setItem(fila, 4,
            new QTableWidgetItem(
                QString::number(p.getExistencias())));

        tabla->setItem(fila, 5,
            new QTableWidgetItem(
                p.estaActivo() ? "Activo" : "Inactivo"));

        // Mostrar productos inactivos en gris
        if (!p.estaActivo()) {

            for (int c = 0; c < 6; c++) {
                tabla->item(fila, c)->setForeground(
                    QBrush(QColor(110, 130, 120)));
            }
        }
    }

    filtrarTabla(ui->txtBuscar->text());
}

void GestionProductos::filtrarTabla(
    const QString& texto) {

    QString filtro = texto.trimmed().toLower();

    QTableWidget* tabla = ui->tblProductos;

    for (int fila = 0;
         fila < tabla->rowCount(); fila++) {

        QString codigo =
            tabla->item(fila, 0)->text();

        QString nombre =
            tabla->item(fila, 1)->text().toLower();

        bool coincide =
            filtro.isEmpty() ||
            codigo.startsWith(filtro) ||
            nombre.contains(filtro);

        tabla->setRowHidden(fila, !coincide);
    }
}

// ================= FORMULARIO =================

bool GestionProductos::esInactivo() const {
    return ui->cmbEstado->currentIndex() == 1;
}

int GestionProductos::siguienteCodigo() const {

    if (gestor == nullptr) {
        return 1;
    }

    int mayor = 0;

    for (const Producto& p : gestor->listar()) {
        if (p.getCodigo() > mayor) {
            mayor = p.getCodigo();
        }
    }

    return mayor + 1;
}

Producto GestionProductos::leerFormulario() const {

    Producto producto(
        ui->spnCodigo->value(),
        ui->txtNombre->text().trimmed().toStdString(),
        static_cast<qint64>(
            qRound64(ui->spnPrecio->value())),
        ui->txtCategoria->text().trimmed().toStdString(),
        ui->spnExistencias->value()
    );

    if (esInactivo()) {
        producto.desactivar();
    }

    return producto;
}

void GestionProductos::cargarFormulario() {

    if (gestor == nullptr) {
        return;
    }

    int codigo = codigoSeleccionado();

    if (codigo == -1) {
        return;
    }

    optional<Producto> producto = gestor->buscar(codigo);

    if (!producto.has_value()) {
        return;
    }

    ui->spnCodigo->setValue(producto->getCodigo());
    ui->spnCodigo->setEnabled(false);

    ui->txtNombre->setText(
        QString::fromStdString(producto->getNombre()));

    ui->spnPrecio->setValue(
        static_cast<double>(producto->getPrecio()));

    ui->txtCategoria->setText(
        QString::fromStdString(producto->getCategoria()));

    ui->spnExistencias->setValue(
        producto->getExistencias());

    ui->cmbEstado->setCurrentIndex(
        producto->estaActivo() ? 0 : 1);
}

void GestionProductos::limpiarFormulario() {

    QSignalBlocker bloqueo(ui->tblProductos);

    ui->tblProductos->clearSelection();
    ui->tblProductos->setCurrentCell(-1, -1);

    ui->spnCodigo->setEnabled(true);
    ui->spnCodigo->setValue(siguienteCodigo());

    ui->txtNombre->clear();
    ui->spnPrecio->setValue(0);
    ui->txtCategoria->clear();
    ui->spnExistencias->setValue(0);

    ui->cmbEstado->setCurrentIndex(0);

    ui->txtNombre->setFocus();
}

// ================= AGREGAR =================

void GestionProductos::agregar() {

    if (gestor == nullptr) {
        QMessageBox::warning(this, "Error",
            "No se ha configurado el gestor de productos.");
        return;
    }

    if (!ui->spnCodigo->isEnabled()) {
        QMessageBox::information(this, "Agregar",
            "Presione Nuevo antes de agregar otro producto.");
        return;
    }

    if (ui->txtNombre->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error",
            "Debe ingresar el nombre del producto.");
        return;
    }

    if (ui->spnPrecio->value() <= 0) {
        QMessageBox::warning(this, "Error",
            "El precio debe ser mayor a cero.");
        return;
    }

    if (!gestor->agregar(leerFormulario())) {
        QMessageBox::warning(this, "Error",
            "No se pudo agregar el producto. "
            "Verifique el codigo y el archivo.");
        return;
    }

    actualizarTabla();
    limpiarFormulario();

    ui->lblEstado->setText("Producto agregado correctamente.");
}

// ================= EDITAR =================

void GestionProductos::editar() {

    if (gestor == nullptr) {
        QMessageBox::warning(this, "Error",
            "No se ha configurado el gestor de productos.");
        return;
    }

    int codigo = codigoSeleccionado();

    if (codigo == -1) {
        QMessageBox::information(this, "Editar",
            "Seleccione un producto de la tabla.");
        return;
    }

    if (ui->txtNombre->text().trimmed().isEmpty() ||
        ui->spnPrecio->value() <= 0) {

        QMessageBox::warning(this, "Error",
            "Ingrese un nombre y un precio valido.");
        return;
    }

    optional<Producto> actual = gestor->buscar(codigo);

    if (!actual.has_value()) {
        QMessageBox::warning(this, "Error",
            "No se encontro el producto seleccionado.");
        return;
    }

    Producto editado = *actual;

    if (!editado.actualizarDatos(
        ui->txtNombre->text().trimmed().toStdString(),
        static_cast<qint64>(
            qRound64(ui->spnPrecio->value())),
        ui->txtCategoria->text().trimmed().toStdString(),
        ui->spnExistencias->value())) {

        QMessageBox::warning(this, "Error",
            "Los datos del producto no son validos.");
        return;
    }

    if (esInactivo()) {
        editado.desactivar();
    }
    else {
        editado.activar();
    }

    if (!gestor->modificar(editado)) {
        QMessageBox::warning(this, "Error",
            "No se pudo guardar la modificacion.");
        return;
    }

    actualizarTabla();
    limpiarFormulario();

    ui->lblEstado->setText("Producto actualizado correctamente.");
}

// ================= DESACTIVAR =================

void GestionProductos::desactivar() {

    if (gestor == nullptr) {
        QMessageBox::warning(this, "Error",
            "No se ha configurado el gestor de productos.");
        return;
    }

    int codigo = codigoSeleccionado();

    if (codigo == -1) {
        QMessageBox::information(this, "Desactivar",
            "Seleccione un producto de la tabla.");
        return;
    }

    optional<Producto> producto = gestor->buscar(codigo);

    if (!producto.has_value()) {
        QMessageBox::warning(this, "Error",
            "No se encontro el producto.");
        return;
    }

    if (!producto->estaActivo()) {
        QMessageBox::information(this, "Desactivar",
            "Este producto ya esta inactivo.");
        return;
    }

    QMessageBox::StandardButton respuesta =
        QMessageBox::question(
            this,
            "Desactivar producto",
            "Desea desactivar este producto?",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

    if (respuesta != QMessageBox::Yes) {
        return;
    }

    if (!gestor->desactivar(codigo)) {
        QMessageBox::warning(this, "Error",
            "No se pudo desactivar el producto.");
        return;
    }

    actualizarTabla();
    limpiarFormulario();

    ui->lblEstado->setText("Producto desactivado correctamente.");
}
