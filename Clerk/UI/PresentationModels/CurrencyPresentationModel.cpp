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
	this->name = currency.name;
	this->shortName = currency.shortName;
	this->sign = currency.sign;
}