
#include "GestorProductos.h"
#include "Autenticacion.h"
#include "GestorActividades.h"

#include <string>

using namespace std;

// ==========================================
// CONSTRUCTOR
// ==========================================

GestorProductos::GestorProductos(ArchivoProductos* archivo)
    : archivo(archivo),
      gestorActividades(nullptr),
      autenticacion(nullptr)
{
    // Cargar los productos guardados
    if (this->archivo != nullptr)
    {
        productos = this->archivo->cargar();
    }
}

// ==========================================
// CONECTAR GESTOR DE ACTIVIDADES
// ==========================================

void GestorProductos::setGestorActividades(
    GestorActividades* gestorActividades)
{
    this->gestorActividades = gestorActividades;
}

// ==========================================
// CONECTAR AUTENTICACION
// ==========================================

void GestorProductos::setAutenticacion(
    Autenticacion* autenticacion)
{
    this->autenticacion = autenticacion;
}

// ==========================================
// REGISTRAR ACTIVIDAD DE PRODUCTOS
// ==========================================

void GestorProductos::registrarActividadProducto(
    const string& tipo,
    const string& descripcion)
{
    // Comprobar que los gestores esten conectados
    if (gestorActividades == nullptr ||
        autenticacion == nullptr)
    {
        return;
    }

    // Obtener el usuario conectado
    Usuario* usuarioActual =
        autenticacion->getUsuarioActual();

    // No registrar operaciones sin sesion activa
    if (usuarioActual == nullptr)
    {
        return;
    }

    // Registrar la actividad
    gestorActividades->registrarActividad(
        usuarioActual->getId(),
        usuarioActual->getNombreUsuario(),
        tipo,
        descripcion
    );
}

// ==========================================
// BUSCAR POSICION DE PRODUCTO
// ==========================================

