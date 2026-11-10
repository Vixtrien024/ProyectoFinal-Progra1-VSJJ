#pragma once
#include <QWidget>

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

    void actualizarTabla();
    void buscarVenta();
    void mostrarDetalle();
    void exportarTXT();
    void exportarHTML();
};