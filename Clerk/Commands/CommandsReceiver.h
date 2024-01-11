#pragma once

#include <optional>
#include "ICommandsReceiver.h"
#include "../UI/Controllers/DialogsController.h"
#include "../UI/Tabs/TabsPanel.h"

using namespace Clerk::UI;
using namespace Clerk::Data;

namespace Clerk {
	namespace Commands {
		class CommandsReceiver: public ICommandsReceiver {
		public:
			CommandsReceiver(DialogsController& dialogsController, TabsPanel& tabsPanel);

			void OpenTab(TabType type) override;
			void OpenAccountTab(int idd) override;
			void OpenAccountsTab(std::optional<AccountType> type) override;
			void OpenReportTab(int id) override;
			void OpenPreferencesDialog() override;
			void OpenAboutDialog() override;			
			void NewTransaction(int id) override;
			void CopyTransaction(int id) override;
			void SplitTransaction(int id) override;
			void EditTransaction(int id) override;
			void OpenNewAccountDialog(AccountType type) override;
			void OpenEditAccountDialog(int id) override;			
			void OpenNewAlertDialog() override;
			void OpenEditAlertDialog(int id) override;			
			void OpenNewBudgetDialog() override;
			void OpenEditBudgetDialog(int id) override;
			void OpenNewGoalDialog() override;
			void OpenEditGoalDialog(int id) override;
			void OpenNewSchedulerDialog() override;
			void OpenEditSchedulerDialog(int id) override;

		private:
			DialogsController& _dialogsController;
			TabsPanel& _tabsPanel;
		};
	}
}