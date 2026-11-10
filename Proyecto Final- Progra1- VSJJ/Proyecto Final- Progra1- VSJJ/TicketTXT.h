#pragma once
#include "GeneradorTicket.h"

using namespace std;

class TicketTXT : public GeneradorTicket
{
public:
    bool generar(const Venta& venta) override;
};