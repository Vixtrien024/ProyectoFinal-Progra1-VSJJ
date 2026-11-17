
#include "ProyectoFinalProgra1VSJJ.h"

#include "GestionUsuarios.h"
#include "GestionProductos.h"
#include "VentanaVentas.h"
#include "VentanaHistorial.h"
#include "VentanaActividades.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QRect>
#include <QEvent>

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
    btnRegistroActividades(nullptr),
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
    // CONECTAR ACTIVIDADES DE PRODUCTOS
    // ======================================

    gestor.setGestorActividades(
        this->gestorActividades
    );

    gestor.setAutenticacion(
        this->autenticacion
    );

    // ======================================
    // CONECTAR ACTIVIDADES DE VENTAS
    // ======================================

    gestorVentas.setGestorActividades(
        this->gestorActividades
    );

    gestorVentas.setAutenticacion(
        this->autenticacion
    );

    // ======================================
    // REGISTRO DE ACTIVIDADES
    // ======================================

    // Compartir el mismo gestor creado
    // en main.cpp.

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

    // Conectar gestor de productos
    if (pantallaProductos != nullptr)
    {
        pantallaProductos->setGestor(&gestor);
    }

    // Mostrar inicialmente el menu
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
    // CREAR BOTON DE ACTIVIDADES
    // ======================================

    configurarBotonActividades();

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
    // El gestor de actividades pertenece
    // a main.cpp.
    // No se elimina aqui.
}

// ==========================================
// CONFIGURAR BOTON DE ACTIVIDADES
// ==========================================

void ProyectoFinalProgra1VSJJ::configurarBotonActividades()
{
    if (paginaMenu == nullptr)
    {
        return;
    }

    // Evitar crear el boton dos veces
    if (btnRegistroActividades != nullptr)
    {
        return;
    }

    // ======================================
    // CREAR BOTON
    // ======================================

    btnRegistroActividades = new QPushButton(
        "Registro de Actividades",
        paginaMenu
    );

    btnRegistroActividades->setObjectName(
        "btnRegistroActividades"
    );

    btnRegistroActividades->setCursor(
        Qt::PointingHandCursor
    );

    // ======================================
    // ESTILO DEL BOTON
    // ======================================

    btnRegistroActividades->setStyleSheet(R"(
        QPushButton#btnRegistroActividades {
            background-color: #187545;
            color: white;
            border: 1px solid #12613A;
            border-radius: 8px;
            padding: 8px 12px;
            font-family: "Segoe UI";
            font-size: 12px;
            font-weight: bold;
        }

        QPushButton#btnRegistroActividades:hover {
            background-color: #105B34;
        }

        QPushButton#btnRegistroActividades:pressed {
            background-color: #0C4729;
        }

        QPushButton#btnRegistroActividades:disabled {
            background-color: #46554A;
            color: #B0B8B2;
        }
    )");

    // ======================================
    // UBICAR BOTON EN EL MENU
    // ======================================

    // No agregar al layout principal.
    // Asi no se modifican los cuatro
    // botones grandes del menu.

    // Obtener la posicion del boton
    // Cerrar Sesion respecto a pageMenu,
    // aunque este dentro de otro widget.

    QRect posicionCerrar(
        ui.btnCerrarSesion->mapTo(
            paginaMenu,
            QPoint(0, 0)
        ),
        ui.btnCerrarSesion->size()
    );

    int anchoBoton = 205;
    int altoBoton = posicionCerrar.height();

    if (altoBoton < 40)
    {
        altoBoton = 45;
    }

    // Colocar a la izquierda de Cerrar Sesion
    int x = posicionCerrar.x()
        - anchoBoton - 12;

    int y = posicionCerrar.y();

    // Evitar salir por el lado izquierdo
    if (x < 10)
    {
        x = 10;
    }

    btnRegistroActividades->setGeometry(
        x,
        y,
        anchoBoton,
        altoBoton
    );

    // Mostrar por encima del fondo
    btnRegistroActividades->raise();

    // ======================================
    // CONECTAR BOTON
    // ======================================

    connect(
        btnRegistroActividades,
        &QPushButton::clicked,
        this,
        &ProyectoFinalProgra1VSJJ::abrirRegistroActividades
    );

    // Inicialmente oculto.
    // configurarPermisos() decide
    // si debe mostrarse.
    btnRegistroActividades->hide();
}

// ==========================================
// MOSTRAR USUARIO CONECTADO
// ==========================================

