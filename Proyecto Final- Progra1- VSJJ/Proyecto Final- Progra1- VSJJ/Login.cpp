
#include "Login.h"

#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>

using namespace std;

// Constructor
Login::Login(Autenticacion* autenticacion, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    this->autenticacion = autenticacion;

    // Ocultar la contraseña
    ui.txtClave->setEchoMode(QLineEdit::Password);

    // Conectar los botones
    connect(ui.btnIngresar, &QPushButton::clicked,
        this, &Login::iniciarSesion);

    connect(ui.btnSalir, &QPushButton::clicked,
        this, &Login::salir);
}

// Destructor
Login::~Login()
{
}

// Validar campos
bool Login::validarCampos()
{
    if (ui.txtUsuario->text().isEmpty()) {
        QMessageBox::warning(
            this,
            "Campo obligatorio",
            "Debe ingresar el nombre de usuario."
        );

        ui.txtUsuario->setFocus();
        return false;
    }

    if (ui.txtClave->text().isEmpty()) {
        QMessageBox::warning(
            this,
            "Campo obligatorio",
            "Debe ingresar la contraseña."
        );

        ui.txtClave->setFocus();
        return false;
    }

    return true;
}

// Iniciar sesion
void Login::iniciarSesion()
{
    if (!validarCampos()) {
        return;
    }

    if (autenticacion == nullptr) {
        QMessageBox::critical(
            this,
            "Error",
            "No se pudo acceder al sistema de autenticacion."
        );

        return;
    }

    // Obtener los datos ingresados
    string nombreUsuario =
        ui.txtUsuario->text().toStdString();

    string clave =
        ui.txtClave->text().toStdString();

    // Verificar credenciales
    bool accesoPermitido =
        autenticacion->iniciarSesion(nombreUsuario, clave);

    if (accesoPermitido) {

        QMessageBox::information(
            this,
            "Bienvenido",
            "Inicio de sesion exitoso."
        );

        // Cerrar el dialogo indicando que fue aceptado
        accept();
    }
    else {

        QMessageBox::warning(
            this,
            "Acceso denegado",
            "Usuario o contraseña incorrectos, o cuenta inactiva."
        );

        // Limpiar solamente la contraseña
        ui.txtClave->clear();
        ui.txtClave->setFocus();
    }
}

// Cerrar ventana
void Login::salir()
{
    reject();
}

// Limpiar formulario
void Login::limpiarCampos()
{
    ui.txtUsuario->clear();
    ui.txtClave->clear();

    ui.txtUsuario->setFocus();
}
