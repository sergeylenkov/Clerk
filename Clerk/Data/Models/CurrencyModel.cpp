#include "CurrencyModel.h"

using namespace Clerk::Data;

CurrencyModel::CurrencyModel()
{
	this->id = -1;
	this->name = std::wstring();
	this->shortName = std::wstring();
	this->sign = std::wstring();
}