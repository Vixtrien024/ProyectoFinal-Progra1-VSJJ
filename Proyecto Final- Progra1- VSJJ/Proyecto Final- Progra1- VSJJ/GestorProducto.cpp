#include "GestorProductos.h"

// Constructor: carga lo que haya guardado en el archivo
GestorProductos::GestorProductos(ArchivoProductos* archivo)
    : archivo(archivo) {

    if (this->archivo != nullptr) {
        productos = this->archivo->cargar();
    }//fin del if
}//fin del constructor


// Buscar la posicion de un producto en el vector
int GestorProductos::posicionDe(int codigo) const {
    for (size_t i = 0; i < productos.size(); i++) {
        if (productos[i].getCodigo() == codigo) {
            return static_cast<int>(i);
        }//fin del if
    }//fin del for

    return -1;//No se encontro el producto
}//fin de posicionDe


// Agregar producto
bool GestorProductos::agregar(const Producto& producto) {

    // Validar los datos antes de aceptar el producto
    if (producto.getCodigo() <= 0 ||
        producto.getNombre().empty() ||
        producto.getPrecio() <= 0) {
        return false;
    }//fin del if

    // Revisa que el codigo no se repita con otro producto ya existente
    if (posicionDe(producto.getCodigo()) != -1) {
        return false;
    }//fin del if

    productos.push_back(producto);//Agregar el producto
    return guardar();
}//fin de agregar


// Modificar producto (se identifica por su codigo)
bool GestorProductos::modificar(const Producto& producto) {

    int pos = posicionDe(producto.getCodigo());

    if (pos == -1) {
        return false;
    }//fin del if

    // Crear una copia para validar los cambios antes de aplicarlos
    Producto actualizado = productos[pos];

    if (!actualizado.setNombre(producto.getNombre())) {
        return false;
    }//fin del if

    if (!actualizado.setPrecio(producto.getPrecio())) {
        return false;
    }//fin del if

    if (!actualizado.setExistencias(producto.getExistencias())) {
        return false;
    }//fin del if

    actualizado.setCategoria(producto.getCategoria());

    if (producto.estaActivo()) {
        actualizado.activar();
    }//fin del if
    else {
        actualizado.desactivar();
    }//fin del else

    // Aplicar los cambios
    productos[pos] = actualizado;

    return guardar();
}//fin de modificar


// Desactivar producto (nunca se elimina de la lista)
bool GestorProductos::desactivar(int codigo) {

    int pos = posicionDe(codigo);

    if (pos == -1) {//valida el producto que se debe desactivar
        return false;
    }//fin del if

    productos[pos].desactivar();
    return guardar();
}//fin de desactivar


// Buscar producto por codigo
// Devuelve una copia: asi nadie queda con un puntero que se pueda danar
// cuando el vector crece.
optional<Producto> GestorProductos::buscar(int codigo) const {

    int pos = posicionDe(codigo);

    if (pos == -1) {
        return nullopt;//No se encontro el producto
    }//fin del if

    return productos[pos];
}//fin de buscar


// Listar productos
vector<Producto> GestorProductos::listar() const {
    return productos;
}//fin de listar


// Guardar en el archivo
// Si no se dio archivo (modo solo memoria) no hay nada que guardar.
bool GestorProductos::guardar() {

    if (archivo == nullptr) {
        return true;
    }//fin del if

    return archivo->guardar(productos);
}//fin de guardar