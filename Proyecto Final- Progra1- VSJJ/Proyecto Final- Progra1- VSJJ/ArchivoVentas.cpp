#include "ArchivoVentas.h"
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonParseError>

// Constructor
ArchivoVentas::ArchivoVentas(const QString& rutaArchivo)
    : m_rutaArchivo(rutaArchivo)
{
}

// Guardar ventas en JSON
bool ArchivoVentas::guardarVentas(const QJsonArray& ventas) const
{
    QSaveFile archivo(m_rutaArchivo);

    if (!archivo.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QJsonDocument documento(ventas);

    archivo.write(documento.toJson(QJsonDocument::Indented));

    return archivo.commit();
}

// Cargar ventas desde JSON
QJsonArray ArchivoVentas::cargarVentas(bool* correcto) const
{
    if (correcto != nullptr)
    {
        *correcto = false;
    }

    QFile archivo(m_rutaArchivo);

    if (!archivo.open(QIODevice::ReadOnly))
    {
        return QJsonArray();
    }

    QByteArray datos = archivo.readAll();

    QJsonParseError error;
    QJsonDocument documento =
        QJsonDocument::fromJson(datos, &error);

    if (error.error != QJsonParseError::NoError ||
        !documento.isArray())
    {
        return QJsonArray();
    }

    if (correcto != nullptr)
    {
        *correcto = true;
    }

    return documento.array();
}
