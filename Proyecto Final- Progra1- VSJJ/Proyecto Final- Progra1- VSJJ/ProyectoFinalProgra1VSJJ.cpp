
#include "ProyectoFinalProgra1VSJJ.h"

#include "GestionUsuarios.h"
#include "GestionProductos.h"
#include "VentanaVentas.h"
#include "VentanaHistorial.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QString>

using namespace std;

// Constructor
ProyectoFinalProgra1VSJJ::ProyectoFinalProgra1VSJJ(
    Autenticacion* autenticacion,
    GestorUsuarios* gestorUsuarios,
    QWidget* parent
) : QMainWindow(parent),
    archivo("productos.txt"),
    gestor(&archivo),
    inventario(&gestor)
{
    ui.setupUi(this);

    this->autenticacion = autenticacion;
    this->gestorUsuarios = gestorUsuarios;

    // Configurar las paginas de la ventana principal
    paginas = ui.centralWidget;
    paginaMenu = ui.pageMenu;
    paginaProductos = ui.pageProductos;
    pantallaProductos = ui.pantallaProductos;

    // Conectar el gestor de productos
    pantallaProductos->setGestor(&gestor);

    // Mostrar inicialmente el menu principal
    paginas->setCurrentWidget(paginaMenu);

    // Boton regresar de productos
    connect(
        pantallaProductos,
        &GestionProductos::regresar,
        this,
        &ProyectoFinalProgra1VSJJ::mostrarMenu
    );

    // Boton Productos
    connect(
        ui.btnProductos,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirProductos
    );

    // Boton Ventas
    connect(
        ui.btnVentas,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirVentas
    );

    // Boton Historial
    connect(
        ui.btnHistorial,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirHistorial
    );

    // Boton Usuarios
    connect(
        ui.btnUsuarios,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirUsuarios
    );

    // Boton Cerrar Sesion
    connect(
        ui.btnCerrarSesion,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::cerrarSesion
    );

    mostrarUsuarioActual();
    configurarPermisos();

    // Ocultar barras innecesarias
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

    ui.lblUsuarioActual->setText(
        QString::fromStdString(informacion)
    );

    string bienvenida = "Bienvenido a La Pulpe, " + nombre;

    ui.lblBienvenido->setText(
        QString::fromStdString(bienvenida)
    );
}

// Configurar permisos
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

// Verificar permisos
bool ProyectoFinalProgra1VSJJ::verificarAcceso(
    Permiso permiso
)
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

// Abrir Productos e Inventario
void ProyectoFinalProgra1VSJJ::abrirProductos()
{
    if (!verificarAcceso(Permiso::GestionarProductos)) {
        return;
    }

    if (paginas == nullptr ||
        paginaProductos == nullptr ||
        pantallaProductos == nullptr) {

        QMessageBox::critical(
            this,
            "Error",
            "No se pudo abrir Productos e Inventario."
        );

        return;
    }

    pantallaProductos->actualizarTabla();

    paginas->setCurrentWidget(paginaProductos);

    setMinimumSize(1280, 700);
    resize(1280, 700);
}

// Regresar al menu principal
void ProyectoFinalProgra1VSJJ::mostrarMenu()
{
    if (paginas == nullptr || paginaMenu == nullptr) {
        return;
    }

    paginas->setCurrentWidget(paginaMenu);

    setMinimumSize(0, 0);
    resize(900, 600);
}

// Abrir Ventas y Cobros
void ProyectoFinalProgra1VSJJ::abrirVentas()
{
    if (!verificarAcceso(Permiso::RegistrarVenta)) {
        return;
    }

    // Crear la ventana de Josue
    VentanaVentas ventana(&gestorVentas, this);

    // Cargar los productos registrados por Julian
    ventana.setProductos(gestor.listar());

    ventana.setWindowTitle(
        "La Pulpe - Ventas y Cobros"
    );

    // Abrir la ventana de ventas
    ventana.exec();

    // Al cerrar, mostrar el menu
    mostrarMenu();
}

// Abrir Historial y Tickets
void ProyectoFinalProgra1VSJJ::abrirHistorial()
{
    if (!verificarAcceso(Permiso::ConsultarHistorial)) {
        return;
    }

    // Crear la ventana de Moises
    VentanaHistorial* ventana = new VentanaHistorial();

    ventana->setWindowTitle(
        "La Pulpe - Historial y Tickets"
    );

    // Liberar la memoria cuando se cierre
    ventana->setAttribute(Qt::WA_DeleteOnClose);

    // Cerrar esta ventana cuando se cierre
    // la ventana principal
    connect(
        this,
        &QObject::destroyed,
        ventana,
        &QWidget::close
    );

    // Mostrar historial
    ventana->show();
}

// Abrir Gestion de Usuarios
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

    GestionUsuarios ventana(
        gestorUsuarios,
        autenticacion,
        this
    );

    ventana.exec();

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

    close();
}
