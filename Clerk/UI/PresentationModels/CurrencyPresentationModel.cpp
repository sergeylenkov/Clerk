#include "CurrencyPresentationModel.h"

using namespace Clerk::UI;

CurrencyPresentationModel::CurrencyPresentationModel() {
	id = -1;
	name = wxString("");
	shortName = wxString("");
	sign = wxString("");
}

CurrencyPresentationModel::CurrencyPresentationModel(CurrencyModel& model) {
	id = model.id;
	name = wxString(model.name);
	shortName = wxString(model.shortName);
	sign = wxString(model.sign);
}