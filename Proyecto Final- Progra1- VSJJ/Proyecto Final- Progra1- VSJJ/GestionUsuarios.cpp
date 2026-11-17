
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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLayout>
#include <QWidget>
#include <QColor>
#include <QSizePolicy>
#include <QAbstractItemView>

using namespace std;

// ==========================================
// CONSTRUCTOR
// ==========================================

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

    // ======================================
    // CONFIGURAR TABLA
    // ======================================

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

    // ======================================
    // CONECTAR BOTONES EXISTENTES
    // ======================================

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

    connect(ui.tblUsuarios, &QTableWidget::cellClicked,
        this, &GestionUsuarios::seleccionarUsuario);

    // ======================================
    // CREAR BOTONES NUEVOS
    // ======================================

    QPushButton* btnActivar =
        new QPushButton("Activar", this);

    QPushButton* btnEliminar =
        new QPushButton("Eliminar", this);

    btnActivar->setObjectName("btnActivar");
    btnEliminar->setObjectName("btnEliminar");

    // Mantener la misma altura que los originales
    int alturaBotones = ui.btnAgregar->height();

    if (alturaBotones < 35)
    {
        alturaBotones = 42;
    }

    btnActivar->setMinimumHeight(alturaBotones);
    btnEliminar->setMinimumHeight(alturaBotones);

    btnActivar->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Fixed
    );

    btnEliminar->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Fixed
    );

    // ======================================
    // ESTILO VERDE - ACTIVAR
    // ======================================

    btnActivar->setStyleSheet(
        "QPushButton {"
        "background-color: #146C35;"
        "color: white;"
        "border: 1px solid #20D65A;"
        "border-radius: 8px;"
        "padding: 8px 15px;"
        "font-weight: bold;"
        "font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "background-color: #198A45;"
        "}"
        "QPushButton:pressed {"
        "background-color: #0D5027;"
        "}"
        "QPushButton:disabled {"
        "background-color: #333333;"
        "color: #888888;"
        "border: 1px solid #555555;"
        "}"
    );

    // ======================================
    // ESTILO ROJO VINO - ELIMINAR
    // ======================================

    btnEliminar->setStyleSheet(
        "QPushButton {"
        "background-color: #74303C;"
        "color: white;"
        "border: 1px solid #C64C62;"
        "border-radius: 8px;"
        "padding: 8px 15px;"
        "font-weight: bold;"
        "font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "background-color: #913D4C;"
        "}"
        "QPushButton:pressed {"
        "background-color: #55212C;"
        "}"
        "QPushButton:disabled {"
        "background-color: #333333;"
        "color: #888888;"
        "border: 1px solid #555555;"
        "}"
    );

    // ======================================
    // CONECTAR BOTONES NUEVOS
    // ======================================

    connect(btnActivar, &QPushButton::clicked,
        this, &GestionUsuarios::activarUsuario);

    connect(btnEliminar, &QPushButton::clicked,
        this, &GestionUsuarios::eliminarUsuario);

    // ======================================
    // UBICAR BOTONES JUNTO A LOS ACTUALES
    // ======================================

    // Buscar el layout que contiene Desactivar
    QLayout* layoutBotones = nullptr;

    QWidget* contenedor =
        ui.btnDesactivar->parentWidget();

    while (contenedor != nullptr &&
        layoutBotones == nullptr)
    {
        if (contenedor->layout() != nullptr)
        {
            layoutBotones = contenedor->layout();
        }

        contenedor = contenedor->parentWidget();
    }

    if (layoutBotones != nullptr)
    {
        // Nueva fila horizontal, igual que en
        // la distribucion que ya funciona.
        QHBoxLayout* filaNueva = new QHBoxLayout();

        filaNueva->setSpacing(8);
        filaNueva->setContentsMargins(0, 0, 0, 0);

        filaNueva->addWidget(btnActivar);
        filaNueva->addWidget(btnEliminar);

        if (QVBoxLayout* vertical =
            qobject_cast<QVBoxLayout*>(layoutBotones))
        {
            vertical->addLayout(filaNueva);
        }
        else if (QHBoxLayout* horizontal =
            qobject_cast<QHBoxLayout*>(layoutBotones))
        {
            horizontal->addWidget(btnActivar);
            horizontal->addWidget(btnEliminar);

            delete filaNueva;
        }
        else
        {
            layoutBotones->addItem(filaNueva);
        }
    }
    else
    {
        // Alternativa para ventanas sin layout
        int x =
            ui.btnDesactivar->geometry().right() + 10;

        int y =
            ui.btnDesactivar->geometry().top();

        btnActivar->setGeometry(
            x, y, 100, alturaBotones
        );

        btnEliminar->setGeometry(
            x + 110, y, 100, alturaBotones
        );

        btnActivar->show();
        btnEliminar->show();
    }

    // ======================================
    // CARGAR USUARIOS
    // ======================================

    cargarTabla();

    // Bloquear botones sin permisos
    if (!verificarPermisos())
    {
        ui.btnAgregar->setEnabled(false);
        ui.btnModificar->setEnabled(false);
        ui.btnDesactivar->setEnabled(false);

        btnActivar->setEnabled(false);
        btnEliminar->setEnabled(false);
    }
}

