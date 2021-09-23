#include "TreeMenuViewModel.h"

using namespace Clerk::Data;

TreeMenuViewModel::TreeMenuViewModel(AccountsService& accountsService, ReportsService& reportsService, TransactionsService& transactionsService) :
	_accountsService(accountsService),
	_reportsService(reportsService),
	_transactionsService(transactionsService)
{

}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetReceiptsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Receipt);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetDepositsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Deposit);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetExpensesAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Expens);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetDebtsAccounts() {	 
	auto accounts = _accountsService.GetByType(AccountType::Debt);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetVirtualsAccounts() {
	auto accounts = _accountsService.GetByType(AccountType::Virtual);

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	return accounts;
}

std::vector<std::shared_ptr<AccountViewModel>> TreeMenuViewModel::GetArchiveAccounts() {
	return _accountsService.GetArchive();
}

std::vector<std::shared_ptr<ReportViewModel>> TreeMenuViewModel::GetReports() {
	return _reportsService.GetAll();
}

std::vector<std::shared_ptr<TransactionViewModel>> TreeMenuViewModel::GetRecentsTransactions(const AccountViewModel& account) {
	return _transactionsService.GetRecents(account, 10);
}

bool TreeMenuViewModel::IsTrashEmpty() {
	return _transactionsService.GetDeleted().size() == 0;
}