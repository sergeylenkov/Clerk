#include "Currency.h"

using namespace Clerk::Data;

Currency::Currency()
{
	this->id = -1;
	this->name = std::make_shared<wxString>();
	this->shortName = std::make_shared<wxString>();
	this->sign = std::make_shared<wxString>();
}