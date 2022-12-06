#include "TreeMenuViewModel.h"

using namespace Clerk::UI;

TreeMenuViewModel::TreeMenuViewModel(AccountsService& accountsService, ReportsService& reportsService, TransactionsService& transactionsService) :
	_accountsService(accountsService),
	_reportsService(reportsService),
	_transactionsService(transactionsService)
{

}

std::vector<AccountPresentationModel*> TreeMenuViewModel::GetReceiptsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Receipt);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<AccountPresentationModel*> TreeMenuViewModel::GetDepositsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Deposit);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<AccountPresentationModel*> TreeMenuViewModel::GetExpensesAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Expens);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<AccountPresentationModel*> TreeMenuViewModel::GetDebtsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Debt);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<AccountPresentationModel*> TreeMenuViewModel::GetVirtualsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Virtual);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<AccountPresentationModel*> TreeMenuViewModel::GetArchiveAccounts() {
	return _accountsService.GetArchive();
}

std::vector<std::shared_ptr<ReportPresentationModel>> TreeMenuViewModel::GetReports() {
	return _reportsService.GetAll();
}

std::vector<std::shared_ptr<TransactionPresentationModel>> TreeMenuViewModel::GetRecentsTransactions(const AccountPresentationModel& account) {
	return _transactionsService.GetRecents(account, 10);
}

bool TreeMenuViewModel::IsTrashEmpty() {
	return _transactionsService.GetDeleted().size() == 0;
}