#pragma once

#include <wx/wx.h>
#include "TreeMenu.h"
#include "Enums.h"
#include "../../Data/ViewModels/TransactionViewModel.h"
#include "../../Commands/CommandsInvoker.h"

using namespace Clerk::Data;
using namespace Clerk::Commands;

namespace Clerk {
	namespace UI {
		class TreeContextMenu : public wxMenu {
		public:
			TreeContextMenu(TreeMenuItemType type, TreeMenuItemType parentType, std::vector<std::shared_ptr<TransactionViewModel>> transactions, CommandsInvoker& commandsInvoker);
			
		private:
			CommandsInvoker& _commandsInvoker;

			void OnMenuAddAccount(wxCommandEvent& event);
			void OnMenuEditAccount(wxCommandEvent& event);
			void OnMenuDeleteAccount(wxCommandEvent& event);
			void OnMenuRestoreAccount(wxCommandEvent& event);
			void OnMenuAddTransaction(wxCommandEvent& event);
			void OnSubMenuAddTransaction(wxCommandEvent& event);
			void OnMenuAddBudget(wxCommandEvent& event);
			void OnMenuAddScheduler(wxCommandEvent& event);
			void OnMenuAddGoal(wxCommandEvent& event);
			void OnMenuAddAlert(wxCommandEvent& event);
			void OnOpenNewTab(wxCommandEvent& event);
			void OnMenuEmptyTrash(wxCommandEvent& event);
		};
	}
}