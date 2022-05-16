#include "ReportingService.h"

ReportingService::ReportingService(ReportingRepository& reportingRepository, ExchangeRatesRepository& exchangeRatesRepository) : _reportingRepository(reportingRepository), _exchangeRatesRepository(exchangeRatesRepository) {

}

void ReportingService::SetBaseCurrency(int id) {
	_baseCurrencyId = id;
}

std::vector<StringValueViewModel> ReportingService::GetExpensesByAccount(const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<StringValueViewModel> results;

	return results;
}

std::vector<StringValueViewModel> ReportingService::GetExpensesByAccount(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<StringValueViewModel> results;

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetExpensesByMonth(const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetExpensesByMonth(std::set<int> accountsIds, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	return results;
}

std::vector<DateValueViewModel> ReportingService::GetBalanceByMonth(const AccountViewModel& account, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	return results;
}