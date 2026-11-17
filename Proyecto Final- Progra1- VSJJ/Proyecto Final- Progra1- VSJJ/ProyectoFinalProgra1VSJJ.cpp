
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

// ==========================================
// CONSTRUCTOR
// ==========================================

ProyectoFinalProgra1VSJJ::ProyectoFinalProgra1VSJJ(
    Autenticacion* autenticacion,
    GestorUsuarios* gestorUsuarios,
    GestorActividades* gestorActividades,
    QWidget* parent
) : QMainWindow(parent),
    autenticacion(autenticacion),
    gestorUsuarios(gestorUsuarios),
    gestorActividades(gestorActividades),
    archivo("productos.txt"),
    gestor(&archivo),
    inventario(&gestor),
    paginas(nullptr),
    paginaMenu(nullptr),
    paginaProductos(nullptr),
    pantallaProductos(nullptr)
{
    ui.setupUi(this);

    // ======================================
    // CONECTAR VENTAS CON INVENTARIO
    // ======================================

    gestorVentas.setInventario(&inventario);

    // ======================================
    // REGISTRO DE ACTIVIDADES
    // ======================================

    // Utilizar el mismo gestor creado en main.cpp.
    // No se crea una segunda instancia.
    if (this->autenticacion != nullptr)
    {
        this->autenticacion->setGestorActividades(
            this->gestorActividades
        );
    }

    // ======================================
    // CONFIGURAR PAGINAS
    // ======================================

    paginas = ui.centralWidget;
    paginaMenu = ui.pageMenu;
    paginaProductos = ui.pageProductos;
    pantallaProductos = ui.pantallaProductos;

    // Conectar el gestor de productos
    if (pantallaProductos != nullptr)
    {
        pantallaProductos->setGestor(&gestor);
    }

    // Mostrar inicialmente el menu principal
    if (paginas != nullptr && paginaMenu != nullptr)
    {
        paginas->setCurrentWidget(paginaMenu);
    }

    // ======================================
    // BOTON REGRESAR DE PRODUCTOS
    // ======================================

    if (pantallaProductos != nullptr)
    {
        connect(
            pantallaProductos,
            &GestionProductos::regresar,
            this,
            &ProyectoFinalProgra1VSJJ::mostrarMenu
        );
    }

    // ======================================
    // BOTON PRODUCTOS
    // ======================================

    connect(
        ui.btnProductos,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirProductos
    );

    // ======================================
    // BOTON VENTAS
    // ======================================

    connect(
        ui.btnVentas,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirVentas
    );

    // ======================================
    // BOTON HISTORIAL
    // ======================================

    connect(
        ui.btnHistorial,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirHistorial
    );

    // ======================================
    // BOTON USUARIOS
    // ======================================

    connect(
        ui.btnUsuarios,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirUsuarios
    );

    // ======================================
    // BOTON CERRAR SESION
    // ======================================

    connect(
        ui.btnCerrarSesion,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::cerrarSesion
    );

    // ======================================
    // MOSTRAR USUARIO Y PERMISOS
    // ======================================

    mostrarUsuarioActual();
    configurarPermisos();

    // ======================================
    // OCULTAR BARRAS INNECESARIAS
    // ======================================

    if (menuBar() != nullptr)
    {
        menuBar()->hide();
    }

    if (statusBar() != nullptr)
    {
        statusBar()->hide();
    }

    if (ui.mainToolBar != nullptr)
    {
        ui.mainToolBar->hide();
    }
}

// ==========================================
// DESTRUCTOR
// ==========================================

ProyectoFinalProgra1VSJJ::~ProyectoFinalProgra1VSJJ()
{
    // El gestor de actividades pertenece a main.cpp.
    // No se elimina ni se desconecta aqui.
}

// ==========================================
// MOSTRAR USUARIO CONECTADO
// ==========================================

