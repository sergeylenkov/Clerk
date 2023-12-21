#pragma once

#include <wx/wx.h>
#include "../AboutDialog.h"
#include "../PreferencesDialog.h"
#include "../Transactions/TransactionDialog.h"
#include "../AccountDialog.h"
#include "../Alerts/AlertDialog.h"
#include "../Budgets/BudgetDialog.h"
#include "../Goals/GoalDialog.h"
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
			void ShowNewAccountDialog(AccountType type);
			void ShowEditAccountDialog(int id);
			void ShowNewAlertDialog();
			void ShowEditAlertDialog(int id);
			void ShowNewBudgetDialog();
			void ShowEditBudgetDialog(int id);
			void ShowNewGoalDialog();
			void ShowEditGoalDialog(int id);

		private:
			wxFrame* _parent;
			DataContext& _context;
			Icons& _icons;

			wxSize GetAboutDialogSize();
			wxSize GetTransactionDialogSize();
			wxSize GetPreferencesDialogSize();
			wxSize GetAccountDialogSize();
			wxSize GetAlertDialogSize();
			wxSize GetBudgetDialogSize();
			wxSize GetGoalDialogSize();
		};
	}
}
