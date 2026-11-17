
#include "ProyectoFinalProgra1VSJJ.h"
#include "ArchivoUsuarios.h"
#include "Autenticacion.h"
#include "GestionUsuarios.h"
#include "GestorUsuarios.h"
#include "GestorActividades.h"
#include "Login.h"
#include "Seguridad.h"
#include "Usuario.h"

#include <QtWidgets/QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QDialog>

#include <string>

using namespace std;

// ==========================================
// FUNCION PRINCIPAL DE LA PULPE
// ==========================================

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    // ======================================
    // CONFIGURAR ARCHIVOS DEL SISTEMA
    // ======================================

    // Archivo de usuarios junto al ejecutable
    string rutaUsuarios =
        QDir(QCoreApplication::applicationDirPath())
        .filePath("usuarios.json")
        .toStdString();

    // ======================================
    // CREAR SISTEMA DE USUARIOS
    // ======================================

    ArchivoUsuarios archivo(rutaUsuarios);

    GestorUsuarios gestor(&archivo);

    // Cargar usuarios registrados
    gestor.cargarUsuarios();

    // ======================================
    // CREAR ADMINISTRADOR INICIAL
    // ======================================

    if (gestor.listarUsuarios().empty())
    {
        Usuario admin;

        admin.setId(gestor.generarId());
        admin.setNombre("Administrador");
        admin.setNombreUsuario("admin");

        // Guardar hash de la contrasena
        admin.setClaveHash(
            Seguridad::crearHashClave("admin")
        );

        admin.setRol(Rol::Administrador);
        admin.setActivo(true);

        if (!gestor.agregarUsuario(admin))
        {
            return 1;
        }
    }

    // ======================================
    // CREAR SISTEMA DE AUTENTICACION
    // ======================================

    Autenticacion autenticacion(&gestor);

    // ======================================
    // CREAR REGISTRO DE ACTIVIDADES
    // ======================================

    // Una sola instancia para toda la aplicacion
    GestorActividades gestorActividades;

    // Conectar actividades con autenticacion
    autenticacion.setGestorActividades(
        &gestorActividades
    );

    // ======================================
    // CONECTAR GESTION DE USUARIOS
    // ======================================

    // Permitir que el gestor registre las
    // operaciones realizadas por usuarios.
    gestor.setGestorActividades(
        &gestorActividades
    );

    // Identificar al usuario responsable
    gestor.setAutenticacion(
        &autenticacion
    );

    // ======================================
    // MOSTRAR INICIO DE SESION
    // ======================================

    Login login(&autenticacion);

    if (login.exec() != QDialog::Accepted)
    {
        return 0;
    }

    // ======================================
    // CREAR VENTANA PRINCIPAL
    // ======================================

    ProyectoFinalProgra1VSJJ window(
        &autenticacion,
        &gestor,
        &gestorActividades
    );

    // Mostrar ventana principal
    window.show();

    // ======================================
    // EJECUTAR APLICACION
    // ======================================

    int resultado = app.exec();

    // ======================================
    // REGISTRAR CIERRE DE SESION
    // ======================================

    // Registrar LOGOUT si se cerro directamente
    // la ventana sin utilizar Cerrar Sesion.
    if (autenticacion.haySesionActiva())
    {
        autenticacion.cerrarSesion();
    }

    return resultado;
}

