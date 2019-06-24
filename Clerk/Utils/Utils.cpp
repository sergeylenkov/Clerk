#include "Utils.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}

wxString Utils::FormatAmount(float amount) {
	wxString number = wxNumberFormatter::ToString(amount, 2);
	return number;
}

wxString Utils::FormatAmount(float amount, Currency *currency) {
	wxString number = wxNumberFormatter::ToString(amount, 2);
	wxString sign = *currency->sign;

	return wxString::Format("%s %s", number, sign);
}

wxColor Utils::ColorForBudget(int percent) {
	if (percent > 50 && percent < 90) {
		return wxColor(251, 175, 67);
	} else if (percent >= 90) {
		return wxColor(242, 73, 101);
	}

	return wxColor(10, 199, 117);
}

wxColor Utils::ColorForGoal(int percent) {
	if (percent > 50 && percent < 90) {
		return wxColor(251, 175, 67);
	}
	else if (percent >= 90) {
		return wxColor(10, 199, 117);
	}

	return wxColor(242, 73, 101);
}

wxColor Utils::ColorForDebt(int percent) {
	if (percent > 50 && percent < 90) {
		return wxColor(251, 175, 67);
	}
	else if (percent >= 90) {
		return wxColor(10, 199, 117);
	}

	return wxColor(242, 73, 101);
}