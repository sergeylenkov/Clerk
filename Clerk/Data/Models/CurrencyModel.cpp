#include "CurrencyModel.h"

using namespace Clerk::Data;

CurrencyModel::CurrencyModel()
{
	this->id = -1;
	this->name = std::string();
	this->shortName = std::string();
	this->sign = std::string();
}