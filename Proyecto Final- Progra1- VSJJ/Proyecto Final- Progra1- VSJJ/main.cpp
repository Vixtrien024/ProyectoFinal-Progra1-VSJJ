
#include "ProyectoFinalProgra1VSJJ.h"
#include "ArchivoUsuarios.h"
#include "Autenticacion.h"
#include "GestionUsuarios.h"
#include "GestorUsuarios.h"
#include "Login.h"
#include "Seguridad.h"
#include "Usuario.h"

#include <QtWidgets/QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QDialog>

#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // Archivo de usuarios junto al ejecutable
    string rutaUsuarios =
        QDir(QCoreApplication::applicationDirPath())
        .filePath("usuarios.json")
        .toStdString();

    // Crear sistema de almacenamiento
    ArchivoUsuarios archivo(rutaUsuarios);

    // Crear gestor de usuarios
    GestorUsuarios gestor(&archivo);

    // Cargar usuarios registrados
    gestor.cargarUsuarios();

    // Crear administrador inicial si no hay usuarios
    if (gestor.listarUsuarios().empty()) {

        Usuario admin;

        admin.setId(gestor.generarId());
        admin.setNombre("Administrador");
        admin.setNombreUsuario("admin");

        // Guardar hash, no contrasena en texto plano
        admin.setClaveHash(
            Seguridad::crearHashClave("admin")
        );

        admin.setRol(Rol::Administrador);
        admin.setActivo(true);

        if (!gestor.agregarUsuario(admin)) {
            return 1;
        }
    }

    // Crear sistema de autenticacion
    Autenticacion autenticacion(&gestor);

    // Mostrar ventana de inicio de sesion
    Login login(&autenticacion);

    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    // Crear ventana principal con los objetos necesarios
    ProyectoFinalProgra1VSJJ window(
        &autenticacion,
        &gestor
    );

    // Mostrar ventana principal
    window.show();

    // Ejecutar aplicacion
    return app.exec();
}
