
#pragma once

#include <QDialog>
#include <QDate>
#include <QString>

#include <vector>

#include "Actividad.h"
#include "GestorActividades.h"

// Declaraciones anticipadas de Qt
class QTableWidget;
class QDateEdit;
class QLineEdit;
class QComboBox;
class QPushButton;
class QLabel;

// ==========================================
// VENTANA DE REGISTRO DE ACTIVIDADES
// LA PULPE
// ==========================================

class VentanaActividades : public QDialog
{
    Q_OBJECT

private:

    // ======================================
    // GESTOR DE ACTIVIDADES
    // ======================================

    // Utilizamos el gestor compartido.
    // Esta ventana no es duena del gestor.
    GestorActividades* gestorActividades;

    // Actividades de la fecha seleccionada
    std::vector<Actividad> actividadesCargadas;

    // ======================================
    // COMPONENTES DE LA VENTANA
    // ======================================

    // Tabla principal
    QTableWidget* tablaActividades;

    // Selector de fecha
    QDateEdit* selectorFecha;

    // Buscar por nombre de usuario
    QLineEdit* txtBuscarUsuario;

    // Filtrar por tipo de actividad
    QComboBox* cmbTipoActividad;

    // Botones
    QPushButton* btnActualizar;
    QPushButton* btnHoy;
    QPushButton* btnCerrar;

    // Informacion de resultados
    QLabel* lblTotalActividades;
    QLabel* lblEstado;

    // ======================================
    // CONFIGURACION VISUAL
    // ======================================

    // Crear los componentes de la ventana
    void configurarInterfaz();

    // Aplicar colores y estilos
    void configurarEstilos();

    // Preparar las columnas de la tabla
    void configurarTabla();

    // ======================================
    // CONSULTA DE ACTIVIDADES
    // ======================================

    // Leer actividades de la fecha seleccionada
    void cargarActividades();

    // Mostrar actividades aplicando filtros
    void aplicarFiltros();

    // Actualizar las opciones de tipos
    void actualizarTiposActividad();

    // Agregar una actividad a la tabla
    void agregarFila(
        const Actividad& actividad
    );

    // Mostrar cantidad de resultados
    void actualizarContador(
        int cantidadMostrada
    );

public:

    // ======================================
    // CONSTRUCTOR
    // ======================================

    explicit VentanaActividades(
        GestorActividades* gestorActividades,
        QWidget* parent = nullptr
    );

    // ======================================
    // DESTRUCTOR
    // ======================================

    ~VentanaActividades();

};