void ProyectoFinalProgra1VSJJ::mostrarUsuarioActual()
{
    if (autenticacion == nullptr)
    {
        ui.lblUsuarioActual->setText(
            "Sin sesion"
        );

        ui.lblBienvenido->setText(
            "Acceso no autorizado"
        );

        return;
    }

    Usuario* usuario =
        autenticacion->getUsuarioActual();

    if (usuario == nullptr)
    {
        ui.lblUsuarioActual->setText(
            "Sin sesion"
        );

        ui.lblBienvenido->setText(
            "Acceso no autorizado"
        );

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

    string informacion =
        nombre + " - " + rol;

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

        if (btnRegistroActividades != nullptr)
        {
            btnRegistroActividades->hide();
            btnRegistroActividades->setEnabled(false);
        }

        return;
    }

    // ======================================
    // PERMISO PRODUCTOS
    // ======================================

    ui.btnProductos->setEnabled(
        autenticacion->tienePermiso(
            Permiso::GestionarProductos
        )
    );

    // ======================================
    // PERMISO VENTAS
    // ======================================

    ui.btnVentas->setEnabled(
        autenticacion->tienePermiso(
            Permiso::RegistrarVenta
        )
    );

    // ======================================
    // PERMISO HISTORIAL
    // ======================================

    ui.btnHistorial->setEnabled(
        autenticacion->tienePermiso(
            Permiso::ConsultarHistorial
        )
    );

    // ======================================
    // PERMISO USUARIOS
    // ======================================

    ui.btnUsuarios->setEnabled(
        autenticacion->tienePermiso(
            Permiso::GestionarUsuarios
        )
    );

    // ======================================
    // PERMISO REGISTRO DE ACTIVIDADES
    // ======================================

    Usuario* usuario =
        autenticacion->getUsuarioActual();

    bool esAdministrador =
        usuario != nullptr &&
        usuario->getRol() == Rol::Administrador;

    if (btnRegistroActividades != nullptr)
    {
        btnRegistroActividades->setVisible(
            esAdministrador
        );

        btnRegistroActividades->setEnabled(
            esAdministrador
        );
    }
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
            "No tiene permisos para acceder "
            "a este modulo."
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
    if (!verificarAcceso(
        Permiso::GestionarProductos))
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

    // Actualizar productos
    pantallaProductos->actualizarTabla();

    // Mostrar pagina
    paginas->setCurrentWidget(
        paginaProductos
    );

    // Ajustar ventana
    setMinimumSize(1280, 700);
    resize(1280, 700);
}

// ==========================================
// REGRESAR AL MENU PRINCIPAL
// ==========================================

void ProyectoFinalProgra1VSJJ::mostrarMenu()
{
    if (paginas == nullptr ||
        paginaMenu == nullptr)
    {
        return;
    }

    paginas->setCurrentWidget(
        paginaMenu
    );

    setMinimumSize(0, 0);
    resize(900, 600);
}

// ==========================================
// ABRIR VENTAS Y COBROS
// ==========================================

void ProyectoFinalProgra1VSJJ::abrirVentas()
{
    if (!verificarAcceso(
        Permiso::RegistrarVenta))
    {
        return;
    }

    // Ventana de ventas de Josue
    VentanaVentas ventana(
        &gestorVentas,
        this
    );

    // Cargar productos de Julian
    ventana.setProductos(
        gestor.listar()
    );

    ventana.setWindowTitle(
        "La Pulpe - Ventas y Cobros"
    );

    ventana.exec();

    mostrarMenu();
}

// ==========================================
// ABRIR HISTORIAL Y TICKETS
// ==========================================

void ProyectoFinalProgra1VSJJ::abrirHistorial()
{
    if (!verificarAcceso(
        Permiso::ConsultarHistorial))
    {
        return;
    }

    // Ventana de historial de Moises
    VentanaHistorial* ventana =
        new VentanaHistorial();

    ventana->setWindowTitle(
        "La Pulpe - Historial y Tickets"
    );

    // Liberar memoria al cerrar
    ventana->setAttribute(
        Qt::WA_DeleteOnClose
    );

    // Cerrar historial cuando se
    // destruya la ventana principal
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
    if (!verificarAcceso(
        Permiso::GestionarUsuarios))
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

    // Actualizar usuario y permisos
    mostrarUsuarioActual();
    configurarPermisos();
}

// ==========================================
// ABRIR REGISTRO DE ACTIVIDADES
// ==========================================

void ProyectoFinalProgra1VSJJ::abrirRegistroActividades()
{
    // ======================================
    // VERIFICAR SESION
    // ======================================

    if (autenticacion == nullptr)
    {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "Debe iniciar sesion para acceder "
            "al registro de actividades."
        );

        return;
    }

    Usuario* usuario =
        autenticacion->getUsuarioActual();

    // ======================================
    // VERIFICAR ADMINISTRADOR
    // ======================================

    if (usuario == nullptr ||
        usuario->getRol() != Rol::Administrador)
    {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "Solo los administradores pueden "
            "consultar el registro de actividades."
        );

        configurarPermisos();

        return;
    }

    // ======================================
    // VERIFICAR GESTOR
    // ======================================

    if (gestorActividades == nullptr)
    {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo acceder al gestor "
            "de actividades."
        );

        return;
    }

    // ======================================
    // ABRIR VENTANA DE ACTIVIDADES
    // ======================================

    VentanaActividades ventana(
        gestorActividades,
        this
    );

    ventana.exec();

    // Regresar al menu
    mostrarMenu();
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

    // Registrar LOGOUT
    if (autenticacion != nullptr)
    {
        autenticacion->cerrarSesion();
    }

    close();
}
