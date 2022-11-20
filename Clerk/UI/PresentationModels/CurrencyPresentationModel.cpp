#include "CurrencyPresentationModel.h"

using namespace Clerk::UI;

CurrencyPresentationModel::CurrencyPresentationModel() {
	this->id = -1;
	this->name = "";
	this->shortName = "";
	this->sign = "";
}

CurrencyPresentationModel::CurrencyPresentationModel(CurrencyModel& currency) {
	this->id = currency.id;
	this->name = wxString::FromUTF8(currency.name.c_str());
	this->shortName = wxString::FromUTF8(currency.shortName.c_str());
	this->sign = wxString::FromUTF8(currency.sign.c_str());
}