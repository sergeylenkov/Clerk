#include "AccountsContextMenu.h"

AccountsContextMenu::AccountsContextMenu(CommandsInvoker& commandsInvoker, TreeMenuItemType type) : TreeContextMenu(commandsInvoker) {
	_type = type;

	this->Append(static_cast<int>(ContextMenuType::NewTab), wxT("Open in New Tab"));
	this->AppendSeparator();

	this->Append(static_cast<int>(ContextMenuType::NewAccount), wxT("New Account..."));
	this->Append(static_cast<int>(ContextMenuType::NewTransaction), wxT("New Transaction..."));

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &AccountsContextMenu::OnMenuSelect, this);
}

void AccountsContextMenu::OnMenuSelect(wxCommandEvent& event) {
	int id = event.GetId();

	if (id == static_cast<int>(ContextMenuType::NewTab)) {
		_commandsInvoker.OnNewTab(TabType::Transactions);
	} else if (id == static_cast<int>(ContextMenuType::NewTransaction)) {
		_commandsInvoker.OnNewTransaction(-1);
	} else if (id == static_cast<int>(ContextMenuType::NewAccount)) {
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

		_commandsInvoker.OnNewAccount(accountType);
	}
}