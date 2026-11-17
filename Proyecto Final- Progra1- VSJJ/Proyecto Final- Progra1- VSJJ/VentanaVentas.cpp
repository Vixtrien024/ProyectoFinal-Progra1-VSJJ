
#include "VentanaVentas.h"
#include "TicketHTML.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QFile>
#include <QTextDocument>
#include <QPrinter>
#include <QPrintDialog>
#include <QAbstractButton>

#include <vector>
#include <filesystem>
#include <string>

using namespace std;

// ==========================================
// CONSTRUCTOR DE LA VENTANA DE VENTAS
// ==========================================

VentanaVentas::VentanaVentas(
    GestorVentas* gestor,
    QWidget* parent
) : QDialog(parent)
{
    ui.setupUi(this);

    this->gestor = gestor;

    // Las columnas ocupan todo el ancho
    ui.tablaCarrito->horizontalHeader()
        ->setSectionResizeMode(
            QHeaderView::Stretch
        );

    // Ocultar los numeros de las filas
    ui.tablaCarrito->verticalHeader()
        ->setVisible(false);

    // ======================================
    // CONECTAR BOTONES
    // ======================================

    connect(
        ui.btnAgregar,
        &QPushButton::clicked,
        this,
        &VentanaVentas::agregarProducto
    );

    connect(
        ui.btnQuitar,
        &QPushButton::clicked,
        this,
        &VentanaVentas::quitarProducto
    );

    connect(
        ui.btnCobrar,
        &QPushButton::clicked,
        this,
        &VentanaVentas::cobrar
    );

    connect(
        ui.btnRegresar,
        &QPushButton::clicked,
        this,
        &VentanaVentas::close
    );

    actualizarTotal();
}

// ==========================================
// CARGAR PRODUCTOS DISPONIBLES
// ==========================================

void VentanaVentas::setProductos(
    vector<Producto> productos
)
{
    this->productos = productos;

    ui.cmbProducto->clear();

    for (int i = 0; i < (int)productos.size(); i++)
    {
        ui.cmbProducto->addItem(
            QString::fromStdString(
                productos[i].getNombre()
            )
        );
    }
}

// ==========================================
// AGREGAR PRODUCTO AL CARRITO
// ==========================================

void VentanaVentas::agregarProducto()
{
    int posicion =
        ui.cmbProducto->currentIndex();

    if (posicion < 0)
    {
        ui.lblMensaje->setText(
            "No hay productos disponibles"
        );

        return;
    }

    bool seAgrego = carrito.agregar(
        productos[posicion],
        ui.spnCantidad->value()
    );

    if (!seAgrego)
    {
        ui.lblMensaje->setText(
            "No se pudo agregar el producto"
        );

        return;
    }

    ui.spnCantidad->setValue(1);
    ui.lblMensaje->setText("");

    actualizarTotal();
}

// ==========================================
// QUITAR PRODUCTO DEL CARRITO
// ==========================================

void VentanaVentas::quitarProducto()
{
    int fila =
        ui.tablaCarrito->currentRow();

    if (fila < 0)
    {
        ui.lblMensaje->setText(
            "Seleccione un producto de la tabla"
        );

        return;
    }

    vector<DetalleVenta> articulos =
        carrito.detalles();

    carrito.quitar(
        articulos[fila].getCodigoProducto()
    );

    ui.lblMensaje->setText("");

    actualizarTotal();
}

// ==========================================
// COBRAR VENTA E IMPRIMIR COMPROBANTE
// ==========================================

