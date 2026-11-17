
#include "GestorActividades.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDateTime>
#include <QString>
#include <QByteArray>

#include <limits>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// CONSTRUCTOR
// ==========================================

GestorActividades::GestorActividades()
{
    // Carpeta junto al ejecutable de La Pulpe
    carpetaRegistros =
        QDir(QCoreApplication::applicationDirPath())
        .filePath("RegistroActividades")
        .toStdString();

    fechaArchivoActual = "";

    // Crear carpeta y cargar registros de hoy
    if (crearCarpeta())
    {
        cargarActividades(obtenerFechaActual());
    }
}

// ==========================================
// OBTENER FECHA ACTUAL
// ==========================================

string GestorActividades::obtenerFechaActual() const
{
    return QDate::currentDate()
        .toString("yyyy-MM-dd")
        .toStdString();
}

// ==========================================
// OBTENER FECHA Y HORA ACTUAL
// ==========================================

string GestorActividades::obtenerFechaHora() const
{
    return QDateTime::currentDateTime()
        .toString("yyyy-MM-dd HH:mm:ss")
        .toStdString();
}

// ==========================================
// OBTENER RUTA DEL ARCHIVO
// ==========================================

string GestorActividades::obtenerRutaArchivo(
    const string& fecha) const
{
    QString nombreArchivo =
        "Actividades_" +
        QString::fromStdString(fecha) +
        ".json";

    return QDir(
        QString::fromStdString(carpetaRegistros)
    ).filePath(nombreArchivo).toStdString();
}

// ==========================================
// CREAR CARPETA DE REGISTROS
// ==========================================

bool GestorActividades::crearCarpeta() const
{
    QDir carpeta(
        QString::fromStdString(carpetaRegistros)
    );

    if (carpeta.exists())
    {
        return true;
    }

    return QDir().mkpath(
        QString::fromStdString(carpetaRegistros)
    );
}

// ==========================================
// OBTENER SIGUIENTE ID
// ==========================================

int GestorActividades::siguienteId() const
{
    int mayor = 0;

    for (const Actividad& actividad : actividades)
    {
        if (actividad.getId() > mayor)
        {
            mayor = actividad.getId();
        }
    }

    if (mayor == numeric_limits<int>::max())
    {
        return -1;
    }

    return mayor + 1;
}

// ==========================================
// CARGAR ACTIVIDADES DE UNA FECHA
// ==========================================

