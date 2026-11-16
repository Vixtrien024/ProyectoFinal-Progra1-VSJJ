
#include "VentanaHistorial.h"
#include "ui_VentanaHistorial.h"

#include "TicketTXT.h"
#include "TicketHTML.h"

#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QString>

using namespace std;

// ==========================================
// MENSAJES PERSONALIZADOS DE LA PULPE
// ==========================================

static void mostrarMensaje(
    QWidget* padre,
    QMessageBox::Icon icono,
    const QString& titulo,
    const QString& mensaje)
{
    QMessageBox ventana(padre);

    ventana.setIcon(icono);
    ventana.setWindowTitle(titulo);
    ventana.setText(mensaje);
    ventana.setStandardButtons(QMessageBox::Ok);

    // Estilo exclusivo para esta ventana de mensaje
    ventana.setStyleSheet(R"(
        QMessageBox {
            background-color: #101C18;
        }

        QMessageBox QLabel {
            color: #FFFFFF;
            background-color: transparent;
            font-size: 14px;
            padding: 5px;
        }

        QMessageBox QPushButton {
            background-color: #008C50;
            color: #FFFFFF;
            border: 2px solid #20D875;
            border-radius: 10px;
            padding: 8px 20px;
            font-weight: bold;
            min-width: 60px;
        }

        QMessageBox QPushButton:hover {
            background-color: #00B865;
        }

        QMessageBox QPushButton:pressed {
            background-color: #006B3D;
        }
    )");

    ventana.exec();
}

// ==========================================
// CONSTRUCTOR
// ==========================================

VentanaHistorial::VentanaHistorial(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::VentanaHistorial),
      archivoVentas("ventas.json"),
      historial(&archivoVentas)
{
    ui->setupUi(this);

    setWindowTitle("La Pulpe - Historial y Tickets");
    resize(1250, 800);

    // CONFIGURAR TABLA DE VENTAS

    ui->tablaVentas->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    ui->tablaVentas->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    ui->tablaVentas->setSelectionMode(
        QAbstractItemView::SingleSelection
    );

    ui->tablaVentas->setColumnCount(5);

    ui->tablaVentas->setHorizontalHeaderLabels({
        "Numero",
        "Fecha",
        "Total",
        "Efectivo",
        "Cambio"
    });

    ui->tablaVentas->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    // CONFIGURAR TABLA DE DETALLES

    ui->tablaDetalles->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    ui->tablaDetalles->setColumnCount(4);

    ui->tablaDetalles->setHorizontalHeaderLabels({
        "Producto",
        "Cantidad",
        "Precio",
        "Subtotal"
    });

    ui->tablaDetalles->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    // CONECTAR BOTONES

    connect(
        ui->btnActualizar,
        &QPushButton::clicked,
        this,
        &VentanaHistorial::actualizarTabla
    );

    connect(
        ui->txtBuscarVenta,
        &QLineEdit::returnPressed,
        this,
        &VentanaHistorial::buscarVenta
    );

    connect(
        ui->tablaVentas,
        &QTableWidget::itemSelectionChanged,
        this,
        &VentanaHistorial::mostrarDetalle
    );

    connect(
        ui->btnTicketTXT,
        &QPushButton::clicked,
        this,
        &VentanaHistorial::exportarTXT
    );

    connect(
        ui->btnTicketHTML,
        &QPushButton::clicked,
        this,
        &VentanaHistorial::exportarHTML
    );

    connect(
        ui->btnCerrar,
        &QPushButton::clicked,
        this,
        &QWidget::close
    );

    // CARGAR VENTAS AL ABRIR
    actualizarTabla();
}

// ==========================================
// DESTRUCTOR
// ==========================================

VentanaHistorial::~VentanaHistorial()
{
    delete ui;
}

// ==========================================
// ACTUALIZAR HISTORIAL
// ==========================================

void VentanaHistorial::actualizarTabla()
{
    ventas = historial.listar();

    QSignalBlocker bloqueo(ui->tablaVentas);

    ui->tablaVentas->setRowCount(0);
    ui->tablaDetalles->setRowCount(0);

    ui->lblNumeroVenta->setText("N° Venta: -");
    ui->lblFechaVenta->setText("Fecha: -");
    ui->lblClienteVenta->setText("Cliente: -");
    ui->lblTotalVenta->setText("Total: ₡0");

    for (const Venta& venta : ventas)
    {
        int fila = ui->tablaVentas->rowCount();

        ui->tablaVentas->insertRow(fila);

        ui->tablaVentas->setItem(
            fila, 0,
            new QTableWidgetItem(
                QString::number(venta.getNumero())
            )
        );

        ui->tablaVentas->setItem(
            fila, 1,
            new QTableWidgetItem(
                QString::fromStdString(venta.getFecha())
            )
        );

        ui->tablaVentas->setItem(
            fila, 2,
            new QTableWidgetItem(
                QString::number(venta.getTotal())
            )
        );

        ui->tablaVentas->setItem(
            fila, 3,
            new QTableWidgetItem(
                QString::number(venta.getEfectivo())
            )
        );

        ui->tablaVentas->setItem(
            fila, 4,
            new QTableWidgetItem(
                QString::number(venta.getCambio())
            )
        );
    }
}

