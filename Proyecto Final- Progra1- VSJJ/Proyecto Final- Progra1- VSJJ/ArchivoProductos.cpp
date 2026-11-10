#include "ArchivoProductos.h"

#include <fstream>
#include <sstream>

// Constructor
ArchivoProductos::ArchivoProductos(const string& ruta)
    : ruta(ruta) {
}//fin del constructor


// Cargar los productos desde el archivo
vector<Producto> ArchivoProductos::cargar() {

    vector<Producto> productos;
    ifstream entrada(ruta);

    if (!entrada.is_open()) {//La primera vez el archivo todavia no existe
        return productos;
    }//fin del if

    string linea;
    while (getline(entrada, linea)) {

        if (linea.empty()) continue;

        // Separar la linea en campos usando ';'
        vector<string> campos;
        stringstream ss(linea);
        string campo;
        while (getline(ss, campo, ';')) {
            campos.push_back(campo);
        }//fin del while interno

        if (campos.size() != 6) continue;//Linea danada: se ignora

        try {
            int codigo = stoi(campos[0]);
            qint64 precio = stoll(campos[2]);
            int existencias = stoi(campos[4]);

            Producto producto(codigo, campos[1], precio, campos[3], existencias);

            if (campos[5] != "1") {//1 = activo, 0 = desactivado
                producto.desactivar();
            }//fin del if

            productos.push_back(producto);
        }//fin del try
        catch (...) {
            continue;//Un numero invalido: se ignora esa linea
        }//fin del catch

    }//fin del while

    return productos;
}//fin de cargar


// Guardar todos los productos en el archivo
bool ArchivoProductos::guardar(const vector<Producto>& productos) {

    ofstream salida(ruta, ios::trunc);//trunc: borra lo anterior y escribe de nuevo

    if (!salida.is_open()) {
        return false;
    }//fin del if

    for (const Producto& p : productos) {

        // Un ';' o un salto de linea dentro de un texto romperia el formato
        string nombre = p.getNombre();
        string categoria = p.getCategoria();
        for (char& c : nombre) {
            if (c == ';') c = ',';
            if (c == '\n' || c == '\r') c = ' ';
        }//fin del for nombre
        for (char& c : categoria) {
            if (c == ';') c = ',';
            if (c == '\n' || c == '\r') c = ' ';
        }//fin del for categoria

        salida << p.getCodigo() << ';'
            << nombre << ';'
            << p.getPrecio() << ';'
            << categoria << ';'
            << p.getExistencias() << ';'
            << (p.estaActivo() ? 1 : 0) << '\n';
    }//fin del for

    return salida.good();
}//fin de guardar