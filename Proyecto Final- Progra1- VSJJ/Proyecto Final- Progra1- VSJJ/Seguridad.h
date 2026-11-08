
#pragma once

#include <string>

using namespace std;

class Seguridad {
public:
    // Crea un hash seguro con sal a partir de la contrasena
    static string crearHashClave(const string& clave);

    // Verifica una contrasena contra el hash almacenado
    static bool verificarClave(
        const string& clave,
        const string& hashAlmacenado);

private:
    // Genera una sal aleatoria
    static string generarSal();

    // Calcula SHA-256 de sal + clave
    static string calcularHash(
        const string& clave,
        const string& sal);
};
