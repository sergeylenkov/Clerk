#include "AccountsContextMenu.h"

AccountsContextMenu::AccountsContextMenu(CommandsInvoker& commandsInvoker, TreeMenuItemType type) : TreeContextMenu(commandsInvoker) {
	_type = type;

	wxMenuItem* item = this->Append(static_cast<int>(TreeContextMenuType::NewTab), wxT("Open in New Tab"));
	item->SetBitmap(wxBitmap(wxT("ICON_NEW_TAB"), wxBITMAP_TYPE_PNG_RESOURCE));

	this->AppendSeparator();

	this->Append(static_cast<int>(TreeContextMenuType::NewAccount), wxT("New Account..."));
	this->Append(static_cast<int>(TreeContextMenuType::NewTransaction), wxT("New Transaction..."));

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &AccountsContextMenu::OnMenuSelect, this);
}

void AccountsContextMenu::OnMenuSelect(wxCommandEvent& event) {
	TreeContextMenuType type = static_cast<TreeContextMenuType>(event.GetId());

	if (type == TreeContextMenuType::NewTab) {
		if (_type == TreeMenuItemType::Accounts) {
			_commandsInvoker.OnNewAccountsTab();
		}
		else {
			_commandsInvoker.OnNewAccountsTab(GetAccountTypeByMenuType(_type));
		}
	} else if (type == TreeContextMenuType::NewTransaction) {
		_commandsInvoker.OnNewTransaction(-1);
	} else if (type == TreeContextMenuType::NewAccount) {
		_commandsInvoker.OnNewAccount(GetAccountTypeByMenuType(_type));
	}
}

AccountType AccountsContextMenu::GetAccountTypeByMenuType(TreeMenuItemType type) {
	AccountType accountType = AccountType::Deposit;

	switch (_type)
	{
		case TreeMenuItemType::Deposits:
			accountType = AccountType::Deposit;
			break;
		case TreeMenuItemType::Receipts:
			accountType = AccountType::Receipt;
			break;
		case TreeMenuItemType::Expenses:
			accountType = AccountType::Expens;
			break;
		case TreeMenuItemType::Debts:
			accountType = AccountType::Debt;
			break;
		case TreeMenuItemType::Virtual:
			accountType = AccountType::Virtual;
			break;
		default:
			accountType = AccountType::Deposit;
			break;
	}

	return accountType;
}