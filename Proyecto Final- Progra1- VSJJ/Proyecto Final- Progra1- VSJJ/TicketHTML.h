#pragma once
#include "GeneradorTicket.h"
using namespace std;

class TicketHTML : public GeneradorTicket
{
public:
	bool generar(const Venta& venta) override;
}; 