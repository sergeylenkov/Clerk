#pragma once

#include <wx/wx.h>
#include "ICommandsReceiver.h"
#include "../UI/Controllers/DialogsController.h"

using namespace Clerk::UI;

namespace Clerk {
	namespace Commands {
		class CommandsReceiver: public ICommandsReceiver {
		public:
			CommandsReceiver();
			CommandsReceiver(wxFrame* frame, DialogsController* dialogsController);

			void Quit() override;
			void OpenPreferencesDialog() override;
			void OpenAboutDialog() override;
			void OpenNewTransactionDialog(int id) override;
			void OpenCopyTransactionDialog(int id) override;
			void OpenEditAccountDialog(int id) override;

		private:
			wxFrame* _frame{};
			DialogsController* _dialogsController{};
		};
	}
}