
#include "GestionUsuarios.h"
#include "Seguridad.h"

#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QString>

using namespace std;

// Constructor
GestionUsuarios::GestionUsuarios(
    GestorUsuarios* gestor,
    Autenticacion* autenticacion,
    QWidget* parent
) : QDialog(parent)
{
    ui.setupUi(this);

    this->gestor = gestor;
    this->autenticacion = autenticacion;
    idSeleccionado = -1;

    // Ocultar contrasena
    ui.txtClave->setEchoMode(QLineEdit::Password);

    // Opciones del rol
    ui.cmbRol->clear();
    ui.cmbRol->addItem("Administrador");
    ui.cmbRol->addItem("Cajero");

    // Configurar tabla
    ui.tblUsuarios->setColumnCount(5);

    ui.tblUsuarios->setHorizontalHeaderLabels({
        "ID",
        "Nombre",
        "Usuario",
        "Rol",
        "Estado"
        });

    ui.tblUsuarios->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    ui.tblUsuarios->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    ui.tblUsuarios->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    ui.tblUsuarios->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    // Conectar botones
    connect(ui.btnAgregar, &QPushButton::clicked,
        this, &GestionUsuarios::agregarUsuario);

    connect(ui.btnModificar, &QPushButton::clicked,
        this, &GestionUsuarios::modificarUsuario);

    connect(ui.btnDesactivar, &QPushButton::clicked,
        this, &GestionUsuarios::desactivarUsuario);

    connect(ui.btnLimpiar, &QPushButton::clicked,
        this, &GestionUsuarios::limpiarFormulario);

    connect(ui.btnCerrar, &QPushButton::clicked,
        this, &GestionUsuarios::cerrarVentana);

    // Seleccionar usuario de la tabla
    connect(ui.tblUsuarios, &QTableWidget::cellClicked,
        this, &GestionUsuarios::seleccionarUsuario);

    cargarTabla();

    // Bloquear acceso sin permisos
    if (!verificarPermisos()) {
        ui.btnAgregar->setEnabled(false);
        ui.btnModificar->setEnabled(false);
        ui.btnDesactivar->setEnabled(false);
    }
}

// Destructor
GestionUsuarios::~GestionUsuarios()
{
}

// Verificar permisos
bool GestionUsuarios::verificarPermisos()
{
    if (autenticacion == nullptr) {
        return false;
    }

    return autenticacion->tienePermiso(
        Permiso::GestionarUsuarios
    );
}

// Validar campos
bool GestionUsuarios::validarCampos()
{
    if (ui.txtNombre->text().trimmed().isEmpty()) {
        QMessageBox::warning(
            this,
            "Datos incompletos",
            "Debe ingresar el nombre."
        );
        return false;
    }

    if (ui.txtNombreUsuario->text().trimmed().isEmpty()) {
        QMessageBox::warning(
            this,
            "Datos incompletos",
            "Debe ingresar el nombre de usuario."
        );
        return false;
    }

    return true;
}

// Obtener rol seleccionado
Rol GestionUsuarios::obtenerRolSeleccionado()
{
    if (ui.cmbRol->currentIndex() == 0) {
        return Rol::Administrador;
    }

    return Rol::Cajero;
}

// Registrar usuario
void GestionUsuarios::agregarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr) {
        QMessageBox::warning(
            this, "Acceso denegado",
            "No tiene permisos para registrar usuarios."
        );
        return;
    }

    if (!validarCampos()) {
        return;
    }

    if (ui.txtClave->text().isEmpty()) {
        QMessageBox::warning(
            this, "Datos incompletos",
            "Debe ingresar una contrasena."
        );
        return;
    }

    string nombre =
        ui.txtNombre->text().trimmed().toStdString();

    string nombreUsuario =
        ui.txtNombreUsuario->text().trimmed().toStdString();

    string clave =
        ui.txtClave->text().toStdString();

    if (clave.size() < 4) {
        QMessageBox::warning(
            this, "Datos incompletos",
            "La contrasena debe tener al menos 4 caracteres."
        );
        return;
    }

    if (gestor->buscarPorNombre(nombreUsuario) != nullptr) {
        QMessageBox::warning(
            this, "Usuario repetido",
            "Ya existe un usuario con ese nombre de usuario."
        );
        return;
    }

    Usuario nuevo;

    nuevo.setId(gestor->generarId());
    nuevo.setNombre(nombre);
    nuevo.setNombreUsuario(nombreUsuario);
    nuevo.setClaveHash(Seguridad::crearHashClave(clave));
    nuevo.setRol(obtenerRolSeleccionado());
    nuevo.setActivo(true);

    if (gestor->agregarUsuario(nuevo)) {
        QMessageBox::information(
            this, "Correcto",
            "Usuario registrado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else {
        QMessageBox::warning(
            this, "Error",
            "No se pudo registrar el usuario."
        );
    }
}

