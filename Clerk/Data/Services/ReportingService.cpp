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
	std::vector<std::string> res;

	for (int id : accountsIds) {
		res.push_back(std::to_string(id));
	}

	auto expenses = _reportingRepository.GetExpensesByAccount(String::Split(res, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto expense : expenses) {
		results.push_back({ wxString(expense.first), expense.second });
	}

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetExpensesByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;
	std::vector<std::string> res;

	for (int id : accountsIds) {
		res.push_back(std::to_string(id));
	}

	auto expenses = _reportingRepository.GetExpensesByMonth(String::Split(res, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto expense : expenses) {
		wxDateTime date = wxDateTime::Today();
		date.ParseISODate(expense.first);

		results.push_back({ date, expense.second });
	}

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetBalanceByMonth(const AccountPresentationModel& account, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetReceiptsByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;
	std::vector<std::string> res;

	for (int id : accountsIds) {
		res.push_back(std::to_string(id));
	}

	auto receipts = _reportingRepository.GetReceiptsByMonth(String::Split(res, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto receipt : receipts) {
		wxDateTime date = wxDateTime::Today();
		date.ParseISODate(receipt.first);

		results.push_back({ date, receipt.second });
	}

	return results;
}

std::vector<StringValueViewModel> ReportingService::GetReceiptsByAccount(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<StringValueViewModel> results;
	std::vector<std::string> res;

	for (int id : accountsIds) {
		res.push_back(std::to_string(id));
	}

	auto receipts = _reportingRepository.GetReceiptsByAccount(String::Split(res, ","), fromDate.FormatISODate().ToStdString(), toDate.FormatISODate().ToStdString());

	for (auto receipt : receipts) {
		results.push_back({ wxString(receipt.first), receipt.second });
	}

	return results;
}