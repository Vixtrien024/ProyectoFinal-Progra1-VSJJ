#pragma once

#include <string>
#include <QtGlobal>

using namespace std;

class Producto {
private:
    int id;
    string nombre;
    qint64 precio;
    string categoria;
    bool activo;

public:
    Producto();
    Producto(int id, const string& nombre,
        qint64 precio, const string& categoria);

    // Getters
    int getId() const;
    string getNombre() const;
    qint64 getPrecio() const;
    string getCategoria() const;
    bool estaActivo() const;

    // Setters
    bool setNombre(const string& nuevoNombre);
    bool setPrecio(qint64 nuevoPrecio);
    void setCategoria(const string& nuevaCategoria);

    // Control del estado
    void desactivar();
    void activar();
};