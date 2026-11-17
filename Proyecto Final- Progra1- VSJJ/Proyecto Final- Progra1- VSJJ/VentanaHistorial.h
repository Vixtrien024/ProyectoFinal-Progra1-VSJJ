
#pragma once

#include <QWidget>
#include <vector>

#include "Venta.h"
#include "ArchivoVentas.h"
#include "HistorialVentas.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
    class VentanaHistorial;
}
QT_END_NAMESPACE

class VentanaHistorial : public QWidget
{
    Q_OBJECT

public:
    explicit VentanaHistorial(QWidget* parent = nullptr);
    ~VentanaHistorial();

private:
    Ui::VentanaHistorial* ui;

    // Archivo compartido con el modulo de ventas
    ArchivoVentas archivoVentas;

    // Permite consultar las ventas guardadas
    HistorialVentas historial;

    // Ventas que se muestran en la tabla
    vector<Venta> ventas;

    // Cargar las ventas registradas
    void actualizarTabla();

    // Buscar una venta por su numero
    void buscarVenta();

    // Mostrar los productos de la venta seleccionada
    void mostrarDetalle();

    // Generar comprobantes
    void exportarTXT();
    void exportarHTML();

    // Imprimir el ticket de una venta existente
    void imprimirTicket();
};
