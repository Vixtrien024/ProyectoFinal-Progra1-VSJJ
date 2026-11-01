#include "ArchivoVentas.h"
#include <fstream>
#include <iterator>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QByteArray>

using namespace std;

// Constructor
ArchivoVentas::ArchivoVentas(const string& rutaArchivo)
    : m_rutaArchivo(rutaArchivo)
{
}

// Guarda las ventas en formato JSON
bool ArchivoVentas::guardarVentas(const QJsonArray& ventas) const
{
    ofstream archivo(m_rutaArchivo);

    if (!archivo.is_open())
    {
        return false;
    }

    QJsonDocument documento(ventas);

    string datos = documento.toJson(
        QJsonDocument::Indented
    ).toStdString();

    archivo << datos;

    archivo.flush();

    bool correcto = archivo.good();

    archivo.close();

    return correcto && !archivo.fail();
}

// Carga las ventas desde el archivo JSON
QJsonArray ArchivoVentas::cargarVentas(bool* correcto) const
{
    if (correcto != nullptr)
    {
        *correcto = false;
    }

    ifstream archivo(m_rutaArchivo);

    if (!archivo.is_open())
    {
        return QJsonArray();
    }

    string datos(
        (istreambuf_iterator<char>(archivo)),
        istreambuf_iterator<char>()
    );

    archivo.close();

    QByteArray contenido = QByteArray::fromStdString(datos);

    QJsonParseError error;

    QJsonDocument documento = QJsonDocument::fromJson(
        contenido,
        &error
    );

    if (error.error != QJsonParseError::NoError)
    {
        return QJsonArray();
    }

    if (!documento.isArray())
    {
        return QJsonArray();
    }

    if (correcto != nullptr)
    {
        *correcto = true;
    }

    return documento.array();
}