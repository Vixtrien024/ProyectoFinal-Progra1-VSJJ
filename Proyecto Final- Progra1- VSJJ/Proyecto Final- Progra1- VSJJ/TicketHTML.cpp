
#include "TicketHTML.h"

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <system_error>

using namespace std;

// EVITAR QUE CARACTERES ESPECIALES DAÑEN EL HTML

static string escaparHTML(const string& texto)
{
    string resultado;

    for (char caracter : texto)
    {
        switch (caracter)
        {
        case '&':
            resultado += "&amp;";
            break;

        case '<':
            resultado += "&lt;";
            break;

        case '>':
            resultado += "&gt;";
            break;

        case '"':
            resultado += "&quot;";
            break;

        case '\'':
            resultado += "&#39;";
            break;

        default:
            resultado += caracter;
            break;
        }
    }

    return resultado;
}

// GENERAR COMPROBANTE HTML

bool TicketHTML::generar(const Venta& venta)
{
    // CREAR CARPETA PARA LOS TICKETS HTML

    filesystem::path carpeta =
        filesystem::current_path() / "Tickets" / "HTML";

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
            to_string(venta.getNumero()) + ".html");

    // Crear archivo HTML
    ofstream archivo(nombreArchivo, ios::binary);

    if (!archivo.is_open())
    {
        return false;
    }

    // ESTRUCTURA Y ESTILOS DEL HTML

    archivo << R"HTML(<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Comprobante de Venta - La Pulpe</title>

<style>
    * {
        box-sizing: border-box;
    }

    body {
        background-color: #07110F;
        color: #F4F7F5;
        font-family: Arial, sans-serif;
        margin: 0;
        padding: 35px 15px;
    }

    .ticket {
        background-color: #0B1513;
        border: 2px solid #20D875;
        border-radius: 18px;
        max-width: 650px;
        margin: auto;
        padding: 30px;
    }

    .marca {
        color: #39E989;
        font-size: 36px;
        font-weight: bold;
        text-align: center;
        margin-bottom: 5px;
    }

    .subtitulo {
        color: #D3DBD7;
        text-align: center;
        font-size: 15px;
    }

    .linea {
        border: none;
        border-top: 1px solid #20D875;
        margin: 22px 0;
    }

    h2 {
        color: #39E989;
        font-size: 20px;
        margin-bottom: 15px;
    }

    .dato {
        display: flex;
        justify-content: space-between;
        gap: 15px;
        margin: 10px 0;
    }

    .dato span:last-child {
        text-align: right;
        overflow-wrap: anywhere;
    }

    table {
        width: 100%;
        border-collapse: collapse;
        margin-top: 15px;
    }

    th {
        background-color: #075A38;
        color: white;
        padding: 12px 8px;
        text-align: left;
    }

    td {
        padding: 12px 8px;
        border-bottom: 1px solid #233E33;
        overflow-wrap: anywhere;
    }

    .numero {
        text-align: right;
    }

    .total {
        color: #39E989;
        font-size: 23px;
        font-weight: bold;
    }

    .agradecimiento {
        text-align: center;
        margin-top: 30px;
        color: #39E989;
        font-weight: bold;
    }

    .pie {
        text-align: center;
        font-size: 13px;
        color: #D3DBD7;
        margin-top: 10px;
    }

    @media print {
        body {
            background: white;
            color: black;
            padding: 0;
        }

        .ticket {
            background: white;
            color: black;
            border: 1px solid #999;
            box-shadow: none;
        }

        .marca, h2, .total, .agradecimiento {
            color: #075A38;
        }

        .subtitulo, .pie {
            color: #444;
        }

        th {
            background: #e6e6e6;
            color: black;
        }

        td {
            border-bottom: 1px solid #ccc;
        }
    }
</style>
</head>

<body>
<div class="ticket">

    <div class="marca">LA PULPE</div>

    <div class="subtitulo">
        Sistema de Gestion de Pulperia
    </div>

    <hr class="linea">

    <h2>Comprobante de Venta</h2>
)HTML";

    // INFORMACION DE LA VENTA

    archivo << "<div class=\"dato\"><span>Numero de venta:</span>"
            << "<strong>" << venta.getNumero()
            << "</strong></div>\n";

    archivo << "<div class=\"dato\"><span>Fecha:</span>"
            << "<span>" << escaparHTML(venta.getFecha())
            << "</span></div>\n";

    archivo << "<div class=\"dato\"><span>Cliente:</span>"
            << "<span>Consumidor final</span></div>\n";

    // TABLA DE PRODUCTOS

    archivo << R"HTML(
    <hr class="linea">

    <h2>Productos vendidos</h2>

    <table>
        <thead>
            <tr>
                <th>Producto</th>
                <th class="numero">Cant.</th>
                <th class="numero">Precio</th>
                <th class="numero">Subtotal</th>
            </tr>
        </thead>
        <tbody>
)HTML";

    // DETALLES DE LOS PRODUCTOS VENDIDOS

    vector<DetalleVenta> detalles = venta.getDetalles();

    for (const DetalleVenta& detalle : detalles)
    {
        archivo << "<tr>\n";

        archivo << "<td>"
                << escaparHTML(detalle.getNombreProducto())
                << "</td>\n";

        archivo << "<td class=\"numero\">"
                << detalle.getCantidad()
                << "</td>\n";

        archivo << "<td class=\"numero\">CRC "
                << detalle.getPrecioUnitario()
                << "</td>\n";

        archivo << "<td class=\"numero\">CRC "
                << detalle.subtotal()
                << "</td>\n";

        archivo << "</tr>\n";
    }

    // RESUMEN DEL COBRO

    archivo << R"HTML(
        </tbody>
    </table>

    <hr class="linea">

    <h2>Resumen del cobro</h2>
)HTML";

    archivo << "<div class=\"dato total\">"
            << "<span>Total:</span>"
            << "<span>CRC " << venta.getTotal()
            << "</span></div>\n";

    archivo << "<div class=\"dato\">"
            << "<span>Efectivo recibido:</span>"
            << "<span>CRC " << venta.getEfectivo()
            << "</span></div>\n";

    archivo << "<div class=\"dato\">"
            << "<span>Cambio:</span>"
            << "<span>CRC " << venta.getCambio()
            << "</span></div>\n";

    // PIE DEL COMPROBANTE

    archivo << R"HTML(
    <hr class="linea">

    <div class="agradecimiento">
        Gracias por su compra!
    </div>

    <div class="pie">
        Esperamos verlo nuevamente en La Pulpe.
    </div>

    <div class="pie">
        Comprobante generado por el sistema La Pulpe.
    </div>

</div>
</body>
</html>
)HTML";

    // VERIFICAR QUE EL ARCHIVO SE GUARDO

    archivo.flush();

    bool correcto = archivo.good();

    archivo.close();

    return correcto;
}
