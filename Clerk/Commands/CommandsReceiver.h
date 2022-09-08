#pragma once

#include <wx/wx.h>
#include "ICommandsReceiver.h"
#include "../UI/Controllers/DialogsController.h"
#include "../UI/Controllers/tabsController.h"

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
			void OpenNewTransactionDialog(int id) override;
			void OpenCopyTransactionDialog(int id) override;
			void OpenNewAccountDialog(AccountType type) override;
			void OpenEditAccountDialog(int id) override;
			void OpenNewTab(TabType type) override;

		private:
			wxFrame* _frame{};
			DialogsController* _dialogsController;
			TabsController* _tabsController;
		};
	}
}