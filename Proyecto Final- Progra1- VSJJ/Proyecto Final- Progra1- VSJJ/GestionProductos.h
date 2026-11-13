#pragma once

#include <QWidget>
#include "GestorProductos.h"

// Clase que Qt genera a partir de GestionProductos.ui (la interfaz hecha en Designer)
namespace Ui {
    class GestionProductos;
}

// Pantalla de productos. Solo muestra datos y recibe clics:
// las reglas las aplica GestorProductos.
class GestionProductos : public QWidget {
    Q_OBJECT

private:
    Ui::GestionProductos* ui;//Da acceso a los controles dibujados en el .ui (ui->nombreDelControl)
    GestorProductos* gestor;//El gestor que ya existe: la pantalla no tiene su propia lista

    void configurarInterfaz();//Ajustes que se hacen por codigo: tabla y conexiones
    int codigoSeleccionado() const;//Codigo de la fila elegida, o -1 si no hay
    Producto leerFormulario() const;//Arma un Producto con lo escrito en el formulario
    bool esInactivo() const;//Lee el comboBox de Estado (0 = Activo, 1 = Inactivo)
    int siguienteCodigo() const;//Sugiere un codigo libre para un producto nuevo

public:
    // Constructor para usar la pantalla como widget promovido dentro de otro .ui.
    // El gestor se asigna despues con setGestor().
    explicit GestionProductos(QWidget* parent = nullptr);

    // Constructor que recibe el gestor directamente
    GestionProductos(GestorProductos* gestor, QWidget* parent = nullptr);

    ~GestionProductos();

    // Asigna el gestor existente y refresca la pantalla
    void setGestor(GestorProductos* gestor);

signals:
    void regresar();//Se emite al presionar "Regresar"; VentanaPrincipal decide que hacer

public slots:
    void agregar();
    void editar();
    void desactivar();
    void actualizarTabla();

private slots:
    void cargarFormulario();//Al elegir una fila, pasa sus datos al formulario
    void limpiarFormulario();//Boton "Nuevo"
    void filtrarTabla(const QString& texto);//Lo que se escribe en txtBuscar oculta las filas que no coinciden

};//fin de la clase
