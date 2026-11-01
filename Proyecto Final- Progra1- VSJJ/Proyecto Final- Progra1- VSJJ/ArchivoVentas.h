#pragma once
#include <QString>
#include <QJsonArray>

class ArchivoVentas
{
public:
    // Constructor
    explicit ArchivoVentas(
        const QString& rutaArchivo = "ventas.json"
    );

    // Guarda las ventas en formato JSON
    bool guardarVentas(const QJsonArray& ventas) const;

    // Recupera las ventas guardadas
    QJsonArray cargarVentas(bool* correcto = nullptr) const;

private:
    QString m_rutaArchivo;
};