// Modificar usuario
void GestionUsuarios::modificarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr) {
        QMessageBox::warning(
            this, "Acceso denegado",
            "No tiene permisos para modificar usuarios."
        );
        return;
    }

    if (idSeleccionado == -1) {
        QMessageBox::warning(
            this, "Seleccion requerida",
            "Seleccione un usuario de la tabla."
        );
        return;
    }

    if (!validarCampos()) {
        return;
    }

    Usuario* actual = gestor->buscarPorId(idSeleccionado);

    if (actual == nullptr) {
        QMessageBox::warning(
            this, "Error",
            "No se encontro el usuario seleccionado."
        );
        return;
    }

    // Crear copia para modificarla
    Usuario modificado = *actual;

    modificado.setNombre(
        ui.txtNombre->text().trimmed().toStdString()
    );

    modificado.setNombreUsuario(
        ui.txtNombreUsuario->text().trimmed().toStdString()
    );

    modificado.setRol(obtenerRolSeleccionado());

    // Cambiar la contrasena solo si se escribio una nueva
    if (!ui.txtClave->text().isEmpty()) {
        modificado.setClaveHash(
            Seguridad::crearHashClave(
                ui.txtClave->text().toStdString())
        );
    }

    if (gestor->modificarUsuario(modificado)) {
        QMessageBox::information(
            this, "Correcto",
            "Usuario modificado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else {
        QMessageBox::warning(
            this, "Error",
            "No se pudo modificar el usuario. "
            "Verifique que el nombre de usuario no este repetido."
        );
    }
}

// Desactivar usuario
void GestionUsuarios::desactivarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr) {
        QMessageBox::warning(
            this, "Acceso denegado",
            "No tiene permisos para desactivar usuarios."
        );
        return;
    }

    if (idSeleccionado == -1) {
        QMessageBox::warning(
            this, "Seleccion requerida",
            "Seleccione un usuario de la tabla."
        );
        return;
    }

    Usuario* usuario = gestor->buscarPorId(idSeleccionado);

    if (usuario == nullptr) {
        QMessageBox::warning(
            this, "Error",
            "No se encontro el usuario."
        );
        return;
    }

    // Evitar desactivar la propia cuenta
    Usuario* sesion = autenticacion->getUsuarioActual();

    if (sesion != nullptr &&
        sesion->getId() == idSeleccionado) {

        QMessageBox::warning(
            this, "Operacion no permitida",
            "No puede desactivar su propia cuenta."
        );
        return;
    }

    QMessageBox::StandardButton respuesta =
        QMessageBox::question(
            this,
            "Confirmar",
            "Desea desactivar el usuario seleccionado?",
            QMessageBox::Yes | QMessageBox::No
        );

    if (respuesta != QMessageBox::Yes) {
        return;
    }

    if (gestor->desactivarUsuario(idSeleccionado)) {
        QMessageBox::information(
            this, "Correcto",
            "Usuario desactivado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else {
        QMessageBox::warning(
            this, "Error",
            "No se pudo desactivar el usuario."
        );
    }
}

// Seleccionar usuario de la tabla
void GestionUsuarios::seleccionarUsuario(
    int fila, int columna)
{
    (void)columna;

    QTableWidgetItem* elemento =
        ui.tblUsuarios->item(fila, 0);

    if (elemento == nullptr || gestor == nullptr) {
        return;
    }

    int id = elemento->text().toInt();

    Usuario* usuario = gestor->buscarPorId(id);

    if (usuario != nullptr) {
        idSeleccionado = id;
        mostrarDatosUsuario(usuario);
    }
}

// Mostrar datos en formulario
void GestionUsuarios::mostrarDatosUsuario(
    Usuario* usuario)
{
    if (usuario == nullptr) {
        return;
    }

    ui.txtNombre->setText(
        QString::fromStdString(usuario->getNombre())
    );

    ui.txtNombreUsuario->setText(
        QString::fromStdString(usuario->getNombreUsuario())
    );

    // Nunca mostrar contrasenas ni hashes
    ui.txtClave->clear();

    if (usuario->getRol() == Rol::Administrador) {
        ui.cmbRol->setCurrentIndex(0);
    }
    else {
        ui.cmbRol->setCurrentIndex(1);
    }
}

// Cargar usuarios en la tabla
void GestionUsuarios::cargarTabla()
{
    ui.tblUsuarios->setRowCount(0);

    if (gestor == nullptr || !verificarPermisos()) {
        return;
    }

    vector<Usuario> usuarios = gestor->listarUsuarios();

    for (const Usuario& usuario : usuarios) {
        int fila = ui.tblUsuarios->rowCount();

        ui.tblUsuarios->insertRow(fila);

        string id = to_string(usuario.getId());

        string rol =
            usuario.getRol() == Rol::Administrador
            ? "Administrador"
            : "Cajero";

        string estado =
            usuario.estaActivo()
            ? "Activo"
            : "Inactivo";

        ui.tblUsuarios->setItem(
            fila, 0,
            new QTableWidgetItem(QString::fromStdString(id))
        );

        ui.tblUsuarios->setItem(
            fila, 1,
            new QTableWidgetItem(
                QString::fromStdString(
                    usuario.getNombre())
            )
        );

        ui.tblUsuarios->setItem(
            fila, 2,
            new QTableWidgetItem(
                QString::fromStdString(
                    usuario.getNombreUsuario())
            )
        );

        ui.tblUsuarios->setItem(
            fila, 3,
            new QTableWidgetItem(QString::fromStdString(rol))
        );

        ui.tblUsuarios->setItem(
            fila, 4,
            new QTableWidgetItem(QString::fromStdString(estado))
        );
    }
}

// Limpiar formulario
void GestionUsuarios::limpiarFormulario()
{
    idSeleccionado = -1;

    ui.txtNombre->clear();
    ui.txtNombreUsuario->clear();
    ui.txtClave->clear();

    ui.cmbRol->setCurrentIndex(0);
    ui.tblUsuarios->clearSelection();

    ui.txtNombre->setFocus();
}

// Cerrar ventana
void GestionUsuarios::cerrarVentana()
{
    close();
}
