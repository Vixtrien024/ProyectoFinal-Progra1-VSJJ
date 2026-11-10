#include "GestionProductos.h"
#include "ui_GestionProductos.h"//Clase que Qt genera a partir de GestionProductos.ui

#include <QMessageBox>
#include <QHeaderView>
#include <QBrush>
#include <QColor>

// ================= PARTE 1: preparar la pantalla y llenar la tabla =================

// Constructor
GestionProductos::GestionProductos(GestorProductos* gestor, QWidget* parent)
    : QWidget(parent), ui(new Ui::GestionProductos), gestor(gestor) {

    ui->setupUi(this);//Crea todos los controles que se dibujaron en el .ui
    configurarInterfaz();//Conexiones y ajustes de la tabla
    actualizarTabla();//Llena la tabla con los productos del gestor
    limpiarFormulario();//Deja el formulario listo para capturar un producto nuevo
}//fin del constructor


// Destructor: libera la interfaz que se creo con new
GestionProductos::~GestionProductos() {
    delete ui;
}//fin del destructor


// Ajustes que se hacen por codigo (lo visual y el estilo vienen en el .ui)
void GestionProductos::configurarInterfaz() {

    // El estilo (colores, bordes) se puso en el .ui; esto hace que el fondo si se respete
    setAttribute(Qt::WA_StyledBackground, true);

    // La tabla llena todo el ancho y no muestra el recuadro punteado de foco
    ui->tblProductos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblProductos->setFocusPolicy(Qt::NoFocus);

    // --- Conectar cada evento con su funcion (senal -> slot) ---
    connect(ui->btnAgregar, &QPushButton::clicked, this, &GestionProductos::agregar);
    connect(ui->btnEditar, &QPushButton::clicked, this, &GestionProductos::editar);
    connect(ui->btnDesactivar, &QPushButton::clicked, this, &GestionProductos::desactivar);
    connect(ui->btnNuevo, &QPushButton::clicked, this, &GestionProductos::limpiarFormulario);
    connect(ui->btnRegresar, &QPushButton::clicked, this, &GestionProductos::regresar);

    // Mientras se escribe en el cuadro de busqueda, se filtra la tabla
    connect(ui->txtBuscar, &QLineEdit::textChanged, this, &GestionProductos::filtrarTabla);

    // Al elegir una fila de la tabla, sus datos pasan al formulario
    connect(ui->tblProductos, &QTableWidget::itemSelectionChanged,
        this, &GestionProductos::cargarFormulario);
}//fin de configurarInterfaz


// Codigo del producto de la fila elegida (-1 si no hay ninguna)
int GestionProductos::codigoSeleccionado() const {

    int fila = ui->tblProductos->currentRow();

    if (fila < 0 || ui->tblProductos->item(fila, 0) == nullptr) {
        return -1;
    }//fin del if

    return ui->tblProductos->item(fila, 0)->text().toInt();
}//fin de codigoSeleccionado


// Volver a llenar la tabla con lo que tenga el gestor
void GestionProductos::actualizarTabla() {

    vector<Producto> lista = gestor->listar();
    const QString colon = QString::fromUtf8("\xE2\x82\xA1");//Simbolo de colon
    QTableWidget* tabla = ui->tblProductos;

    tabla->blockSignals(true);//Evita que cargarFormulario se dispare mientras se llena
    tabla->setRowCount(0);//Vacia la tabla

    for (const Producto& p : lista) {

        int fila = tabla->rowCount();
        tabla->insertRow(fila);

        tabla->setItem(fila, 0, new QTableWidgetItem(QString::number(p.getCodigo())));
        tabla->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(p.getNombre())));
        tabla->setItem(fila, 2, new QTableWidgetItem(colon + QString::number(p.getPrecio())));
        tabla->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(p.getCategoria())));
        tabla->setItem(fila, 4, new QTableWidgetItem(QString::number(p.getExistencias())));
        tabla->setItem(fila, 5, new QTableWidgetItem(p.estaActivo() ? "Activo" : "Inactivo"));

        if (!p.estaActivo()) {//Los desactivados se ven en gris
            for (int c = 0; c < 6; c++) {
                tabla->item(fila, c)->setForeground(QBrush(QColor(110, 130, 120)));
            }//fin del for interno
        }//fin del if

    }//fin del for

    tabla->blockSignals(false);

    filtrarTabla(ui->txtBuscar->text());//Vuelve a aplicar la busqueda que hubiera escrita
}//fin de actualizarTabla


// Lo que se escribe en txtBuscar oculta las filas que no coinciden.
// Busca por codigo (que empiece igual) o por nombre (que lo contenga),
// sin distinguir mayusculas.
void GestionProductos::filtrarTabla(const QString& texto) {

    QString filtro = texto.trimmed().toLower();
    QTableWidget* tabla = ui->tblProductos;

    for (int fila = 0; fila < tabla->rowCount(); fila++) {

        bool coincide = filtro.isEmpty();//Sin texto de busqueda se muestran todas

        if (!coincide) {
            QString codigo = tabla->item(fila, 0) != nullptr
                ? tabla->item(fila, 0)->text() : QString();
            QString nombre = tabla->item(fila, 1) != nullptr
                ? tabla->item(fila, 1)->text() : QString();

            coincide = codigo.startsWith(filtro) || nombre.toLower().contains(filtro);
        }//fin del if

        tabla->setRowHidden(fila, !coincide);
    }//fin del for
}//fin de filtrarTabla


// ================= PARTE 2: formulario y botones =================

// Lee el estado elegido en el comboBox (0 = Activo, 1 = Inactivo, como en el .ui)
bool GestionProductos::esInactivo() const {
    return ui->cmbEstado->currentIndex() == 1;
}//fin de esInactivo


