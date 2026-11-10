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
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Archivo de usuarios junto al ejecutable
    QString rutaUsuarios =
        QDir(QCoreApplication::applicationDirPath())
        .filePath("usuarios.json");

    ArchivoUsuarios archivo(rutaUsuarios.toStdString());

    GestorUsuarios gestor(&archivo);
    gestor.cargarUsuarios();

    // Crear administrador inicial si no hay usuarios
    if (gestor.listarUsuarios().empty()) {

        Usuario admin;

        admin.setId(gestor.generarId());
        admin.setNombre("Administrador");
        admin.setNombreUsuario("admin");
        admin.setClaveHash(
            Seguridad::crearHashClave("admin"));
        admin.setRol(Rol::Administrador);
        admin.setActivo(true);

        gestor.agregarUsuario(admin);
    }

    // Autenticacion y ventana de inicio de sesion
    Autenticacion autenticacion(&gestor);

    Login login(&autenticacion);

    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    ProyectoFinalProgra1VSJJ window;
    window.show();

    // Abrir gestion de usuarios si tiene permiso
    if (autenticacion.tienePermiso(
        Permiso::GestionarUsuarios)) {

        GestionUsuarios gestion(&gestor, &autenticacion);
        gestion.exec();
    }

    return app.exec();
}
