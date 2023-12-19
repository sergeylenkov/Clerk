#pragma once

#include <optional>
#include "ICommandsReceiver.h"
#include "../UI/Controllers/DialogsController.h"
#include "../UI/Controllers/TabsController.h"

using namespace Clerk::UI;
using namespace Clerk::Data;

namespace Clerk {
	namespace Commands {
		class CommandsReceiver: public ICommandsReceiver {
		public:
			CommandsReceiver();
			CommandsReceiver(DialogsController* dialogsController, TabsController* tabsController);

			void OpenPreferencesDialog() override;
			void OpenAboutDialog() override;
			void NewTransaction(int id) override;
			void CopyTransaction(int id) override;
			void SplitTransaction(int id) override;
			void EditTransaction(int id) override;
			void OpenNewAccountDialog(AccountType type) override;
			void OpenEditAccountDialog(int id) override;
			void OpenTab(TabType type) override;
			void OpenAccountTab(int idd) override;
			void OpenAccountsTab(std::optional<AccountType> type) override;
			void OpenReportTab(int id) override;
			void OpenEditAlertDialog(int id) override;
			void OpenEditSchedulerDialog(int id) override;

		private:
			DialogsController* _dialogsController;
			TabsController* _tabsController;
		};
	}
}