void ProyectoFinalProgra1VSJJ::mostrarUsuarioActual()
{
    if (autenticacion == nullptr)
    {
        ui.lblUsuarioActual->setText("Sin sesion");
        ui.lblBienvenido->setText("Acceso no autorizado");
        return;
    }

    Usuario* usuario = autenticacion->getUsuarioActual();

    if (usuario == nullptr)
    {
        ui.lblUsuarioActual->setText("Sin sesion");
        ui.lblBienvenido->setText("Acceso no autorizado");
        return;
    }

    string nombre = usuario->getNombre();
    string rol;

    if (usuario->getRol() == Rol::Administrador)
    {
        rol = "Administrador";
    }
    else
    {
        rol = "Cajero";
    }

    string informacion = nombre + " - " + rol;

    ui.lblUsuarioActual->setText(
        QString::fromStdString(informacion)
    );

    string bienvenida =
        "Bienvenido a La Pulpe, " + nombre;

    ui.lblBienvenido->setText(
        QString::fromStdString(bienvenida)
    );
}

// ==========================================
// CONFIGURAR PERMISOS
// ==========================================

void ProyectoFinalProgra1VSJJ::configurarPermisos()
{
    if (autenticacion == nullptr)
    {
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

// ==========================================
// VERIFICAR PERMISOS
// ==========================================

bool ProyectoFinalProgra1VSJJ::verificarAcceso(
    Permiso permiso
)
{
    if (autenticacion == nullptr ||
        !autenticacion->tienePermiso(permiso))
    {
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

// ==========================================
// ABRIR PRODUCTOS E INVENTARIO
// ==========================================

void ProyectoFinalProgra1VSJJ::abrirProductos()
{
    if (!verificarAcceso(Permiso::GestionarProductos))
    {
        return;
    }

    if (paginas == nullptr ||
        paginaProductos == nullptr ||
        pantallaProductos == nullptr)
    {
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

// ==========================================
// REGRESAR AL MENU PRINCIPAL
// ==========================================

void ProyectoFinalProgra1VSJJ::mostrarMenu()
{
    if (paginas == nullptr || paginaMenu == nullptr)
    {
        return;
    }

    paginas->setCurrentWidget(paginaMenu);

    setMinimumSize(0, 0);
    resize(900, 600);
}

// ==========================================
// ABRIR VENTAS Y COBROS
// ==========================================

void ProyectoFinalProgra1VSJJ::abrirVentas()
{
    if (!verificarAcceso(Permiso::RegistrarVenta))
    {
        return;
    }

    // Crear ventana de ventas de Josue
    VentanaVentas ventana(&gestorVentas, this);

    // Cargar productos del inventario de Julian
    ventana.setProductos(gestor.listar());

    ventana.setWindowTitle(
        "La Pulpe - Ventas y Cobros"
    );

    // Mostrar ventana de ventas
    ventana.exec();

    // Regresar al menu principal
    mostrarMenu();
}

// ==========================================
// ABRIR HISTORIAL Y TICKETS
// ==========================================

void ProyectoFinalProgra1VSJJ::abrirHistorial()
{
    if (!verificarAcceso(Permiso::ConsultarHistorial))
    {
        return;
    }

    // Crear ventana de historial de Moises
    VentanaHistorial* ventana = new VentanaHistorial();

    ventana->setWindowTitle(
        "La Pulpe - Historial y Tickets"
    );

    // Liberar memoria al cerrar
    ventana->setAttribute(Qt::WA_DeleteOnClose);

    // Cerrar historial al destruir la ventana principal
    connect(
        this,
        &QObject::destroyed,
        ventana,
        &QWidget::close
    );

    ventana->show();
}

// ==========================================
// ABRIR GESTION DE USUARIOS
// ==========================================

void ProyectoFinalProgra1VSJJ::abrirUsuarios()
{
    if (!verificarAcceso(Permiso::GestionarUsuarios))
    {
        return;
    }

    if (gestorUsuarios == nullptr)
    {
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

// ==========================================
// CERRAR SESION
// ==========================================

void ProyectoFinalProgra1VSJJ::cerrarSesion()
{
    int respuesta = QMessageBox::question(
        this,
        "Cerrar sesion",
        "Desea cerrar la sesion actual?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (respuesta != QMessageBox::Yes)
    {
        return;
    }

    // Registrar LOGOUT mediante Autenticacion
    if (autenticacion != nullptr)
    {
        autenticacion->cerrarSesion();
    }

    close();
}
