
#include "VentanaHistorial.h"
#include "ui_VentanaHistorial.h"
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>

VentanaHistorial::VentanaHistorial(QWidget* parent)
    : QWidget(parent),
    ui(new Ui::VentanaHistorial)
{
    ui->setupUi(this);
    ui->layoutTablas->setStretch(0, 2);
    ui->layoutTablas->setStretch(1, 1);
  
    setWindowTitle("La Pulpe - Historial y Tickets");
    resize(1100, 700);

    // Configurar las tablas.
    ui->tablaVentas->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    ui->tablaVentas->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    ui->tablaVentas->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    ui->tablaDetalles->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    // Botón Actualizar.
    connect(ui->btnActualizar, &QPushButton::clicked,
        this, &VentanaHistorial::actualizarTabla);

    // Buscar al presionar Enter.
    connect(ui->txtBuscarVenta, &QLineEdit::returnPressed,
        this, &VentanaHistorial::buscarVenta);

    // Mostrar detalle al seleccionar una venta.
    connect(ui->tablaVentas, &QTableWidget::itemSelectionChanged,
        this, &VentanaHistorial::mostrarDetalle);

    // Generar tickets.
    connect(ui->btnTicketTXT, &QPushButton::clicked,
        this, &VentanaHistorial::exportarTXT);

    connect(ui->btnTicketHTML, &QPushButton::clicked,
        this, &VentanaHistorial::exportarHTML);

    // Cerrar ventana.
    connect(ui->btnCerrar, &QPushButton::clicked,
        this, &QWidget::close);
}

VentanaHistorial::~VentanaHistorial()
{
    delete ui;
}

void VentanaHistorial::actualizarTabla()
{
    // Pendiente: cargar ventas reales.
}

void VentanaHistorial::buscarVenta()
{
    // Pendiente: buscar una venta por número.
}

void VentanaHistorial::mostrarDetalle()
{
    // Pendiente: mostrar productos de la venta.
}

void VentanaHistorial::exportarTXT()
{
    // Pendiente: conectar TicketTXT.
}

void VentanaHistorial::exportarHTML()
{
    // Pendiente: conectar TicketHTML.
}
