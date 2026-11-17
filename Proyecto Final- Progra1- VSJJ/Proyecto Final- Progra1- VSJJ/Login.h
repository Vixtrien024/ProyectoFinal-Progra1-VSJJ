
#pragma once

#include <QDialog>
#include <string>

#include "Autenticacion.h"
#include "ui_Login.h"

using namespace std;

class Login : public QDialog
{
    Q_OBJECT

private:
    Ui::Login ui;

    // Clase encargada de autenticar usuarios
    Autenticacion* autenticacion;

public:
    // Constructor
    Login(Autenticacion* autenticacion,
        QWidget* parent = nullptr);

    // Destructor
    ~Login();

private slots:

    // Se ejecuta al presionar Iniciar Sesion
    void iniciarSesion();

    // Se ejecuta al presionar Salir
    void salir();

private:

    // Verifica que los campos tengan datos
    bool validarCampos();

    // Limpia los campos de la ventana
    void limpiarCampos();
};
