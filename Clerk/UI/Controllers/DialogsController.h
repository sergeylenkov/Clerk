#pragma once

#include <wx/wx.h>
#include "../AboutDialog.h"
#include "../PreferencesDialog.h"
#include "../Transactions/TransactionDialog.h"
#include "../AccountDialog.h"
#include "../ViewModels/TransactionViewModel.h"
#include "../ViewModels/AccountViewModel.h"
#include "../../Data/DataContext.h"
#include "../../Utils/Icons.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class DialogsController {
		public:
			DialogsController(DataContext& context, Icons& icons);

			void SetMainWindow(wxFrame* frame);
			void ShowAboutDialog();
			void ShowPreferencesDialog();
			void ShowNewTransactionDialog(int id);
			void ShowCopyTransactionDialog(int id);
			void ShowSplitTransactionDialog(int id);
			void ShowEditTransactionDialog(int id);
			void DeleteTransaction(int id);
			void ShowNewAccountDialog(AccountType type);
			void ShowEditAccountDialog(int id);
			void ShowEditAlertDialog(int id);

		private:
			wxFrame* _parent;
			DataContext& _context;
			Icons& _icons;

			wxSize GetAboutDialogSize();
			wxSize GetTransactionDialogSize();
			wxSize GetPreferencesDialogSize();
		};
	}
}
