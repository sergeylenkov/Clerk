#pragma once

#include <optional>
#include "../Data/Enums.h"
#include "../UI/Tabs/Enums.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class ICommandsReceiver {
		public:
			virtual void Quit() = 0;			
			virtual void OpenPreferencesDialog() = 0;
			virtual void OpenAboutDialog() = 0;
			virtual void NewTransaction(int id) = 0;
			virtual void CopyTransaction(int id) = 0;
			virtual void SplitTransaction(int id) = 0;
			virtual void EditTransaction(int id) = 0;
			virtual void DeleteTransaction(int id) = 0;
			virtual void OpenNewAccountDialog(AccountType type) = 0;
			virtual void OpenEditAccountDialog(int id) = 0;
			virtual void OpenTab(TabType type) = 0;
			virtual void OpenAccountTab(int accountId) = 0;
			virtual void OpenAccountsTab(std::optional<AccountType> type) = 0;
			virtual void OpenReportTab(int id) = 0;
			virtual void OpenEditAlertDialog(int id) = 0;
			virtual void OpenEditSchedulerDialog(int id) = 0;
		};
	}
}