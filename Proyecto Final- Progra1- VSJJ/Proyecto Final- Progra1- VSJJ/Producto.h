#pragma once

#include <string>
#include <QtGlobal>

using namespace std;

class Producto {
private:
    int codigo;//Identificador unico del producto (no cambia nunca)
    string nombre;
    qint64 precio;
    string categoria;
    int existencias;//Unidades disponibles en stock
    bool activo;//true = se puede vender, false = desactivado (no se borra)

public:
    Producto();
    Producto(int codigo, const string& nombre,
        qint64 precio, const string& categoria, int existencias = 0);

    // Getters
    int getCodigo() const;
    string getNombre() const;
    qint64 getPrecio() const;
    string getCategoria() const;
    int getExistencias() const;
    bool estaActivo() const;

    // Setters (devuelven false si el dato no es valido y no cambian nada)
    bool setNombre(const string& nuevoNombre);
    bool setPrecio(qint64 nuevoPrecio);
    void setCategoria(const string& nuevaCategoria);
    bool setExistencias(int nuevasExistencias);

    // Cambia nombre, precio, categoria y existencias juntos (todo o nada)
    bool actualizarDatos(const string& nuevoNombre, qint64 nuevoPrecio,
        const string& nuevaCategoria, int nuevasExistencias);

    // Control del estado
    void desactivar();
    void activar();
};//fin de la clase
