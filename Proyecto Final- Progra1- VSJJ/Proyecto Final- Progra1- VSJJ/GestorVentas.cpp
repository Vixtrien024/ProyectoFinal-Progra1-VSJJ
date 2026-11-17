
#include "GestorVentas.h"
#include "GestorActividades.h"
#include "Autenticacion.h"

#include <ctime>
#include <map>
#include <vector>
#include <string>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

using namespace std;

// ==========================================
// OBTENER FECHA Y HORA ACTUAL
// ==========================================

static string fechaActual()
{
    time_t ahora = time(nullptr);
    tm hora{};

    localtime_s(&hora, &ahora);

    char texto[30];

    strftime(
        texto,
        sizeof(texto),
        "%Y-%m-%d %H:%M:%S",
        &hora
    );

    return string(texto);
}

// ==========================================
// CONSTRUCTOR
// ==========================================

GestorVentas::GestorVentas()
    : archivoVentas("ventas.json")
{
    cargarVentas();
}

// ==========================================
// CONECTAR INVENTARIO
// ==========================================

void GestorVentas::setInventario(
    Inventario* inventarioActual)
{
    inventario = inventarioActual;
}

// ==========================================
// CONECTAR GESTOR DE ACTIVIDADES
// ==========================================

void GestorVentas::setGestorActividades(
    GestorActividades* gestorActividades)
{
    this->gestorActividades = gestorActividades;
}

// ==========================================
// CONECTAR AUTENTICACION
// ==========================================

void GestorVentas::setAutenticacion(
    Autenticacion* autenticacion)
{
    this->autenticacion = autenticacion;
}

// ==========================================
// REGISTRAR ACTIVIDAD DE VENTAS
// ==========================================

void GestorVentas::registrarActividadVenta(
    const string& tipo,
    const string& descripcion)
{
    // Verificar conexiones
    if (gestorActividades == nullptr ||
        autenticacion == nullptr)
    {
        return;
    }

    // Obtener usuario conectado
    Usuario* usuarioActual =
        autenticacion->getUsuarioActual();

    if (usuarioActual == nullptr)
    {
        return;
    }

    // Guardar actividad con usuario responsable
    gestorActividades->registrarActividad(
        usuarioActual->getId(),
        usuarioActual->getNombreUsuario(),
        tipo,
        descripcion
    );
}

// ==========================================
// CARGAR VENTAS DESDE JSON
// ==========================================

void GestorVentas::cargarVentas()
{
    ventas.clear();

    bool correcto = false;

    QJsonArray datos =
        archivoVentas.cargarVentas(&correcto);

    if (!correcto)
    {
        return;
    }

    for (const QJsonValue& valor : datos)
    {
        if (valor.isObject())
        {
            Venta venta =
                Venta::desdeJson(valor.toObject());

            ventas.push_back(venta);
        }
    }
}

// ==========================================
// GUARDAR VENTAS EN JSON
// ==========================================

bool GestorVentas::guardarVentas()
{
    QJsonArray datos;

    for (const Venta& venta : ventas)
    {
        datos.append(venta.aJson());
    }

    return archivoVentas.guardarVentas(datos);
}

// ==========================================
// VALIDAR CARRITO, EFECTIVO E INVENTARIO
// ==========================================

bool GestorVentas::validar(
    Carrito carrito,
    qint64 efectivo)
{
    vector<DetalleVenta> detalles =
        carrito.detalles();

    // Verificar que el carrito no este vacio
    if (detalles.empty())
    {
        return false;
    }

    // Verificar que exista inventario conectado
    if (inventario == nullptr)
    {
        return false;
    }

    // Verificar que el total sea valido
    if (carrito.total() <= 0)
    {
        return false;
    }

    // Verificar que el efectivo alcance
    if (efectivo < carrito.total())
    {
        return false;
    }

    // Acumular cantidades por codigo
    map<int, int> cantidades;

    for (const DetalleVenta& detalle : detalles)
    {
        if (detalle.getCodigoProducto() <= 0 ||
            detalle.getCantidad() <= 0 ||
            detalle.getPrecioUnitario() <= 0)
        {
            return false;
        }

        cantidades[detalle.getCodigoProducto()] +=
            detalle.getCantidad();
    }

    // Verificar existencias reales
    for (const auto& producto : cantidades)
    {
        int codigo = producto.first;
        int cantidad = producto.second;

        if (!inventario->hayExistencias(
            codigo, cantidad))
        {
            return false;
        }
    }

    return true;
}

// ==========================================
// CONFIRMAR VENTA
// ==========================================

