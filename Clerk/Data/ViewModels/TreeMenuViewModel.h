#pragma once

#include "../Services/AccountsService.h"
#include "../Services/TransactionsService.h"
#include "../Services/ReportsService.h"
#include "../ViewModels/AccountViewModel.h"
#include "../ViewModels/ReportViewModel.h"

namespace Clerk {
	namespace Data {
		class TreeMenuViewModel {
		public:
			TreeMenuViewModel(AccountsService& accountsService, ReportsService& reportsService, TransactionsService& transactionsService);

			std::vector<std::shared_ptr<AccountViewModel>> GetReceiptsAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetDepositsAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetExpensesAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetDebtsAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetVirtualsAccounts();
			std::vector<std::shared_ptr<AccountViewModel>> GetArchiveAccounts();
			std::vector<std::shared_ptr<ReportViewModel>> GetReports();
			std::vector<std::shared_ptr<TransactionViewModel>> GetRecentsTransactions(const AccountViewModel& account);
			bool IsTrashEmpty();

		private:
			AccountsService& _accountsService;
			TransactionsService& _transactionsService;
			ReportsService& _reportsService;
		};
	}
}
