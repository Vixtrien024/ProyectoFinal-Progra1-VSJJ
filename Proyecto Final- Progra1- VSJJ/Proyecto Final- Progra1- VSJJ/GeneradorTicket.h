#pragma once
#include "Venta.h"

using namespace std;

class GeneradorTicket
{
public:
    virtual bool generar(const Venta& venta) = 0;

    virtual ~GeneradorTicket() = default;
};
