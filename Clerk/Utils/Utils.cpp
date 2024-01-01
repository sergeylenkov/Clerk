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

wxString Format::Date(const wxDateTime& date) {
	wxString dateFormat = date.Format("%B %e");

	if (wxDateTime::Now().GetYear() != date.GetYear()) {
		dateFormat = date.Format("%B %e, %Y");
	}

	return dateFormat;
}

wxString Format::DaysRemain(wxDateTime& date) {
	wxDateSpan diff = date.DiffAsDateSpan(wxDateTime::Now());

	int days = diff.GetTotalDays();
	int months = diff.GetTotalMonths();

	if (months > 0) {
		return wxString::Format("%d months", months);
	}

	return wxString::Format("%d days", days);
}

wxColor Colors::ColorForProgress(int percent, bool isReverse) {
	if (isReverse) {
		if (percent > 50 && percent < 90) {
			return wxColor(251, 175, 67);
		}
		else if (percent >= 90) {
			return wxColor(242, 73, 101);
		}

		return wxColor(10, 199, 117);
	}

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

wxColor Colors::ColorFoImportance(AlertImportance importance) {
	if (importance == AlertImportance::Medium) {
		return wxColor(251, 175, 67);
	}
	else if (importance == AlertImportance::High) {
		return wxColor(242, 73, 101);
	}

	return wxColor(10, 199, 117);
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

	case Periods::Type::ThreeMonths:
		fromDate.Subtract(wxDateSpan::wxDateSpan(0, 3, 0, 0));
		fromDate.SetDay(1);

		toDate.Subtract(wxDateSpan::wxDateSpan(0, 1, 0, 0));
		toDate.SetToLastMonthDay();
		
		break;

	case Periods::Type::SixMonths:
		fromDate.Subtract(wxDateSpan::wxDateSpan(0, 6, 0, 0));
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

std::vector<int> String::Split(std::wstring string, char delimiter) {
	std::vector<int> result;

	std::wstringstream stream(string);

	int i;

	while (stream >> i)
	{
		result.push_back(i);

		if (stream.peek() == delimiter) {
			stream.ignore();
		}
	}

	return result;
}

std::string String::Join(std::vector<std::string> const& vec, std::string delimiter) {
	if (vec.empty()) {
		return std::string();
	}

	return std::accumulate(vec.begin() + 1, vec.end(),
		vec[0],
		[delimiter](const std::string& a, std::string b) {
			return a + delimiter + b;
		});
}

bool String::Search(wxString str, wxString search) {
	auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());

	std::wstring strW = str.ToStdWstring();
	f.tolower(&strW[0], &strW[0] + strW.size());

	std::wstring searchW = search.ToStdWstring();
	f.tolower(&searchW[0], &searchW[0] + searchW.size());

	std::size_t found = strW.find(searchW);

	return found != std::string::npos;
}