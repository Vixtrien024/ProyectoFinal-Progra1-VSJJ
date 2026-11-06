
#include "ProyectoFinalProgra1VSJJ.h"
#include "GestionUsuarios.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLabel>

using namespace std;

// Constructor
ProyectoFinalProgra1VSJJ::ProyectoFinalProgra1VSJJ(
    Autenticacion* autenticacion,
    GestorUsuarios* gestorUsuarios,
    QWidget* parent
) : QMainWindow(parent)
{
    ui.setupUi(this);

    this->autenticacion = autenticacion;
    this->gestorUsuarios = gestorUsuarios;

    // Conectar botones de la ventana principal
    connect(ui.btnProductos, &QPushButton::clicked,
        this, &ProyectoFinalProgra1VSJJ::abrirProductos);

    connect(ui.btnVentas, &QPushButton::clicked,
        this, &ProyectoFinalProgra1VSJJ::abrirVentas);

    connect(ui.btnHistorial, &QPushButton::clicked,
        this, &ProyectoFinalProgra1VSJJ::abrirHistorial);

    connect(ui.btnUsuarios, &QPushButton::clicked,
        this, &ProyectoFinalProgra1VSJJ::abrirUsuarios);

    connect(ui.btnCerrarSesion, &QPushButton::clicked,
        this, &ProyectoFinalProgra1VSJJ::cerrarSesion);

    // Mostrar usuario conectado
    mostrarUsuarioActual();

    // Configurar acceso segun el rol
    configurarPermisos();

    // Ocultar barras que no utilizaremos
    if (menuBar() != nullptr) {
        menuBar()->hide();
    }

    if (statusBar() != nullptr) {
        statusBar()->hide();
    }

    if (ui.mainToolBar != nullptr) {
        ui.mainToolBar->hide();
    }
}

// Destructor
ProyectoFinalProgra1VSJJ::~ProyectoFinalProgra1VSJJ()
{
}

// Mostrar usuario conectado
void ProyectoFinalProgra1VSJJ::mostrarUsuarioActual()
{
    if (autenticacion == nullptr) {
        ui.lblUsuarioActual->setText("Sin sesion");
        ui.lblBienvenido->setText("Acceso no autorizado");
        return;
    }

    Usuario* usuario = autenticacion->getUsuarioActual();

    if (usuario == nullptr) {
        ui.lblUsuarioActual->setText("Sin sesion");
        ui.lblBienvenido->setText("Acceso no autorizado");
        return;
    }

    string nombre = usuario->getNombre();

    string rol;

    if (usuario->getRol() == Rol::Administrador) {
        rol = "Administrador";
    }
    else {
        rol = "Cajero";
    }

    string informacion = nombre + " - " + rol;

    ui.lblUsuarioActual->setText(informacion.c_str());

    string bienvenida = "Bienvenido a La Pulpe, " + nombre;

    ui.lblBienvenido->setText(bienvenida.c_str());
}

// Configurar botones segun permisos
void ProyectoFinalProgra1VSJJ::configurarPermisos()
{
    if (autenticacion == nullptr) {
        ui.btnProductos->setEnabled(false);
        ui.btnVentas->setEnabled(false);
        ui.btnHistorial->setEnabled(false);
        ui.btnUsuarios->setEnabled(false);
        return;
    }

    ui.btnProductos->setEnabled(
        autenticacion->tienePermiso(
            Permiso::GestionarProductos
        )
    );

    ui.btnVentas->setEnabled(
        autenticacion->tienePermiso(
            Permiso::RegistrarVenta
        )
    );

    ui.btnHistorial->setEnabled(
        autenticacion->tienePermiso(
            Permiso::ConsultarHistorial
        )
    );

    ui.btnUsuarios->setEnabled(
        autenticacion->tienePermiso(
            Permiso::GestionarUsuarios
        )
    );
}

// Verificar acceso
bool ProyectoFinalProgra1VSJJ::verificarAcceso(
    Permiso permiso)
{
    if (autenticacion == nullptr ||
        !autenticacion->tienePermiso(permiso)) {

        QMessageBox::warning(
            this,
            "Acceso denegado",
            "No tiene permisos para acceder a este modulo."
        );

        configurarPermisos();
        return false;
    }

    return true;
}

// Abrir productos e inventario
void ProyectoFinalProgra1VSJJ::abrirProductos()
{
    if (!verificarAcceso(Permiso::GestionarProductos)) {
        return;
    }

    // Pendiente de conectar con el modulo de Julian
    QMessageBox::information(
        this,
        "Productos e Inventario",
        "El modulo de productos se conectara "
        "cuando este disponible la ventana correspondiente."
    );
}

// Abrir ventas y cobros
void ProyectoFinalProgra1VSJJ::abrirVentas()
{
    if (!verificarAcceso(Permiso::RegistrarVenta)) {
        return;
    }

    // Pendiente de conectar con el modulo de Josue
    QMessageBox::information(
        this,
        "Ventas y Cobros",
        "El modulo de ventas se conectara "
        "cuando este disponible la ventana correspondiente."
    );
}

// Abrir historial y tickets
void ProyectoFinalProgra1VSJJ::abrirHistorial()
{
    if (!verificarAcceso(Permiso::ConsultarHistorial)) {
        return;
    }

    // Pendiente de conectar con el modulo de Moises
    QMessageBox::information(
        this,
        "Historial y Tickets",
        "El modulo de historial se conectara "
        "cuando este disponible la ventana correspondiente."
    );
}

// Abrir gestion de usuarios
void ProyectoFinalProgra1VSJJ::abrirUsuarios()
{
    if (!verificarAcceso(Permiso::GestionarUsuarios)) {
        return;
    }

    if (gestorUsuarios == nullptr) {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo acceder al gestor de usuarios."
        );
        return;
    }

    // Abrir ventana de gestion de usuarios
    GestionUsuarios ventana(gestorUsuarios, autenticacion, this);

    ventana.exec();

    // Actualizar datos y permisos al regresar
    mostrarUsuarioActual();
    configurarPermisos();
}

// Cerrar sesion
void ProyectoFinalProgra1VSJJ::cerrarSesion()
{
    int respuesta = QMessageBox::question(
        this,
        "Cerrar sesion",
        "Desea cerrar la sesion actual?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (respuesta != QMessageBox::Yes) {
        return;
    }

    if (autenticacion != nullptr) {
        autenticacion->cerrarSesion();
    }

    // Cerrar ventana principal
    close();
}
