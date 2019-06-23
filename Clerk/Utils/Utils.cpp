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