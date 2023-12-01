#pragma once

#include <wx/wx.h>
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
			CommandsReceiver(wxFrame* frame, DialogsController* dialogsController, TabsController* tabsController);

			void Quit() override;
			void OpenPreferencesDialog() override;
			void OpenAboutDialog() override;
			void NewTransaction(int id) override;
			void CopyTransaction(int id) override;
			void SplitTransaction(int id) override;
			void EditTransaction(int id) override;
			void DeleteTransaction(int id) override;
			void OpenNewAccountDialog(AccountType type) override;
			void OpenEditAccountDialog(int id) override;
			void OpenNewTab(TabType type) override;
			void OpenNewAccountTab(int accountId) override;
			void OpenNewAccountsTab(std::optional<AccountType> type) override;

		private:
			wxFrame* _frame{};
			DialogsController* _dialogsController;
			TabsController* _tabsController;
		};
	}
}