// Codigo sugerido para un producto nuevo: el mayor usado + 1
int GestionProductos::siguienteCodigo() const {

    int mayor = 0;

    for (const Producto& p : gestor->listar()) {
        if (p.getCodigo() > mayor) {
            mayor = p.getCodigo();
        }//fin del if
    }//fin del for

    return mayor + 1;
}//fin de siguienteCodigo


// Arma un Producto con lo que hay escrito en el formulario
Producto GestionProductos::leerFormulario() const {

    Producto producto(ui->spnCodigo->value(),
        ui->txtNombre->text().toStdString(),
        static_cast<qint64>(qRound(ui->spnPrecio->value())),//qRound: redondea los centimos
        ui->txtCategoria->text().toStdString(),
        ui->spnExistencias->value());

    if (esInactivo()) {//El estado se elige en el comboBox
        producto.desactivar();
    }//fin del if

    return producto;
}//fin de leerFormulario


// Al elegir una fila, pasa sus datos al formulario
void GestionProductos::cargarFormulario() {

    int codigo = codigoSeleccionado();

    if (codigo == -1) {
        return;
    }//fin del if

    optional<Producto> producto = gestor->buscar(codigo);

    if (!producto.has_value()) {
        return;
    }//fin del if

    ui->spnCodigo->setValue(producto->getCodigo());
    ui->spnCodigo->setEnabled(false);//El codigo no se puede cambiar al editar
    ui->txtNombre->setText(QString::fromStdString(producto->getNombre()));
    ui->spnPrecio->setValue(static_cast<double>(producto->getPrecio()));
    ui->txtCategoria->setText(QString::fromStdString(producto->getCategoria()));
    ui->spnExistencias->setValue(producto->getExistencias());
    ui->cmbEstado->setCurrentIndex(producto->estaActivo() ? 0 : 1);//0 = Activo, 1 = Inactivo
}//fin de cargarFormulario


// Boton "Nuevo": deja el formulario listo para un producto nuevo
void GestionProductos::limpiarFormulario() {

    ui->tblProductos->clearSelection();
    ui->tblProductos->setCurrentCell(-1, -1);

    ui->spnCodigo->setEnabled(true);
    ui->spnCodigo->setValue(siguienteCodigo());//Sugiere un codigo libre
    ui->txtNombre->clear();
    ui->spnPrecio->setValue(0);
    ui->txtCategoria->clear();
    ui->spnExistencias->setValue(0);
    ui->cmbEstado->setCurrentIndex(0);//Un producto nuevo empieza activo
    ui->txtNombre->setFocus();
}//fin de limpiarFormulario


// Boton "Agregar"
void GestionProductos::agregar() {

    if (!ui->spnCodigo->isEnabled()) {//El formulario esta mostrando un producto existente
        QMessageBox::information(this, "Agregar",
            "Esta editando un producto. Presione \"Nuevo\" para limpiar el formulario.");
        return;
    }//fin del if

    // El gestor es quien decide si el producto es valido
    if (!gestor->agregar(leerFormulario())) {
        QMessageBox::warning(this, "No se pudo agregar",
            "Revise que el codigo no exista, que el nombre no este vacio, "
            "que el precio sea mayor a 0 y que se pueda guardar el archivo.");
        return;
    }//fin del if

    actualizarTabla();
    limpiarFormulario();
    ui->lblEstado->setText("Producto agregado.");
}//fin de agregar


// Boton "Editar"
void GestionProductos::editar() {

    int codigo = codigoSeleccionado();

    if (codigo == -1) {
        QMessageBox::information(this, "Editar", "Seleccione un producto de la tabla.");
        return;
    }//fin del if

    optional<Producto> actual = gestor->buscar(codigo);

    if (!actual.has_value()) {
        return;
    }//fin del if

    // Se edita una copia; actualizarDatos valida todo antes de cambiar nada
    Producto editado = *actual;

    if (!editado.actualizarDatos(ui->txtNombre->text().toStdString(),
        static_cast<qint64>(qRound(ui->spnPrecio->value())),
        ui->txtCategoria->text().toStdString(),
        ui->spnExistencias->value())) {
        QMessageBox::warning(this, "No se pudo editar",
            "Revise que el nombre no este vacio y que el precio sea mayor a 0.");
        return;
    }//fin del if

    // El estado tambien se edita desde el comboBox del formulario
    if (esInactivo()) {
        editado.desactivar();
    }//fin del if
    else {
        editado.activar();
    }//fin del else

    if (!gestor->modificar(editado)) {
        QMessageBox::warning(this, "No se pudo editar",
            "No se pudo guardar el cambio.");
        return;
    }//fin del if

    actualizarTabla();
    limpiarFormulario();
    ui->lblEstado->setText("Producto actualizado.");
}//fin de editar


// Boton "Desactivar"
void GestionProductos::desactivar() {

    int codigo = codigoSeleccionado();

    if (codigo == -1) {
        QMessageBox::information(this, "Desactivar", "Seleccione un producto de la tabla.");
        return;
    }//fin del if

    QMessageBox::StandardButton respuesta = QMessageBox::question(this,
        "Desactivar producto",
        "Desea desactivar este producto? Dejara de estar disponible para la venta.");

    if (respuesta != QMessageBox::Yes) {
        return;
    }//fin del if

    if (!gestor->desactivar(codigo)) {
        QMessageBox::warning(this, "Error", "No se pudo desactivar el producto.");
        return;
    }//fin del if

    actualizarTabla();
    limpiarFormulario();
    ui->lblEstado->setText("Producto desactivado.");
}//fin de desactivar
