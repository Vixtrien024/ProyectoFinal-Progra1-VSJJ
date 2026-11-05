
#include "ArchivoUsuarios.h"

#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;

using json = nlohmann::json;

// Constructor
ArchivoUsuarios::ArchivoUsuarios(string ruta) {
    this->ruta = ruta;
}

// Getter
string ArchivoUsuarios::getRuta() const {
    return ruta;
}

// Setter
void ArchivoUsuarios::setRuta(string ruta) {
    this->ruta = ruta;
}

// Guardar usuarios en JSON
bool ArchivoUsuarios::guardar(const vector<Usuario>& usuarios) {

    if (ruta.empty()) {
        return false;
    }

    try {
        json datos = json::array();

        for (const Usuario& usuario : usuarios) {

            json objeto;

            objeto["id"] = usuario.getId();
            objeto["nombre"] = usuario.getNombre();
            objeto["nombreUsuario"] = usuario.getNombreUsuario();
            objeto["claveHash"] = usuario.getClaveHash();

            objeto["rol"] =
                usuario.getRol() == Rol::Administrador
                ? "Administrador"
                : "Cajero";

            objeto["activo"] = usuario.estaActivo();

            datos.push_back(objeto);
        }

        ofstream archivo(
            ruta, ios::binary | ios::trunc);

        if (!archivo.is_open()) {
            return false;
        }

        archivo << datos.dump(4);
        archivo.close();

        return !archivo.fail();
    }
    catch (...) {
        return false;
    }
}

// Cargar usuarios desde JSON
vector<Usuario> ArchivoUsuarios::cargar() {

    vector<Usuario> usuarios;

    if (ruta.empty()) {
        return usuarios;
    }

    ifstream archivo(ruta, ios::binary);

    if (!archivo.is_open()) {
        return usuarios;
    }

    try {
        json datos;
        archivo >> datos;

        if (!datos.is_array()) {
            return usuarios;
        }

        for (const auto& objeto : datos) {

            if (!objeto.is_object()) {
                continue;
            }

            try {
                int id = objeto.at("id").get<int>();

                string nombre =
                    objeto.at("nombre").get<string>();

                string nombreUsuario =
                    objeto.at("nombreUsuario").get<string>();

                string claveHash =
                    objeto.at("claveHash").get<string>();

                string nombreRol =
                    objeto.at("rol").get<string>();

                bool activo =
                    objeto.at("activo").get<bool>();

                Rol rol;

                if (nombreRol == "Administrador") {
                    rol = Rol::Administrador;
                }
                else if (nombreRol == "Cajero") {
                    rol = Rol::Cajero;
                }
                else {
                    continue;
                }

                Usuario usuario(
                    id,
                    nombre,
                    nombreUsuario,
                    claveHash,
                    rol,
                    activo
                );

                usuarios.push_back(usuario);
            }
            catch (...) {
                // Saltar registros incompletos o invalidos
                continue;
            }
        }
    }
    catch (...) {
        return {};
    }

    return usuarios;
}