ResultadoVenta GestorVentas::confirmar(
    Carrito carrito,
    qint64 efectivo)
{
    vector<DetalleVenta> detalles =
        carrito.detalles();

    // ======================================
    // VALIDAR CARRITO
    // ======================================

    if (detalles.empty())
    {
        return ResultadoVenta(
            false,
            "El carrito esta vacio.",
            Venta()
        );
    }

    // ======================================
    // VALIDAR INVENTARIO
    // ======================================

    if (inventario == nullptr)
    {
        return ResultadoVenta(
            false,
            "No se ha conectado el inventario al modulo de ventas.",
            Venta()
        );
    }

    // ======================================
    // VALIDAR TOTAL
    // ======================================

    if (carrito.total() <= 0)
    {
        return ResultadoVenta(
            false,
            "El total de la venta no es valido.",
            Venta()
        );
    }

    // ======================================
    // VALIDAR EFECTIVO
    // ======================================

    if (efectivo < carrito.total())
    {
        return ResultadoVenta(
            false,
            "El efectivo no alcanza para pagar.",
            Venta()
        );
    }

    // ======================================
    // AGRUPAR PRODUCTOS Y CANTIDADES
    // ======================================

    map<int, int> cantidades;

    for (const DetalleVenta& detalle : detalles)
    {
        if (detalle.getCodigoProducto() <= 0 ||
            detalle.getCantidad() <= 0 ||
            detalle.getPrecioUnitario() <= 0)
        {
            return ResultadoVenta(
                false,
                "El carrito contiene productos invalidos.",
                Venta()
            );
        }

        cantidades[detalle.getCodigoProducto()] +=
            detalle.getCantidad();
    }

    // ======================================
    // VERIFICAR EXISTENCIAS
    // ======================================

    for (const auto& producto : cantidades)
    {
        int codigo = producto.first;
        int cantidad = producto.second;

        if (!inventario->hayExistencias(
            codigo, cantidad))
        {
            return ResultadoVenta(
                false,
                "No hay suficientes existencias para uno de los productos.",
                Venta()
            );
        }
    }

    // ======================================
    // DESCONTAR PRODUCTOS DEL INVENTARIO
    // ======================================

    vector<pair<int, int>> descontados;

    for (const auto& producto : cantidades)
    {
        int codigo = producto.first;
        int cantidad = producto.second;

        if (!inventario->descontar(
            codigo, cantidad))
        {
            // Intentar restaurar descuentos anteriores
            bool restaurado = true;

            for (const auto& anterior : descontados)
            {
                if (!inventario->reponer(
                    anterior.first,
                    anterior.second))
                {
                    restaurado = false;
                }
            }

            if (!restaurado)
            {
                return ResultadoVenta(
                    false,
                    "Error al descontar inventario. "
                    "No se pudieron restaurar todas las existencias. "
                    "Revise el inventario manualmente.",
                    Venta()
                );
            }

            return ResultadoVenta(
                false,
                "No se pudo actualizar el inventario. "
                "La venta no fue registrada.",
                Venta()
            );
        }

        // Guardar el producto descontado
        descontados.push_back({
            codigo,
            cantidad
        });
    }

    // ======================================
    // CREAR NUEVA VENTA
    // ======================================

    Venta nueva(
        siguienteNumero(),
        fechaActual(),
        detalles,
        efectivo
    );

    // ======================================
    // AGREGAR VENTA TEMPORALMENTE
    // ======================================

    ventas.push_back(nueva);

    // ======================================
    // GUARDAR VENTA EN JSON
    // ======================================

    if (!guardarVentas())
    {
        // Quitar venta de memoria
        ventas.pop_back();

        // Intentar restaurar inventario
        bool restaurado = true;

        for (const auto& producto : descontados)
        {
            if (!inventario->reponer(
                producto.first,
                producto.second))
            {
                restaurado = false;
            }
        }

        if (!restaurado)
        {
            // Registrar incidencia que requiere revision
            registrarActividadVenta(
                "ERROR_INVENTARIO_VENTA",
                "No se pudo guardar una venta y fallo "
                "la restauracion del inventario. "
                "Se requiere revision manual."
            );

            return ResultadoVenta(
                false,
                "No se pudo guardar la venta y "
                "fallo la restauracion del inventario. "
                "Revise los archivos y las existencias.",
                Venta()
            );
        }

        return ResultadoVenta(
            false,
            "No se pudo guardar la venta. "
            "Se intento restaurar el inventario.",
            Venta()
        );
    }

    // ======================================
    // REGISTRAR VENTA REALIZADA
    // ======================================

    registrarActividadVenta(
        "VENTA_REALIZADA",
        "Venta #" +
        to_string(nueva.getNumero()) +
        " | Total: " +
        to_string(carrito.total()) +
        " CRC" +
        " | Efectivo recibido: " +
        to_string(efectivo) +
        " CRC" +
        " | Cambio: " +
        to_string(efectivo - carrito.total()) +
        " CRC"
    );

    // ======================================
    // REGISTRAR MOVIMIENTOS DE INVENTARIO
    // ======================================

    // Se registran solamente despues de guardar
    // correctamente la venta.

    for (const auto& producto : descontados)
    {
        registrarActividadVenta(
            "INVENTARIO_VENTA",
            "Venta #" +
            to_string(nueva.getNumero()) +
            " | Codigo de producto: " +
            to_string(producto.first) +
            " | Cantidad descontada: " +
            to_string(producto.second)
        );
    }

    // ======================================
    // VENTA COMPLETADA
    // ======================================

    return ResultadoVenta(
        true,
        "Venta realizada correctamente. "
        "Inventario actualizado y venta guardada.",
        nueva
    );
}

// ==========================================
// OBTENER SIGUIENTE NUMERO DE VENTA
// ==========================================

int GestorVentas::siguienteNumero()
{
    int mayorNumero = 0;

    for (const Venta& venta : ventas)
    {
        if (venta.getNumero() > mayorNumero)
        {
            mayorNumero = venta.getNumero();
        }
    }

    return mayorNumero + 1;
}

// ==========================================
// LISTAR VENTAS
// ==========================================

vector<Venta> GestorVentas::listarVentas() const
{
    return ventas;
}