// ==========================================
// DESTRUCTOR
// ==========================================

GestionUsuarios::~GestionUsuarios()
{
}

// ==========================================
// VERIFICAR PERMISOS
// ==========================================

bool GestionUsuarios::verificarPermisos()
{
    if (autenticacion == nullptr)
    {
        return false;
    }

    return autenticacion->tienePermiso(
        Permiso::GestionarUsuarios
    );
}

// ==========================================
// VALIDAR CAMPOS
// ==========================================

bool GestionUsuarios::validarCampos()
{
    if (ui.txtNombre->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Datos incompletos",
            "Debe ingresar el nombre."
        );

        return false;
    }

    if (ui.txtNombreUsuario->text().trimmed().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Datos incompletos",
            "Debe ingresar el nombre de usuario."
        );

        return false;
    }

    return true;
}

// ==========================================
// OBTENER ROL
// ==========================================

Rol GestionUsuarios::obtenerRolSeleccionado()
{
    if (ui.cmbRol->currentIndex() == 0)
    {
        return Rol::Administrador;
    }

    return Rol::Cajero;
}

// ==========================================
// AGREGAR USUARIO
// ==========================================

void GestionUsuarios::agregarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr)
    {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "No tiene permisos para registrar usuarios."
        );
        return;
    }

    if (!validarCampos())
    {
        return;
    }

    if (ui.txtClave->text().isEmpty())
    {
        QMessageBox::warning(
            this,
            "Datos incompletos",
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

    if (clave.size() < 4)
    {
        QMessageBox::warning(
            this,
            "Datos incompletos",
            "La contrasena debe tener al menos 4 caracteres."
        );
        return;
    }

    if (gestor->buscarPorNombre(nombreUsuario) != nullptr)
    {
        QMessageBox::warning(
            this,
            "Usuario repetido",
            "Ya existe un usuario con ese nombre de usuario."
        );
        return;
    }

    Usuario nuevo;

    nuevo.setId(gestor->generarId());
    nuevo.setNombre(nombre);
    nuevo.setNombreUsuario(nombreUsuario);
    nuevo.setClaveHash(
        Seguridad::crearHashClave(clave)
    );
    nuevo.setRol(obtenerRolSeleccionado());
    nuevo.setActivo(true);

    if (gestor->agregarUsuario(nuevo))
    {
        QMessageBox::information(
            this,
            "Correcto",
            "Usuario registrado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se pudo registrar el usuario."
        );
    }
}

// ==========================================
// MODIFICAR USUARIO
// ==========================================

void GestionUsuarios::modificarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr)
    {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "No tiene permisos para modificar usuarios."
        );
        return;
    }

    if (idSeleccionado == -1)
    {
        QMessageBox::warning(
            this,
            "Seleccion requerida",
            "Seleccione un usuario de la tabla."
        );
        return;
    }

    if (!validarCampos())
    {
        return;
    }

    Usuario* actual =
        gestor->buscarPorId(idSeleccionado);

    if (actual == nullptr)
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se encontro el usuario seleccionado."
        );
        return;
    }

    Usuario modificado = *actual;

    modificado.setNombre(
        ui.txtNombre->text().trimmed().toStdString()
    );

    modificado.setNombreUsuario(
        ui.txtNombreUsuario->text().trimmed().toStdString()
    );

    modificado.setRol(obtenerRolSeleccionado());

    // Cambiar clave solamente si se escribio una nueva
    if (!ui.txtClave->text().isEmpty())
    {
        modificado.setClaveHash(
            Seguridad::crearHashClave(
                ui.txtClave->text().toStdString()
            )
        );
    }

    if (gestor->modificarUsuario(modificado))
    {
        QMessageBox::information(
            this,
            "Correcto",
            "Usuario modificado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se pudo modificar el usuario. "
            "Verifique que el nombre no este repetido "
            "o que no sea el ultimo administrador activo."
        );
    }
}

