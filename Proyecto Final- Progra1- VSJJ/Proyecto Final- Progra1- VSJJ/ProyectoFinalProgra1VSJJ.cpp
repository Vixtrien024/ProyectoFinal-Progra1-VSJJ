#include "ProyectoFinalProgra1VSJJ.h"

// Constructor: crea el archivo, el gestor y la pantalla de productos
ProyectoFinalProgra1VSJJ::ProyectoFinalProgra1VSJJ(QWidget *parent)
    : QMainWindow(parent),
    archivo("productos.txt"),//Aqui se guardan los productos
    gestor(&archivo),//Carga lo que haya guardado en el archivo
    inventario(&gestor),//Trabaja sobre la misma coleccion del gestor
    pantallaProductos(nullptr) {

    ui.setupUi(this);

    // La pantalla dibujada en GestionProductos.ui se muestra dentro de esta ventana
    pantallaProductos = new GestionProductos(&gestor, this);
    pantallaProductos->setMinimumSize(1280, 700);//La interfaz no se deja achicar
    setCentralWidget(pantallaProductos);

    // "Regresar" cierra la aplicacion (cuando exista un menu principal, aqui se abrira)
    connect(pantallaProductos, &GestionProductos::regresar, this, &QMainWindow::close);
}//fin del constructor


// Destructor: la pantalla es hija de esta ventana, Qt la libera solo
ProyectoFinalProgra1VSJJ::~ProyectoFinalProgra1VSJJ()
{
}//fin del destructor
