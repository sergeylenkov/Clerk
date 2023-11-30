#include "ReportingService.h"

ReportingService::ReportingService(ReportingRepository& reportingRepository, CurrenciesService& currenciesService) :
	_reportingRepository(reportingRepository), _currenciesService(currenciesService) {

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

std::vector<DateValueViewModel> ReportingService::GetBalanceByMonth(const AccountPresentationModel& account, const wxDateTime& fromDate, const wxDateTime& toDate) {
	std::vector<DateValueViewModel> results;

	return results;
}