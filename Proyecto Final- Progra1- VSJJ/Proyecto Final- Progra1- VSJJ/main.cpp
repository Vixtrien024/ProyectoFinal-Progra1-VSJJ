// Main del proyecto final.
// La ventana principal crea el archivo, el gestor y la pantalla de productos.
#include <QApplication>
#include "ProyectoFinalProgra1VSJJ.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);//Inicia Qt (siempre va primero)

    ProyectoFinalProgra1VSJJ ventanaPrincipal;//Crea el gestor y la pantalla de productos
    ventanaPrincipal.show();//Muestra la ventana

    return app.exec();//Mantiene el programa abierto hasta que se cierre la ventana
}//fin de main
