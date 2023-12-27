#pragma once

#include <functional>
#include "../../Data/Services/AccountsService.h"
#include "../../Data/Services/TransactionsService.h"
#include "../../Data/Services/ReportsService.h"
#include "../../Utils/Types.h"
#include "../../Utils/EventEmitter.h"

namespace Clerk {
	namespace UI {
		class TreeMenuViewModel {
		public:
			TreeMenuViewModel(AccountsService& accountsService, ReportsService& reportsService, TransactionsService& transactionsService);
			~TreeMenuViewModel();

			shared_vector<AccountPresentationModel> GetReceiptsAccounts();
			shared_vector<AccountPresentationModel> GetDepositsAccounts();
			shared_vector<AccountPresentationModel> GetExpensesAccounts();
			shared_vector<AccountPresentationModel> GetDebtsAccounts();
			shared_vector<AccountPresentationModel> GetVirtualsAccounts();
			shared_vector<AccountPresentationModel> GetArchiveAccounts();
			shared_vector<ReportPresentationModel> GetReports();
			shared_vector<TransactionPresentationModel> GetRecentsTransactions(const AccountPresentationModel& account);
			bool IsTrashEmpty();

			void OnUpdate(std::function<void()> fn);

		private:
			AccountsService& _accountsService;
			TransactionsService& _transactionsService;
			ReportsService& _reportsService;
			EventEmitter* _eventEmitter;

			unsigned int _subscriptionId;
		};
	}
}
