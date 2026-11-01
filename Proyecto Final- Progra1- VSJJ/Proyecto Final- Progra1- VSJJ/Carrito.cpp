#include "Carrito.h"

bool Carrito::agregarProducto(const Producto& producto, int cantidad) {
    if (cantidad <= 0 || !producto.estaActivo()) return false;

    for (DetalleVenta& d : articulos) {
        if (d.getIdProducto() == producto.getId()) {
            return d.setCantidad(d.getCantidad() + cantidad);
        }
    }
    articulos.push_back(DetalleVenta(producto.getId(), producto.getNombre(),
        producto.getPrecio(), cantidad));
    return true;
}

bool Carrito::quitarProducto(int idProducto) {
    for (size_t i = 0; i < articulos.size(); i++) {
        if (articulos[i].getIdProducto() == idProducto) {
            articulos.erase(articulos.begin() + i);
            return true;
        }
    }
    return false;
}

void Carrito::vaciar() { articulos.clear(); }

const vector<DetalleVenta>& Carrito::getArticulos() const { return articulos; }
bool Carrito::estaVacio() const { return articulos.empty(); }

qint64 Carrito::calcularTotal() const {
    qint64 total = 0;
    for (const DetalleVenta& d : articulos) total += d.calcularSubtotal();
    return total;
}
