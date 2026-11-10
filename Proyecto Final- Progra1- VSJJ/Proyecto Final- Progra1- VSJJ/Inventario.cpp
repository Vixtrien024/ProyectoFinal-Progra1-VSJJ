#include "Inventario.h"

// Constructor
Inventario::Inventario(GestorProductos* gestor)
    : gestor(gestor) {
}//fin del constructor


// Comprobar si hay suficientes existencias
bool Inventario::hayExistencias(int codigo, int cantidad) const {

    if (gestor == nullptr || cantidad <= 0) {
        return false;
    }//fin del if

    optional<Producto> producto = gestor->buscar(codigo);

    if (!producto.has_value() || !producto->estaActivo()) {
        return false;
    }//fin del if

    return producto->getExistencias() >= cantidad;

}//fin de hayExistencias


// Descontar existencias
bool Inventario::descontar(int codigo, int cantidad) {

    if (!hayExistencias(codigo, cantidad)) {
        return false;
    }//fin del if

    // buscar devuelve una copia: se cambia la copia y luego se manda al gestor,
    // que es quien actualiza la coleccion y guarda en el archivo.
    Producto producto = *gestor->buscar(codigo);

    producto.setExistencias(producto.getExistencias() - cantidad);

    return gestor->modificar(producto);

}//fin de descontar


// Reponer existencias
bool Inventario::reponer(int codigo, int cantidad) {

    if (gestor == nullptr || cantidad <= 0) {
        return false;
    }//fin del if

    optional<Producto> encontrado = gestor->buscar(codigo);

    if (!encontrado.has_value() || !encontrado->estaActivo()) {
        return false;
    }//fin del if

    Producto producto = *encontrado;

    producto.setExistencias(producto.getExistencias() + cantidad);

    return gestor->modificar(producto);

}//fin de reponer