void VentanaVentas::cobrar()
{
    if (gestor == nullptr)
    {
        ui.lblMensaje->setText(
            "No se encuentra disponible "
            "el gestor de ventas"
        );

        return;
    }

    // ======================================
    // OBTENER EFECTIVO RECIBIDO
    // ======================================

    bool efectivoValido = false;

    qint64 efectivo =
        ui.txtEfectivo->text().toLongLong(
            &efectivoValido
        );

    if (!efectivoValido || efectivo < 0)
    {
        ui.lblMensaje->setText(
            "Ingrese una cantidad de efectivo valida"
        );

        return;
    }

    // ======================================
    // CONFIRMAR VENTA
    // ======================================

    ResultadoVenta resultado =
        gestor->confirmar(
            carrito,
            efectivo
        );

    if (!resultado.fueExitosa())
    {
        ui.lblMensaje->setText(
            QString::fromStdString(
                resultado.getMensaje()
            )
        );

        return;
    }

    Venta venta = resultado.getVenta();

    // ======================================
    // MOSTRAR RESULTADO EN LA VENTANA
    // ======================================

    ui.lblMensaje->setText(
        "Venta " +
        QString::number(venta.getNumero()) +
        " realizada. Cambio: ₡" +
        QString::number(venta.getCambio())
    );

    // Limpiar carrito despues de confirmar
    carrito.vaciar();

    ui.txtEfectivo->clear();

    actualizarTotal();

    // ======================================
    // MENSAJE DE VENTA REALIZADA
    // ======================================

    QMessageBox mensaje(this);

    mensaje.setWindowTitle(
        "La Pulpe - Venta realizada"
    );

    mensaje.setIcon(
        QMessageBox::Information
    );

    mensaje.setText(
        "¡Venta realizada correctamente!\n\n"
        "Venta N.º " +
        QString::number(venta.getNumero()) +
        "\nTotal: ₡" +
        QString::number(venta.getTotal()) +
        "\nEfectivo recibido: ₡" +
        QString::number(venta.getEfectivo()) +
        "\nCambio: ₡" +
        QString::number(venta.getCambio()) +
        "\n\n¿Qué desea hacer con el comprobante?"
    );

    // ======================================
    // ESTILO CORREGIDO DEL MENSAJE
    // ======================================

    // Se aplica solamente a este QMessageBox.
    // No modifica el estilo de otras ventanas.

    mensaje.setStyleSheet(R"(
        QMessageBox {
            background-color: #202622;
            color: #FFFFFF;
            font-family: "Segoe UI";
            font-size: 13px;
        }

        QMessageBox QLabel {
            background-color: transparent;
            color: #FFFFFF;
            font-family: "Segoe UI";
            font-size: 13px;
            min-width: 285px;
        }

        QMessageBox QPushButton {
            background-color: #187545;
            color: #FFFFFF;
            border: 1px solid #12613A;
            border-radius: 7px;
            padding: 9px 13px;
            font-family: "Segoe UI";
            font-size: 12px;
            font-weight: bold;
            min-width: 90px;
            min-height: 20px;
        }

        QMessageBox QPushButton:hover {
            background-color: #105B34;
        }

        QMessageBox QPushButton:pressed {
            background-color: #0C4729;
        }
    )");

    // ======================================
    // BOTONES DEL COMPROBANTE
    // ======================================

    QPushButton* btnImprimir =
        mensaje.addButton(
            "Imprimir ticket",
            QMessageBox::ActionRole
        );

    QPushButton* btnGuardar =
        mensaje.addButton(
            "Guardar HTML",
            QMessageBox::ActionRole
        );

    QPushButton* btnFinalizar =
        mensaje.addButton(
            "Finalizar",
            QMessageBox::RejectRole
        );

    // Estilo diferente para finalizar
    btnFinalizar->setStyleSheet(R"(
        QPushButton {
            background-color: #48534B;
            color: white;
            border: 1px solid #59665C;
            border-radius: 7px;
            padding: 9px 13px;
            font-family: "Segoe UI";
            font-size: 12px;
            font-weight: bold;
            min-width: 90px;
            min-height: 20px;
        }

        QPushButton:hover {
            background-color: #5C6A60;
        }
    )");

    // Mostrar mensaje
    mensaje.exec();

    // ======================================
    // FINALIZAR SIN COMPROBANTE
    // ======================================

    if (mensaje.clickedButton() != btnImprimir &&
        mensaje.clickedButton() != btnGuardar)
    {
        return;
    }

    // ======================================
    // GENERAR TICKET HTML
    // ======================================

    TicketHTML generador;

    if (!generador.generar(venta))
    {
        QMessageBox::warning(
            this,
            "La Pulpe",
            "La venta fue registrada correctamente, "
            "pero no se pudo generar el "
            "comprobante HTML."
        );

        return;
    }

    // ======================================
    // OBTENER RUTA DEL COMPROBANTE
    // ======================================

    QString rutaTicket =
        QString::fromStdString(
            (
                std::filesystem::current_path() /
                "Tickets" /
                "HTML" /
                (
                    "Ticket_Venta_" +
                    std::to_string(
                        venta.getNumero()
                    ) +
                    ".html"
                )
            ).string()
        );

    // ======================================
    // OPCION GUARDAR HTML
    // ======================================

    if (mensaje.clickedButton() == btnGuardar)
    {
        QMessageBox::information(
            this,
            "La Pulpe",
            "Comprobante HTML guardado "
            "correctamente en la carpeta "
            "Tickets/HTML."
        );

        return;
    }

    // ======================================
    // OPCION IMPRIMIR TICKET
    // ======================================

    QFile archivo(rutaTicket);

    if (!archivo.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(
            this,
            "La Pulpe",
            "No se pudo abrir el comprobante "
            "para imprimirlo."
        );

        return;
    }

    // Leer contenido HTML
    QString contenidoHTML =
        QString::fromUtf8(
            archivo.readAll()
        );

    archivo.close();

    // Crear documento imprimible
    QTextDocument documento;

    documento.setHtml(
        contenidoHTML
    );

    // ======================================
    // CONFIGURAR IMPRESORA
    // ======================================

    QPrinter impresora(
        QPrinter::HighResolution
    );

    impresora.setDocName(
        "La Pulpe - Ticket Venta " +
        QString::number(
            venta.getNumero()
        )
    );

    // Abrir selector de impresoras
    QPrintDialog dialogo(
        &impresora,
        this
    );

    dialogo.setWindowTitle(
        "La Pulpe - Imprimir ticket"
    );

    // Si cancela, no imprimir
    if (dialogo.exec() != QDialog::Accepted)
    {
        return;
    }

    // Enviar documento a impresora
    documento.print(
        &impresora
    );

    QMessageBox::information(
        this,
        "La Pulpe",
        "El comprobante fue enviado "
        "a la impresora seleccionada."
    );
}

