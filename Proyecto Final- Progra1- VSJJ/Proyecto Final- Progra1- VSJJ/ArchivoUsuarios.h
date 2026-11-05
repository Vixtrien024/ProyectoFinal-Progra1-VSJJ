
#pragma once

#include <string>
#include <vector>
#include "Usuario.h"

using namespace std;

class ArchivoUsuarios {
private:
    string ruta;

public:
    // Constructor
    ArchivoUsuarios(string ruta);

    // Obtener la ruta del archivo
    string getRuta() const;

    // Cambiar la ruta del archivo
    void setRuta(string ruta);

    // Guardar los usuarios en JSON
    bool guardar(const vector<Usuario>& usuarios);

    // Cargar los usuarios desde JSON
    vector<Usuario> cargar();
};