bool GestorActividades::cargarActividades(
    const string& fecha)
{
    // Validar fecha para evitar rutas incorrectas
    QString fechaQt = QString::fromStdString(fecha);

    QDate fechaValidada =
        QDate::fromString(fechaQt, "yyyy-MM-dd");

    if (!fechaValidada.isValid() ||
        fechaValidada.toString("yyyy-MM-dd") != fechaQt)
    {
        return false;
    }

    QString ruta = QString::fromStdString(
        obtenerRutaArchivo(fecha)
    );

    QFile archivo(ruta);

    // Si no existe, ese dia aun no tiene registros
    if (!archivo.exists())
    {
        actividades.clear();
        fechaArchivoActual = fecha;
        return true;
    }

    if (!archivo.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray contenido = archivo.readAll();
    archivo.close();

    QJsonParseError error;

    QJsonDocument documento =
        QJsonDocument::fromJson(contenido, &error);

    if (error.error != QJsonParseError::NoError ||
        !documento.isArray())
    {
        return false;
    }

    QJsonArray arreglo = documento.array();

    vector<Actividad> actividadesCargadas;

    for (const QJsonValue& valor : arreglo)
    {
        if (!valor.isObject())
        {
            return false;
        }

        QJsonObject objeto = valor.toObject();

        if (!objeto.value("id").isDouble() ||
            !objeto.value("idUsuario").isDouble() ||
            !objeto.value("nombreUsuario").isString() ||
            !objeto.value("tipo").isString() ||
            !objeto.value("descripcion").isString() ||
            !objeto.value("fechaHora").isString())
        {
            return false;
        }

        int id = objeto.value("id").toInt(-1);
        int idUsuario = objeto.value("idUsuario").toInt(-1);

        if (id <= 0 || idUsuario <= 0)
        {
            return false;
        }

        Actividad actividad(
            id,
            idUsuario,
            objeto.value("nombreUsuario")
                .toString().toStdString(),
            objeto.value("tipo")
                .toString().toStdString(),
            objeto.value("descripcion")
                .toString().toStdString(),
            objeto.value("fechaHora")
                .toString().toStdString()
        );

        actividadesCargadas.push_back(actividad);
    }

    // Actualizar solo si todo se leyo correctamente
    actividades = actividadesCargadas;
    fechaArchivoActual = fecha;

    return true;
}

// ==========================================
// GUARDAR ACTIVIDADES EN JSON
// ==========================================

bool GestorActividades::guardarActividades() const
{
    if (fechaArchivoActual.empty())
    {
        return false;
    }

    if (!crearCarpeta())
    {
        return false;
    }

    QJsonArray arreglo;

    for (const Actividad& actividad : actividades)
    {
        QJsonObject objeto;

        objeto["id"] = actividad.getId();
        objeto["idUsuario"] = actividad.getIdUsuario();

        objeto["nombreUsuario"] =
            QString::fromStdString(
                actividad.getNombreUsuario()
            );

        objeto["tipo"] =
            QString::fromStdString(
                actividad.getTipo()
            );

        objeto["descripcion"] =
            QString::fromStdString(
                actividad.getDescripcion()
            );

        objeto["fechaHora"] =
            QString::fromStdString(
                actividad.getFechaHora()
            );

        arreglo.append(objeto);
    }

    QString ruta = QString::fromStdString(
        obtenerRutaArchivo(fechaArchivoActual)
    );

    QJsonDocument documento(arreglo);

    QSaveFile archivo(ruta);

    if (!archivo.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QByteArray contenido = documento.toJson(
        QJsonDocument::Indented
    );

    if (archivo.write(contenido) != contenido.size())
    {
        archivo.cancelWriting();
        return false;
    }

    return archivo.commit();
}

// ==========================================
// REGISTRAR NUEVA ACTIVIDAD
// ==========================================

bool GestorActividades::registrarActividad(
    int idUsuario,
    string nombreUsuario,
    string tipo,
    string descripcion)
{
    if (idUsuario <= 0 ||
        nombreUsuario.empty() ||
        tipo.empty() ||
        descripcion.empty())
    {
        return false;
    }

    // Determinar la fecha correspondiente
    string fechaActual = obtenerFechaActual();

    // Recargar los registros antes de agregar
    if (!cargarActividades(fechaActual))
    {
        return false;
    }

    int nuevoId = siguienteId();

    if (nuevoId <= 0)
    {
        return false;
    }

    Actividad nuevaActividad(
        nuevoId,
        idUsuario,
        nombreUsuario,
        tipo,
        descripcion,
        obtenerFechaHora()
    );

    actividades.push_back(nuevaActividad);

    // Si falla el guardado, revertir
    if (!guardarActividades())
    {
        actividades.pop_back();
        return false;
    }

    return true;
}

// ==========================================
// LISTAR ACTIVIDADES CARGADAS
// ==========================================

vector<Actividad> GestorActividades::listarActividades() const
{
    return actividades;
}

// ==========================================
// BUSCAR ACTIVIDADES POR USUARIO
// ==========================================

vector<Actividad> GestorActividades::buscarPorUsuario(
    int idUsuario) const
{
    vector<Actividad> resultado;

    for (const Actividad& actividad : actividades)
    {
        if (actividad.getIdUsuario() == idUsuario)
        {
            resultado.push_back(actividad);
        }
    }

    return resultado;
}

// ==========================================
// BUSCAR ACTIVIDADES POR TIPO
// ==========================================

vector<Actividad> GestorActividades::buscarPorTipo(
    string tipo) const
{
    vector<Actividad> resultado;

    for (const Actividad& actividad : actividades)
    {
        if (actividad.getTipo() == tipo)
        {
            resultado.push_back(actividad);
        }
    }

    return resultado;
}

// ==========================================
// ACTUALIZAR REGISTROS DEL DIA ACTUAL
// ==========================================

bool GestorActividades::actualizarActividades()
{
    return cargarActividades(obtenerFechaActual());
}

// ==========================================
// CONSULTAR REGISTROS DE OTRA FECHA
// ==========================================

vector<Actividad> GestorActividades::listarPorFecha(
    string fecha)
{
    // Guardar el estado actual
    vector<Actividad> actividadesAnteriores = actividades;
    string fechaAnterior = fechaArchivoActual;

    vector<Actividad> resultado;

    if (cargarActividades(fecha))
    {
        resultado = actividades;
    }

    // Restaurar la lista que estaba cargada
    actividades = actividadesAnteriores;
    fechaArchivoActual = fechaAnterior;

    return resultado;
}