// ==========================================
// ACTUALIZAR TABLA DEL CARRITO Y TOTAL
// ==========================================

void VentanaVentas::actualizarTotal()
{
    vector<DetalleVenta> articulos =
        carrito.detalles();

    ui.tablaCarrito->setRowCount(
        (int)articulos.size()
    );

    for (int i = 0; i < (int)articulos.size(); i++)
    {
        // ==================================
        // NOMBRE DEL PRODUCTO
        // ==================================

        ui.tablaCarrito->setItem(
            i,
            0,
            new QTableWidgetItem(
                QString::fromStdString(
                    articulos[i].getNombreProducto()
                )
            )
        );

        // ==================================
        // CANTIDAD
        // ==================================

        ui.tablaCarrito->setItem(
            i,
            1,
            new QTableWidgetItem(
                QString::number(
                    articulos[i].getCantidad()
                )
            )
        );

        // ==================================
        // PRECIO UNITARIO
        // ==================================

        ui.tablaCarrito->setItem(
            i,
            2,
            new QTableWidgetItem(
                "₡" +
                QString::number(
                    articulos[i].getPrecioUnitario()
                )
            )
        );

        // ==================================
        // SUBTOTAL
        // ==================================

        ui.tablaCarrito->setItem(
            i,
            3,
            new QTableWidgetItem(
                "₡" +
                QString::number(
                    articulos[i].subtotal()
                )
            )
        );
    }

    // ======================================
    // ACTUALIZAR TOTAL
    // ======================================

    ui.lblTotal->setText(
        "Total: ₡" +
        QString::number(
            carrito.total()
        )
    );
}
