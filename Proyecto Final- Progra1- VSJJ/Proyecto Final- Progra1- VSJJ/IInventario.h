#pragma once

// Contrato minimo que Ventas necesita del inventario (modulo de Julian).
// Inventario hereda de esta interfaz; asi Ventas no depende de su implementacion.
class IInventario {
public:
    virtual ~IInventario() = default;
    virtual int consultarExistencias(int idProducto) const = 0;
    virtual bool descontarExistencias(int idProducto, int cantidad) = 0;
};
