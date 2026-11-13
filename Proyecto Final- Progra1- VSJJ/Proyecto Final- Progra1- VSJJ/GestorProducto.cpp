
#include "GestorProductos.h"

// Constructor: carga los productos guardados
GestorProductos::GestorProductos(ArchivoProductos* archivo)
    : archivo(archivo) {

    if (this->archivo != nullptr) {
        productos = this->archivo->cargar();
    }
}

// Buscar la posicion de un producto por codigo
int GestorProductos::posicionDe(int codigo) const {

    for (size_t i = 0; i < productos.size(); i++) {

        if (productos[i].getCodigo() == codigo) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// Agregar un producto nuevo
bool GestorProductos::agregar(const Producto& producto) {

    // Validar los datos
    if (producto.getCodigo() <= 0 ||
        producto.getNombre().empty() ||
        producto.getPrecio() <= 0 ||
        producto.getExistencias() < 0) {
        return false;
    }

    // Evitar codigos repetidos
    if (posicionDe(producto.getCodigo()) != -1) {
        return false;
    }

    // Agregar temporalmente
    productos.push_back(producto);

    // Guardar los cambios
    if (!guardar()) {

        // Si falla, quitar el producto agregado
        productos.pop_back();
        return false;
    }

    return true;
}

// Modificar un producto existente
bool GestorProductos::modificar(const Producto& producto) {

    int pos = posicionDe(producto.getCodigo());

    if (pos == -1) {
        return false;
    }

    // Guardar una copia del producto original
    Producto original = productos[pos];

    // Crear una copia para aplicar los cambios
    Producto actualizado = original;

    if (!actualizado.setNombre(producto.getNombre())) {
        return false;
    }

    if (!actualizado.setPrecio(producto.getPrecio())) {
        return false;
    }

    if (!actualizado.setExistencias(producto.getExistencias())) {
        return false;
    }

    actualizado.setCategoria(producto.getCategoria());

    // Actualizar el estado
    if (producto.estaActivo()) {
        actualizado.activar();
    }
    else {
        actualizado.desactivar();
    }

    // Aplicar los cambios
    productos[pos] = actualizado;

    // Intentar guardar
    if (!guardar()) {

        // Restaurar el producto original
        productos[pos] = original;
        return false;
    }

    return true;
}

// Desactivar un producto
bool GestorProductos::desactivar(int codigo) {

    int pos = posicionDe(codigo);

    if (pos == -1) {
        return false;
    }

    // Guardar el estado anterior
    Producto original = productos[pos];

    // Desactivar
    productos[pos].desactivar();

    // Intentar guardar
    if (!guardar()) {

        // Restaurar si falla
        productos[pos] = original;
        return false;
    }

    return true;
}

// Buscar un producto por codigo
optional<Producto> GestorProductos::buscar(int codigo) const {

    int pos = posicionDe(codigo);

    if (pos == -1) {
        return nullopt;
    }

    return productos[pos];
}

// Obtener la lista de productos
vector<Producto> GestorProductos::listar() const {
    return productos;
}

// Guardar todos los productos
bool GestorProductos::guardar() {

    // Permite trabajar solamente en memoria
    if (archivo == nullptr) {
        return true;
    }

    return archivo->guardar(productos);
}
