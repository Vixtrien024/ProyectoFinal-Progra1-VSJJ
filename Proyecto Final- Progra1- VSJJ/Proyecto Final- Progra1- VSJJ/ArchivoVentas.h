#pragma once
#include <string>
#include <QJsonArray>

using namespace std;

class ArchivoVentas
{
public:
    // Constructor
    explicit ArchivoVentas(const string& rutaArchivo = "ventas.json");

    // Guarda las ventas en un archivo JSON
    bool guardarVentas(const QJsonArray& ventas) const;

    // Carga las ventas desde el archivo JSON
    QJsonArray cargarVentas(bool* correcto = nullptr) const;

private:
    string m_rutaArchivo;
};