// ==========================================
// DESACTIVAR USUARIO
// ==========================================

void GestionUsuarios::desactivarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr)
    {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "No tiene permisos para desactivar usuarios."
        );
        return;
    }

    if (idSeleccionado == -1)
    {
        QMessageBox::warning(
            this,
            "Seleccion requerida",
            "Seleccione un usuario de la tabla."
        );
        return;
    }

    Usuario* usuario =
        gestor->buscarPorId(idSeleccionado);

    if (usuario == nullptr)
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se encontro el usuario."
        );
        return;
    }

    if (!usuario->estaActivo())
    {
        QMessageBox::information(
            this,
            "Informacion",
            "Este usuario ya se encuentra desactivado."
        );
        return;
    }

    Usuario* sesion =
        autenticacion->getUsuarioActual();

    if (sesion != nullptr &&
        sesion->getId() == idSeleccionado)
    {
        QMessageBox::warning(
            this,
            "Operacion no permitida",
            "No puede desactivar su propia cuenta."
        );
        return;
    }

    QMessageBox::StandardButton respuesta =
        QMessageBox::question(
            this,
            "Confirmar desactivacion",
            "Desea desactivar el usuario seleccionado?",
            QMessageBox::Yes | QMessageBox::No
        );

    if (respuesta != QMessageBox::Yes)
    {
        return;
    }

    if (gestor->desactivarUsuario(idSeleccionado))
    {
        QMessageBox::information(
            this,
            "Correcto",
            "Usuario desactivado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se pudo desactivar el usuario. "
            "Compruebe que no sea el ultimo administrador."
        );
    }
}

// ==========================================
// ACTIVAR USUARIO
// ==========================================

void GestionUsuarios::activarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr)
    {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "No tiene permisos para activar usuarios."
        );
        return;
    }

    if (idSeleccionado == -1)
    {
        QMessageBox::warning(
            this,
            "Seleccion requerida",
            "Seleccione un usuario de la tabla."
        );
        return;
    }

    Usuario* usuario =
        gestor->buscarPorId(idSeleccionado);

    if (usuario == nullptr)
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se encontro el usuario."
        );
        return;
    }

    if (usuario->estaActivo())
    {
        QMessageBox::information(
            this,
            "Informacion",
            "Este usuario ya se encuentra activo."
        );
        return;
    }

    QString nombre =
        QString::fromStdString(
            usuario->getNombreUsuario()
        );

    QMessageBox::StandardButton respuesta =
        QMessageBox::question(
            this,
            "Confirmar activacion",
            "Desea activar nuevamente al usuario '" +
            nombre + "'?",
            QMessageBox::Yes | QMessageBox::No
        );

    if (respuesta != QMessageBox::Yes)
    {
        return;
    }

    if (gestor->activarUsuario(idSeleccionado))
    {
        QMessageBox::information(
            this,
            "Correcto",
            "Usuario activado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se pudo activar el usuario."
        );
    }
}

// ==========================================
// ELIMINAR USUARIO
// ==========================================

