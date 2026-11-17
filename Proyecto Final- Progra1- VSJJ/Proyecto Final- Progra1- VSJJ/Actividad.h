
#pragma once

#include <string>

using namespace std;

// Representa una actividad realizada en La Pulpe
class Actividad {
private:
    int id;
    int idUsuario;
    string nombreUsuario;
    string tipo;
    string descripcion;
    string fechaHora;

public:
    // Constructor vacio
    Actividad();

    // Constructor con parametros
    Actividad(
        int id,
        int idUsuario,
        string nombreUsuario,
        string tipo,
        string descripcion,
        string fechaHora
    );

    // Getters
    int getId() const;
    int getIdUsuario() const;
    string getNombreUsuario() const;
    string getTipo() const;
    string getDescripcion() const;
    string getFechaHora() const;

    // Setters
    void setId(int id);
    void setIdUsuario(int idUsuario);
    void setNombreUsuario(string nombreUsuario);
    void setTipo(string tipo);
    void setDescripcion(string descripcion);
    void setFechaHora(string fechaHora);
};
