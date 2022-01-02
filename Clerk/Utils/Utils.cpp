#include "Utils.h"

using namespace Clerk::Utils;

wxString Format::Amount(float amount) {
	wxString number = wxNumberFormatter::ToString(amount, 2);
	return number;
}

wxString Format::Amount(float amount, wxString& sign) {
	wxString number = wxNumberFormatter::ToString(amount, 2);	

	return wxString::Format("%s %s", number, sign);
}

wxColor Colors::ColorForBudget(int percent) {
	if (percent > 50 && percent < 90) {
		return wxColor(251, 175, 67);
	} else if (percent >= 90) {
		return wxColor(242, 73, 101);
	}

	return wxColor(10, 199, 117);
}

wxColor Colors::ColorForGoal(int percent) {
	if (percent > 50 && percent < 90) {
		return wxColor(251, 175, 67);
	}
	else if (percent >= 90) {
		return wxColor(10, 199, 117);
	}

	return wxColor(242, 73, 101);
}

wxColor Colors::ColorForDebt(int percent) {
	if (percent > 50 && percent < 90) {
		return wxColor(251, 175, 67);
	}
	else if (percent >= 90) {
		return wxColor(10, 199, 117);
	}

	return wxColor(242, 73, 101);
}

wxColor Colors::ColorForBalance(boolean isReceipt) {
	if (isReceipt) {
		return wxColor(10, 199, 117);
	}

	return wxColor(242, 73, 101);
}

void Periods::Calculate(Periods::Type type, wxDateTime &fromDate, wxDateTime &toDate) {
	switch (type)
	{
	case Periods::Type::CurrentWeek:
			fromDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);
			toDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Sun);
		break;

	case Periods::Type::PreviousWeek:
			fromDate.SetToPrevWeekDay(wxDateTime::WeekDay::Sun);
			fromDate.SetToWeekDayInSameWeek(wxDateTime::WeekDay::Mon);

			toDate.SetToPrevWeekDay(wxDateTime::WeekDay::Sun);
			break;		

	case Periods::Type::CurrentMonth:
			fromDate.SetDay(1);
			toDate.SetToLastMonthDay();
			break;

	case Periods::Type::PreviousMonth:
			fromDate.Subtract(wxDateSpan::wxDateSpan(0, 1, 0, 0));
			fromDate.SetDay(1);

			toDate.Subtract(wxDateSpan::wxDateSpan(0, 1, 0, 0));
			toDate.SetToLastMonthDay();
			break;

	case Periods::Type::CurrentYear:
			fromDate.SetMonth(wxDateTime::Month::Jan);
			fromDate.SetDay(1);
			toDate.SetMonth(wxDateTime::Month::Dec);
			toDate.SetDay(31);
			break;

	case Periods::Type::PreviousYear:
			fromDate.Subtract(wxDateSpan::wxDateSpan(1, 0, 0, 0));
			fromDate.SetMonth(wxDateTime::Month::Jan);
			fromDate.SetDay(1);

			toDate.Subtract(wxDateSpan::wxDateSpan(1, 0, 0, 0));
			toDate.SetMonth(wxDateTime::Month::Dec);
			toDate.SetToLastMonthDay(wxDateTime::Month::Dec);
			break;

		default:
			break;
	}
}