int GestorProductos::posicionDe(int codigo) const
{
    for (size_t i = 0; i < productos.size(); i++)
    {
        if (productos[i].getCodigo() == codigo)
        {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// ==========================================
// AGREGAR PRODUCTO
// ==========================================

bool GestorProductos::agregar(const Producto& producto)
{
    // Validar los datos
    if (producto.getCodigo() <= 0 ||
        producto.getNombre().empty() ||
        producto.getPrecio() <= 0 ||
        producto.getExistencias() < 0)
    {
        return false;
    }

    // Evitar codigos repetidos
    if (posicionDe(producto.getCodigo()) != -1)
    {
        return false;
    }

    // Agregar temporalmente
    productos.push_back(producto);

    // Guardar los cambios
    if (!guardar())
    {
        // Restaurar si falla
        productos.pop_back();
        return false;
    }

    // Registrar producto agregado
    registrarActividadProducto(
        "PRODUCTO_CREADO",
        "Se agrego el producto: " +
        producto.getNombre() +
        " | Codigo: " +
        to_string(producto.getCodigo()) +
        " | Existencias iniciales: " +
        to_string(producto.getExistencias())
    );

    return true;
}

// ==========================================
// MODIFICAR PRODUCTO
// ==========================================

bool GestorProductos::modificar(const Producto& producto)
{
    int pos = posicionDe(producto.getCodigo());

    if (pos == -1)
    {
        return false;
    }

    // Guardar una copia del producto original
    Producto original = productos[pos];

    // Crear una copia para aplicar los cambios
    Producto actualizado = original;

    if (!actualizado.setNombre(producto.getNombre()))
    {
        return false;
    }

    if (!actualizado.setPrecio(producto.getPrecio()))
    {
        return false;
    }

    if (!actualizado.setExistencias(
        producto.getExistencias()))
    {
        return false;
    }

    actualizado.setCategoria(producto.getCategoria());

    // Actualizar estado
    if (producto.estaActivo())
    {
        actualizado.activar();
    }
    else
    {
        actualizado.desactivar();
    }

    // Aplicar cambios
    productos[pos] = actualizado;

    // Intentar guardar
    if (!guardar())
    {
        // Restaurar producto original
        productos[pos] = original;
        return false;
    }

    // ======================================
    // REGISTRAR MODIFICACION
    // ======================================

    registrarActividadProducto(
        "PRODUCTO_MODIFICADO",
        "Se modifico el producto: " +
        actualizado.getNombre() +
        " | Codigo: " +
        to_string(actualizado.getCodigo())
    );

    // ======================================
    // REGISTRAR AJUSTE MANUAL DE INVENTARIO
    // ======================================

    // Registrar existencias solamente si cambiaron
    if (original.getExistencias() !=
        actualizado.getExistencias())
    {
        registrarActividadProducto(
            "INVENTARIO_AJUSTADO",
            "Producto: " +
            actualizado.getNombre() +
            " | Codigo: " +
            to_string(actualizado.getCodigo()) +
            " | Existencias anteriores: " +
            to_string(original.getExistencias()) +
            " | Existencias nuevas: " +
            to_string(actualizado.getExistencias())
        );
    }

    return true;
}

// ==========================================
// ACTUALIZAR EXISTENCIAS INTERNAMENTE
// ==========================================

bool GestorProductos::actualizarExistenciasInterno(
    int codigo,
    int nuevasExistencias)
{
    // ======================================
    // VALIDAR DATOS
    // ======================================

    // No permitir codigos invalidos
    if (codigo <= 0)
    {
        return false;
    }

    // No permitir existencias negativas
    if (nuevasExistencias < 0)
    {
        return false;
    }

    // ======================================
    // BUSCAR PRODUCTO
    // ======================================

    int pos = posicionDe(codigo);

    if (pos == -1)
    {
        return false;
    }

    // No modificar productos desactivados
    if (!productos[pos].estaActivo())
    {
        return false;
    }

    // ======================================
    // GUARDAR ESTADO ORIGINAL
    // ======================================

    Producto original = productos[pos];

    // ======================================
    // ACTUALIZAR EXISTENCIAS
    // ======================================

    if (!productos[pos].setExistencias(
        nuevasExistencias))
    {
        return false;
    }

    // ======================================
    // GUARDAR CAMBIOS
    // ======================================

    if (!guardar())
    {
        // Restaurar el valor anterior en memoria
        productos[pos] = original;

        return false;
    }

    // ======================================
    // OPERACION COMPLETADA
    // ======================================

    // No registrar PRODUCTO_MODIFICADO.
    // No registrar INVENTARIO_AJUSTADO.
    //
    // Las ventas exitosas se registran
    // desde GestorVentas.cpp.
    //
    // Si una venta falla, el inventario
    // puede restaurarse sin generar
    // registros de modificaciones manuales.

    return true;
}

// ==========================================
// DESACTIVAR PRODUCTO
// ==========================================

bool GestorProductos::desactivar(int codigo)
{
    int pos = posicionDe(codigo);

    if (pos == -1)
    {
        return false;
    }

    // Guardar el producto original
    Producto original = productos[pos];

    // Desactivar
    productos[pos].desactivar();

    // Intentar guardar
    if (!guardar())
    {
        // Restaurar si falla
        productos[pos] = original;
        return false;
    }

    // Registrar desactivacion
    registrarActividadProducto(
        "PRODUCTO_DESACTIVADO",
        "Se desactivo el producto: " +
        productos[pos].getNombre() +
        " | Codigo: " +
        to_string(codigo)
    );

    return true;
}

// ==========================================
// BUSCAR PRODUCTO POR CODIGO
// ==========================================

optional<Producto> GestorProductos::buscar(int codigo) const
{
    int pos = posicionDe(codigo);

    if (pos == -1)
    {
        return nullopt;
    }

    return productos[pos];
}

// ==========================================
// LISTAR TODOS LOS PRODUCTOS
// ==========================================

vector<Producto> GestorProductos::listar() const
{
    return productos;
}

// ==========================================
// GUARDAR PRODUCTOS
// ==========================================

bool GestorProductos::guardar()
{
    // Permite trabajar solamente en memoria
    if (archivo == nullptr)
    {
        return true;
    }

    return archivo->guardar(productos);
}
