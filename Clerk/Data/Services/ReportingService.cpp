#include "ReportingService.h"

ReportingService::ReportingService(ReportingRepository& reportingRepository, CurrenciesService& currenciesService) :
	_reportingRepository(reportingRepository), _currenciesService(currenciesService) {

}

std::vector<StringValueViewModel> ReportingService::GetExpensesByAccount(const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<StringValueViewModel> results;

	auto expenses = _reportingRepository.GetExpensesByAccount(fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto expense : expenses) {
		results.push_back({ wxString(expense.first), expense.second });
	}

	return results;
}

std::vector<StringValueViewModel> ReportingService::GetExpensesByAccount(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<StringValueViewModel> results;

	auto expenses = _reportingRepository.GetExpensesByAccount(String::Join(accountsIds, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto expense : expenses) {
		results.push_back({ wxString(expense.first), expense.second });
	}

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetExpensesByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	auto expenses = _reportingRepository.GetExpensesByMonth(String::Join(accountsIds, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto expense : expenses) {
		wxDateTime date = wxDateTime::Today();
		date.ParseISODate(expense.first);

		results.push_back({ date, expense.second });
	}

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetReceiptsByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	auto receipts = _reportingRepository.GetReceiptsByMonth(String::Join(accountsIds, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto receipt : receipts) {
		wxDateTime date = wxDateTime::Today();
		date.ParseISODate(receipt.first);

		results.push_back({ date, receipt.second });
	}

	return results;
}

std::vector<StringValueViewModel> ReportingService::GetReceiptsByAccount(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<StringValueViewModel> results;

	auto receipts = _reportingRepository.GetReceiptsByAccount(String::Join(accountsIds, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto receipt : receipts) {
		results.push_back({ wxString(receipt.first), receipt.second });
	}

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetBalanceByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	wxDateSpan diff = toDate.DiffAsDateSpan(fromDate);
	wxDateTime date = wxDateTime(fromDate);

	for (int i = 0; i < diff.GetMonths(); i++) {
		date = date.Add(wxDateSpan(0, 1, 0, 0));
		date.SetDay(1);
		date.SetToLastMonthDay();

		float balance = _reportingRepository.GetBalanceForDate(String::Join(accountsIds, ","), date.FormatISODate().ToStdString());

		results.push_back({ date, balance });
	}

	return results;
}