void GestionUsuarios::eliminarUsuario()
{
    if (!verificarPermisos() || gestor == nullptr)
    {
        QMessageBox::warning(
            this,
            "Acceso denegado",
            "No tiene permisos para eliminar usuarios."
        );
        return;
    }

    if (idSeleccionado == -1)
    {
        QMessageBox::warning(
            this,
            "Seleccion requerida",
            "Seleccione un usuario de la tabla."
        );
        return;
    }

    Usuario* usuario =
        gestor->buscarPorId(idSeleccionado);

    if (usuario == nullptr)
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se encontro el usuario."
        );
        return;
    }

    // No permitir eliminar la propia cuenta
    Usuario* sesion =
        autenticacion->getUsuarioActual();

    if (sesion != nullptr &&
        sesion->getId() == idSeleccionado)
    {
        QMessageBox::warning(
            this,
            "Operacion no permitida",
            "No puede eliminar su propia cuenta."
        );
        return;
    }

    QString nombre =
        QString::fromStdString(
            usuario->getNombreUsuario()
        );

    QMessageBox::StandardButton respuesta =
        QMessageBox::warning(
            this,
            "Confirmar eliminacion",
            "Esta seguro de eliminar definitivamente "
            "al usuario '" + nombre + "'?\n\n"
            "Esta accion no se puede deshacer.",
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

    if (respuesta != QMessageBox::Yes)
    {
        return;
    }

    if (gestor->eliminarUsuario(idSeleccionado))
    {
        QMessageBox::information(
            this,
            "Correcto",
            "Usuario eliminado correctamente."
        );

        cargarTabla();
        limpiarFormulario();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Error",
            "No se pudo eliminar el usuario. "
            "Puede ser el ultimo administrador activo."
        );
    }
}

// ==========================================
// SELECCIONAR USUARIO
// ==========================================

void GestionUsuarios::seleccionarUsuario(
    int fila, int columna)
{
    (void)columna;

    QTableWidgetItem* elemento =
        ui.tblUsuarios->item(fila, 0);

    if (elemento == nullptr || gestor == nullptr)
    {
        return;
    }

    int id = elemento->text().toInt();

    Usuario* usuario =
        gestor->buscarPorId(id);

    if (usuario != nullptr)
    {
        idSeleccionado = id;
        mostrarDatosUsuario(usuario);
    }
}

// ==========================================
// MOSTRAR DATOS DEL USUARIO
// ==========================================

void GestionUsuarios::mostrarDatosUsuario(
    Usuario* usuario)
{
    if (usuario == nullptr)
    {
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

    if (usuario->getRol() == Rol::Administrador)
    {
        ui.cmbRol->setCurrentIndex(0);
    }
    else
    {
        ui.cmbRol->setCurrentIndex(1);
    }
}

// ==========================================
// CARGAR TABLA DE USUARIOS
// ==========================================

void GestionUsuarios::cargarTabla()
{
    ui.tblUsuarios->setRowCount(0);

    if (gestor == nullptr || !verificarPermisos())
    {
        return;
    }

    vector<Usuario> usuarios =
        gestor->listarUsuarios();

    for (const Usuario& usuario : usuarios)
    {
        int fila =
            ui.tblUsuarios->rowCount();

        ui.tblUsuarios->insertRow(fila);

        string id =
            to_string(usuario.getId());

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
            new QTableWidgetItem(
                QString::fromStdString(id)
            )
        );

        ui.tblUsuarios->setItem(
            fila, 1,
            new QTableWidgetItem(
                QString::fromStdString(
                    usuario.getNombre()
                )
            )
        );

        ui.tblUsuarios->setItem(
            fila, 2,
            new QTableWidgetItem(
                QString::fromStdString(
                    usuario.getNombreUsuario()
                )
            )
        );

        ui.tblUsuarios->setItem(
            fila, 3,
            new QTableWidgetItem(
                QString::fromStdString(rol)
            )
        );

        QTableWidgetItem* itemEstado =
            new QTableWidgetItem(
                QString::fromStdString(estado)
            );

        if (usuario.estaActivo())
        {
            itemEstado->setForeground(
                QColor("#198754")
            );
        }
        else
        {
            itemEstado->setForeground(
                QColor("#dc3545")
            );
        }

        ui.tblUsuarios->setItem(
            fila, 4,
            itemEstado
        );
    }
}

// ==========================================
// LIMPIAR FORMULARIO
// ==========================================

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

// ==========================================
// CERRAR VENTANA
// ==========================================

void GestionUsuarios::cerrarVentana()
{
    close();
}
