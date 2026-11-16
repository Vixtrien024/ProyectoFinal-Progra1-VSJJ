
#include "TicketTXT.h"

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <system_error>

using namespace std;

bool TicketTXT::generar(const Venta& venta)
{
    // CREAR CARPETA PARA LOS TICKETS TXT

    filesystem::path carpeta =
        filesystem::current_path() / "Tickets" / "TXT";

    error_code error;

    filesystem::create_directories(carpeta, error);

    // Verificar que se pudo crear la carpeta
    if (error)
    {
        return false;
    }

    // NOMBRE Y UBICACION DEL ARCHIVO

    filesystem::path nombreArchivo =
        carpeta / ("Ticket_Venta_" +
            to_string(venta.getNumero()) + ".txt");

    // Crear el archivo de texto
    ofstream archivo(nombreArchivo, ios::binary);

    // Verificar que se pueda crear
    if (!archivo.is_open())
    {
        return false;
    }

    // ENCABEZADO DEL TICKET

    archivo << "========================================\n";
    archivo << "                LA PULPE\n";
    archivo << "========================================\n";
    archivo << "       COMPROBANTE DE VENTA\n";
    archivo << "========================================\n\n";

    archivo << "Numero de venta: "
            << venta.getNumero() << "\n";

    archivo << "Fecha: "
            << venta.getFecha() << "\n";

    archivo << "Cliente: Consumidor final\n\n";

    // PRODUCTOS VENDIDOS

    archivo << "----------------------------------------\n";
    archivo << "PRODUCTOS VENDIDOS\n";
    archivo << "----------------------------------------\n";

    vector<DetalleVenta> detalles = venta.getDetalles();

    for (const DetalleVenta& detalle : detalles)
    {
        archivo << "\nProducto: "
                << detalle.getNombreProducto() << "\n";

        archivo << "Codigo: "
                << detalle.getCodigoProducto() << "\n";

        archivo << "Cantidad: "
                << detalle.getCantidad() << "\n";

        archivo << "Precio unitario: CRC "
                << detalle.getPrecioUnitario() << "\n";

        archivo << "Subtotal: CRC "
                << detalle.subtotal() << "\n";
    }

    // RESUMEN DEL COBRO

    archivo << "\n========================================\n";

    archivo << "TOTAL: CRC "
            << venta.getTotal() << "\n";

    archivo << "EFECTIVO RECIBIDO: CRC "
            << venta.getEfectivo() << "\n";

    archivo << "CAMBIO: CRC "
            << venta.getCambio() << "\n";

    archivo << "========================================\n\n";

    archivo << "       Gracias por su compra!\n";
    archivo << "         Vuelva pronto!\n\n";

    archivo << "========================================\n";
    archivo << "                LA PULPE\n";
    archivo << "========================================\n";

    // VERIFICAR QUE EL ARCHIVO SE GUARDO

    archivo.flush();

    bool correcto = archivo.good();

    archivo.close();

    return correcto;
}
