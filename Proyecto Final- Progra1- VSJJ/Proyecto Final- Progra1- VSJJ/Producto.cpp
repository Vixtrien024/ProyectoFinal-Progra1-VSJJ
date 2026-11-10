#include "Producto.h"

using namespace std;

// Funcion auxiliar: elimina los espacios del inicio y del final.
static string recortar(const string& texto) {
    const string espacios = " \t\n\r";

    // Posicion del primer caracter que NO es espacio
    size_t inicio = texto.find_first_not_of(espacios);

    // Si el texto esta vacio o solo contiene espacios
    if (inicio == string::npos) return "";

    // Posicion del ultimo caracter que NO es espacio
    size_t fin = texto.find_last_not_of(espacios);

    return texto.substr(inicio, fin - inicio + 1);
}//fin de recortar

// Constructores

// Constructor vacio: valores por defecto
Producto::Producto()
    : codigo(0), nombre(""), precio(0),
    categoria(""), existencias(0), activo(true) {
}//fin del constructor vacio

// Constructor con datos
// Si el precio o las existencias vienen negativos se dejan en 0;
// GestorProductos es quien rechaza el producto si los datos no sirven.
Producto::Producto(int codigo, const string& nombre,
    qint64 precio, const string& categoria, int existencias)
    : codigo(codigo),
    nombre(recortar(nombre)),
    precio(precio > 0 ? precio : 0),
    categoria(recortar(categoria)),
    existencias(existencias > 0 ? existencias : 0),
    activo(true) {
}//fin del constructor con datos

// Getters

int Producto::getCodigo() const {
    return codigo;
}//fin de getCodigo

string Producto::getNombre() const {
    return nombre;
}//fin de getNombre

qint64 Producto::getPrecio() const {
    return precio;
}//fin de getPrecio

string Producto::getCategoria() const {
    return categoria;
}//fin de getCategoria

int Producto::getExistencias() const {
    return existencias;
}//fin de getExistencias

bool Producto::estaActivo() const {
    return activo;
}//fin de estaActivo

// Setters con validacion

// El nombre no puede estar vacio ni contener solo espacios
bool Producto::setNombre(const string& nuevoNombre) {
    string limpio = recortar(nuevoNombre);

    if (limpio.empty()) return false;

    nombre = limpio;
    return true;
}//fin de setNombre

// El precio debe ser mayor a cero
bool Producto::setPrecio(qint64 nuevoPrecio) {
    if (nuevoPrecio <= 0) return false;

    precio = nuevoPrecio;
    return true;
}//fin de setPrecio

// La categoria puede quedar vacia
void Producto::setCategoria(const string& nuevaCategoria) {
    categoria = recortar(nuevaCategoria);
}//fin de setCategoria

// Las existencias no pueden ser negativas (cero si es valido: se agoto)
bool Producto::setExistencias(int nuevasExistencias) {
    if (nuevasExistencias < 0) return false;

    existencias = nuevasExistencias;
    return true;
}//fin de setExistencias

// Actualiza todos los datos editables de una sola vez.
// Primero se valida todo; si algo falla no se cambia nada.
bool Producto::actualizarDatos(const string& nuevoNombre, qint64 nuevoPrecio,
    const string& nuevaCategoria, int nuevasExistencias) {

    if (recortar(nuevoNombre).empty() ||
        nuevoPrecio <= 0 ||
        nuevasExistencias < 0) {
        return false;
    }//fin del if

    nombre = recortar(nuevoNombre);
    precio = nuevoPrecio;
    categoria = recortar(nuevaCategoria);
    existencias = nuevasExistencias;
    return true;
}//fin de actualizarDatos

// Control del estado del producto

// Desactivar no elimina el producto
void Producto::desactivar() {
    activo = false;
}//fin de desactivar

void Producto::activar() {
    activo = true;
}//fin de activar