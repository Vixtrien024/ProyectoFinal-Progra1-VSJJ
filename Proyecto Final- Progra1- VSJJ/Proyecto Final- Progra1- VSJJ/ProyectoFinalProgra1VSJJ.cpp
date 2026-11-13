
#include "ProyectoFinalProgra1VSJJ.h"
#include "GestionUsuarios.h"
#include "GestionProductos.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLabel>

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

    // --- Integrar la pantalla de productos dentro de la ventana principal ---
    // El centralWidget del .ui es un QStackedWidget: la primera pagina es el
    // menu (pageMenu) y la segunda (pageProductos) contiene el widget promovido
    // GestionProductos, que representa GestionProductos.ui.
    paginas = ui.centralWidget;
    paginaMenu = ui.pageMenu;
    paginaProductos = ui.pageProductos;
    pantallaProductos = ui.pantallaProductos;

    // El widget promovido se creo sin gestor; se le asigna el de esta ventana
    pantallaProductos->setGestor(&gestor);

    paginas->setCurrentWidget(paginaMenu);

    // El boton "Regresar" de la pantalla de productos vuelve al menu
    connect(pantallaProductos, &GestionProductos::regresar,
        this, &ProyectoFinalProgra1VSJJ::mostrarMenu);

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
    // Verificar permisos del usuario
    if (!verificarAcceso(Permiso::GestionarProductos)) {
        return;
    }

    // Comprobar que la interfaz esté disponible
    if (paginas == nullptr ||
        paginaProductos == nullptr ||
        pantallaProductos == nullptr) {

        QMessageBox::critical(
            this,
            "Error",
            "No se pudo abrir la ventana de Productos e Inventario."
        );

        return;
    }

    // Actualizar productos registrados
    pantallaProductos->actualizarTabla();

    // Mostrar la interfaz de Julian
    paginas->setCurrentWidget(paginaProductos);

    // Ajustar el tamaño de la ventana
    setMinimumSize(1280, 700);
    resize(1280, 700);
}


// Volver al menu principal desde una pantalla de modulo
void ProyectoFinalProgra1VSJJ::mostrarMenu()
{
    paginas->setCurrentWidget(paginaMenu);

    // El menu fue disenado a 900x600
    setMinimumSize(0, 0);
    resize(900, 600);
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
