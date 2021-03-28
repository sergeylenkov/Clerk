#include "ContextMenu.h"

using namespace Clerk::UI;

ContextMenu::ContextMenu(TreeMenuItemType type, TreeMenuItemType parentType, std::vector<std::shared_ptr<TransactionViewModel>> transactions) {
	this->Append(static_cast<int>(ContextMenuType::NewTab), wxT("Open in New Tab"));

	if (type == TreeMenuItemType::Account) {
		if (parentType == TreeMenuItemType::Archive) {
			this->AppendSeparator();
			this->Append(static_cast<int>(ContextMenuType::EditAccount), wxT("Edit Account..."));
			this->Append(static_cast<int>(ContextMenuType::RestoreAccount), wxT("Restore from Archive"));

			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuEditAccount, this, static_cast<int>(ContextMenuType::EditAccount));
			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuRestoreAccount, this, static_cast<int>(ContextMenuType::RestoreAccount));
		}
		else {
			this->AppendSeparator();
			this->Append(static_cast<int>(ContextMenuType::EditAccount), wxT("Edit Account..."));
			this->Append(static_cast<int>(ContextMenuType::DeleteAccount), wxT("Move to Archive"));
			this->AppendSeparator();			

			if (transactions.size() > 0) {
				wxMenu* menuTransaction = new wxMenu();
				this->AppendSubMenu(menuTransaction, wxT("New Transaction"));

				menuTransaction->Append(static_cast<int>(ContextMenuType::AddTransaction), wxT("New Transaction..."));
				menuTransaction->AppendSeparator();
				
				for (auto& transaction : transactions)
				{
					menuTransaction->Append(transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
					this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnSubMenuAddTransaction, this, transaction->id);
				}

				this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnSubMenuAddTransaction, this, static_cast<int>(ContextMenuType::AddTransaction));
			}
			else {
				this->Append(static_cast<int>(ContextMenuType::AddTransaction), wxT("New Transaction..."));
				this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuAddTransaction, this, static_cast<int>(ContextMenuType::AddTransaction));
			}

			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuEditAccount, this, static_cast<int>(ContextMenuType::EditAccount));
			this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuDeleteAccount, this, static_cast<int>(ContextMenuType::DeleteAccount));
		}
	}
	else if (type == TreeMenuItemType::Accounts || type == TreeMenuItemType::Receipts || type == TreeMenuItemType::Deposits
		|| type == TreeMenuItemType::Expenses || type == TreeMenuItemType::Credits || type == TreeMenuItemType::Debt) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddAccount), wxT("New Account..."));
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddTransaction), wxT("New Transaction..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuAddAccount, this, static_cast<int>(ContextMenuType::AddAccount));
		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuAddTransaction, this, static_cast<int>(ContextMenuType::AddTransaction));
	}
	else if (type == TreeMenuItemType::Budgets) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddBudget), wxT("New Budget..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuAddBudget, this, static_cast<int>(ContextMenuType::AddBudget));
	}
	else if (type == TreeMenuItemType::Schedulers) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddScheduler), wxT("New Scheduler..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuAddScheduler, this, static_cast<int>(ContextMenuType::AddScheduler));
	}
	else if (type == TreeMenuItemType::Goals) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddGoal), wxT("New Goal..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuAddGoal, this, static_cast<int>(ContextMenuType::AddGoal));
	}
	else if (type == TreeMenuItemType::Alerts) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::AddAlert), wxT("New Alert..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuAddAlert, this, static_cast<int>(ContextMenuType::AddAlert));
	}
	else if (type == TreeMenuItemType::Trash) {
		this->AppendSeparator();
		this->Append(static_cast<int>(ContextMenuType::EmptyTrash), wxT("Empty Trash..."));

		this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnMenuEmptyTrash, this, static_cast<int>(ContextMenuType::EmptyTrash));
	}

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &ContextMenu::OnOpenNewTab, this, static_cast<int>(ContextMenuType::NewTab));
}

void ContextMenu::OnMenuAddAccount(wxCommandEvent& event) {

}

void ContextMenu::OnMenuEditAccount(wxCommandEvent& event) {

}

void ContextMenu::OnMenuDeleteAccount(wxCommandEvent& event) {
}

void ContextMenu::OnMenuRestoreAccount(wxCommandEvent& event) {
}

void ContextMenu::OnMenuAddTransaction(wxCommandEvent& event) {
}

void ContextMenu::OnSubMenuAddTransaction(wxCommandEvent& event) {
}

void ContextMenu::OnMenuAddBudget(wxCommandEvent& event) {

}

void ContextMenu::OnMenuAddScheduler(wxCommandEvent& event) {

}

void ContextMenu::OnMenuAddGoal(wxCommandEvent& event) {

}

void ContextMenu::OnMenuAddAlert(wxCommandEvent& event) {

}

void ContextMenu::OnOpenNewTab(wxCommandEvent& event) {
}

void ContextMenu::OnMenuEmptyTrash(wxCommandEvent& event) {
}