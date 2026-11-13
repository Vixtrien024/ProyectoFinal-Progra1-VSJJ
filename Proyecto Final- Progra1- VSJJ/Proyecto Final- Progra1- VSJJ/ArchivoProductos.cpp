
#include "ArchivoProductos.h"

#include <fstream>
#include <sstream>

#include <QSaveFile>
#include <QString>
#include <QByteArray>

// ================= CONSTRUCTOR =================

ArchivoProductos::ArchivoProductos(const string& ruta)
    : ruta(ruta) {
}

// ================= CARGAR PRODUCTOS =================

vector<Producto> ArchivoProductos::cargar() {

    vector<Producto> productos;

    ifstream entrada(ruta);

    // Si el archivo no existe, devolver lista vacia
    if (!entrada.is_open()) {
        return productos;
    }

    string linea;

    while (getline(entrada, linea)) {

        if (linea.empty()) {
            continue;
        }

        vector<string> campos;
        stringstream ss(linea);
        string campo;

        // Separar los datos utilizando ;
        while (getline(ss, campo, ';')) {
            campos.push_back(campo);
        }

        // Cada producto debe tener 6 campos
        if (campos.size() != 6) {
            continue;
        }

        try {

            int codigo = stoi(campos[0]);
            qint64 precio = stoll(campos[2]);
            int existencias = stoi(campos[4]);

            // Validar valores
            if (codigo <= 0 ||
                campos[1].empty() ||
                precio <= 0 ||
                existencias < 0) {
                continue;
            }

            // Validar estado
            if (campos[5] != "0" &&
                campos[5] != "1") {
                continue;
            }

            Producto producto(
                codigo,
                campos[1],
                precio,
                campos[3],
                existencias
            );

            // Recuperar el estado del producto
            if (campos[5] == "0") {
                producto.desactivar();
            }

            productos.push_back(producto);
        }
        catch (...) {

            // Ignorar registros con numeros invalidos
            continue;
        }
    }

    entrada.close();

    return productos;
}

// ================= GUARDAR PRODUCTOS =================

bool ArchivoProductos::guardar(
    const vector<Producto>& productos) {

    // Preparar todos los datos en memoria
    ostringstream contenido;

    for (const Producto& p : productos) {

        string nombre = p.getNombre();
        string categoria = p.getCategoria();

        // Evitar caracteres que dañen el formato
        for (char& c : nombre) {

            if (c == ';') {
                c = ',';
            }

            if (c == '\n' || c == '\r') {
                c = ' ';
            }
        }

        for (char& c : categoria) {

            if (c == ';') {
                c = ',';
            }

            if (c == '\n' || c == '\r') {
                c = ' ';
            }
        }

        // Escribir los seis campos
        contenido
            << p.getCodigo() << ';'
            << nombre << ';'
            << p.getPrecio() << ';'
            << categoria << ';'
            << p.getExistencias() << ';'
            << (p.estaActivo() ? 1 : 0)
            << '\n';

        if (!contenido.good()) {
            return false;
        }
    }

    // Abrir el archivo de forma segura
    QSaveFile archivo(
        QString::fromStdString(ruta)
    );

    if (!archivo.open(QIODevice::WriteOnly)) {
        return false;
    }

    // Convertir el contenido a bytes
    string texto = contenido.str();

    QByteArray datos(
        texto.data(),
        static_cast<qsizetype>(texto.size())
    );

    // Escribir todos los datos
    qint64 escritos = archivo.write(datos);

    if (escritos != datos.size()) {

        archivo.cancelWriting();
        return false;
    }

    // Confirmar la escritura
    if (!archivo.commit()) {
        return false;
    }

    return true;
}
