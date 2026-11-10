#pragma once

#include "GestorProductos.h"

class Inventario {
private:
    GestorProductos* gestor;//Evita que dupliquemos la coleccion de productos: usa la del gestor

public:
    Inventario(GestorProductos* gestor);

    bool hayExistencias(int codigo, int cantidad) const;//Comprueba si hay suficientes unidades
    bool descontar(int codigo, int cantidad);//Descuenta unidades del stock (al vender)
    bool reponer(int codigo, int cantidad);//Suma unidades al stock (al recibir mercaderia)

};//fin de la clase