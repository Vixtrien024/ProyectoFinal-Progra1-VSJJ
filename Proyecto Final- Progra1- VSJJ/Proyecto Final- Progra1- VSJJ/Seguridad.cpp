#include "Seguridad.h"

#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QByteArray>

using namespace std;

namespace {
    const string PREFIJO = "sha256";
    const int LONGITUD_SAL = 16;
}

// Generar sal aleatoria
string Seguridad::generarSal() {
    QByteArray sal(LONGITUD_SAL, Qt::Uninitialized);

    for (int i = 0; i < LONGITUD_SAL; ++i) {
        sal[i] = static_cast<char>(
            QRandomGenerator::global()->bounded(256));
    }

    return sal.toHex().toStdString();
}

// Calcular hash SHA-256 de sal + clave
string Seguridad::calcularHash(
    const string& clave, const string& sal) {

    QByteArray datos =
        QByteArray::fromStdString(sal + clave);

    return QCryptographicHash::hash(
        datos, QCryptographicHash::Sha256)
        .toHex()
        .toStdString();
}

// Crear hash con formato sha256$sal$hash
string Seguridad::crearHashClave(const string& clave) {
    string sal = generarSal();
    string hash = calcularHash(clave, sal);

    return PREFIJO + "$" + sal + "$" + hash;
}

// Verificar clave contra el hash almacenado
bool Seguridad::verificarClave(
    const string& clave,
    const string& hashAlmacenado) {

    size_t primerSeparador = hashAlmacenado.find('$');

    if (primerSeparador == string::npos) {
        return false;
    }

    size_t segundoSeparador =
        hashAlmacenado.find('$', primerSeparador + 1);

    if (segundoSeparador == string::npos) {
        return false;
    }

    string prefijo =
        hashAlmacenado.substr(0, primerSeparador);

    string sal = hashAlmacenado.substr(
        primerSeparador + 1,
        segundoSeparador - primerSeparador - 1);

    string hash =
        hashAlmacenado.substr(segundoSeparador + 1);

    if (prefijo != PREFIJO || sal.empty() || hash.empty()) {
        return false;
    }

    string calculado = calcularHash(clave, sal);

    if (calculado.size() != hash.size()) {
        return false;
    }

    // Comparacion en tiempo constante
    unsigned char diferencia = 0;

    for (size_t i = 0; i < hash.size(); ++i) {
        diferencia |= static_cast<unsigned char>(
            calculado[i] ^ hash[i]);
    }

    return diferencia == 0;
}