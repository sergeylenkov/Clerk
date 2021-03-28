#pragma once

#include <wx/wx.h>
#include "Menu.h"
#include "Enums.h"
#include "../../Data/ViewModels/TransactionViewModel.h"

using namespace Clerk::Data;

namespace Clerk {
	namespace UI {
		class ContextMenu : public wxMenu {
		public:
			ContextMenu(TreeMenuItemType type, TreeMenuItemType parentType, std::vector<std::shared_ptr<TransactionViewModel>> transactions);

		private:
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