// ==========================================
// BUSCAR VENTA
// ==========================================

void VentanaHistorial::buscarVenta()
{
    bool correcto = false;

    int numero = ui->txtBuscarVenta->text()
        .trimmed().toInt(&correcto);

    if (!correcto || numero <= 0)
    {
        mostrarMensaje(
            this,
            QMessageBox::Warning,
            "Busqueda",
            "Ingrese un numero de venta valido."
        );

        return;
    }

    actualizarTabla();

    for (int fila = 0;
         fila < ui->tablaVentas->rowCount();
         fila++)
    {
        QTableWidgetItem* item =
            ui->tablaVentas->item(fila, 0);

        if (item != nullptr &&
            item->text().toInt() == numero)
        {
            ui->tablaVentas->selectRow(fila);
            ui->tablaVentas->scrollToItem(item);
            return;
        }
    }

    mostrarMensaje(
        this,
        QMessageBox::Information,
        "Busqueda",
        "No se encontro la venta indicada."
    );
}

// ==========================================
// MOSTRAR DETALLES DE LA VENTA
// ==========================================

void VentanaHistorial::mostrarDetalle()
{
    ui->tablaDetalles->setRowCount(0);

    int fila = ui->tablaVentas->currentRow();

    if (fila < 0 ||
        fila >= static_cast<int>(ventas.size()))
    {
        ui->lblNumeroVenta->setText("N° Venta: -");
        ui->lblFechaVenta->setText("Fecha: -");
        ui->lblClienteVenta->setText("Cliente: -");
        ui->lblTotalVenta->setText("Total: ₡0");

        return;
    }

    const Venta& venta = ventas[fila];

    ui->lblNumeroVenta->setText(
        QString("N° Venta: %1")
            .arg(venta.getNumero())
    );

    ui->lblFechaVenta->setText(
        QString("Fecha: %1")
            .arg(QString::fromStdString(venta.getFecha()))
    );

    ui->lblClienteVenta->setText(
        "Cliente: Consumidor final"
    );

    ui->lblTotalVenta->setText(
        QString("Total: ₡%1")
            .arg(venta.getTotal())
    );

    vector<DetalleVenta> detalles =
        venta.getDetalles();

    for (const DetalleVenta& detalle : detalles)
    {
        int filaDetalle =
            ui->tablaDetalles->rowCount();

        ui->tablaDetalles->insertRow(filaDetalle);

        ui->tablaDetalles->setItem(
            filaDetalle, 0,
            new QTableWidgetItem(
                QString::fromStdString(
                    detalle.getNombreProducto()
                )
            )
        );

        ui->tablaDetalles->setItem(
            filaDetalle, 1,
            new QTableWidgetItem(
                QString::number(
                    detalle.getCantidad()
                )
            )
        );

        ui->tablaDetalles->setItem(
            filaDetalle, 2,
            new QTableWidgetItem(
                QString::number(
                    detalle.getPrecioUnitario()
                )
            )
        );

        ui->tablaDetalles->setItem(
            filaDetalle, 3,
            new QTableWidgetItem(
                QString::number(
                    detalle.subtotal()
                )
            )
        );
    }
}

// ==========================================
// GENERAR TICKET TXT
// ==========================================

void VentanaHistorial::exportarTXT()
{
    int fila = ui->tablaVentas->currentRow();

    if (fila < 0 ||
        fila >= static_cast<int>(ventas.size()))
    {
        mostrarMensaje(
            this,
            QMessageBox::Warning,
            "Ticket TXT",
            "Primero seleccione una venta."
        );

        return;
    }

    const Venta& venta = ventas[fila];

    TicketTXT generador;

    if (generador.generar(venta))
    {
        mostrarMensaje(
            this,
            QMessageBox::Information,
            "Ticket TXT",
            QString(
                "Ticket de la venta N° %1 generado correctamente."
            ).arg(venta.getNumero())
        );
    }
    else
    {
        mostrarMensaje(
            this,
            QMessageBox::Warning,
            "Ticket TXT",
            "No se pudo generar el ticket TXT."
        );
    }
}

// ==========================================
// GENERAR TICKET HTML
// ==========================================

void VentanaHistorial::exportarHTML()
{
    int fila = ui->tablaVentas->currentRow();

    if (fila < 0 ||
        fila >= static_cast<int>(ventas.size()))
    {
        mostrarMensaje(
            this,
            QMessageBox::Warning,
            "Ticket HTML",
            "Primero seleccione una venta."
        );

        return;
    }

    const Venta& venta = ventas[fila];

    TicketHTML generador;

    if (generador.generar(venta))
    {
        mostrarMensaje(
            this,
            QMessageBox::Information,
            "Ticket HTML",
            QString(
                "Ticket de la venta N° %1 generado correctamente."
            ).arg(venta.getNumero())
        );
    }
    else
    {
        mostrarMensaje(
            this,
            QMessageBox::Warning,
            "Ticket HTML",
            "No se pudo generar el ticket HTML."
        );
    }
}
