
#include "Producto.h"

// Función auxiliar: elimina los espacios del inicio y del final.
static string recortar(const string& texto) {
    const string espacios = " \t\n\r";

    // Posición del primer carácter que NO es espacio
    size_t inicio = texto.find_first_not_of(espacios);

    // Si el texto está vacío o solo contiene espacios
    if (inicio == string::npos) return "";

    // Posición del último carácter que NO es espacio
    size_t fin = texto.find_last_not_of(espacios);

    return texto.substr(inicio, fin - inicio + 1);
}

// Constructores

// Constructor vacío: valores por defecto
Producto::Producto()
    : id(0), nombre(""), precio(0),
    categoria(""), activo(true) {
}

// Constructor con datos
Producto::Producto(int id, const string& nombre,
    qint64 precio, const string& categoria)
    : id(id),
    nombre(recortar(nombre)),
    precio(precio > 0 ? precio : 0),
    categoria(recortar(categoria)),
    activo(true) {
}

// Getters

int Producto::getId() const {
    return id;
}

string Producto::getNombre() const {
    return nombre;
}

qint64 Producto::getPrecio() const {
    return precio;
}

string Producto::getCategoria() const {
    return categoria;
}

bool Producto::estaActivo() const {
    return activo;
}

// Setters con validación

// El nombre no puede estar vacío ni contener solo espacios
bool Producto::setNombre(const string& nuevoNombre) {
    string limpio = recortar(nuevoNombre);

    if (limpio.empty()) return false;

    nombre = limpio;
    return true;
}

// El precio debe ser mayor a cero
bool Producto::setPrecio(qint64 nuevoPrecio) {
    if (nuevoPrecio <= 0) return false;

    precio = nuevoPrecio;
    return true;
}

// La categoría puede quedar vacía
void Producto::setCategoria(const string& nuevaCategoria) {
    categoria = recortar(nuevaCategoria);
}

// Control del estado del producto

// Desactivar no elimina el producto
void Producto::desactivar() {
    activo = false;
}

void Producto::activar() {
    activo = true;
}
