#include "AccountsContextMenu.h"

AccountsContextMenu::AccountsContextMenu(CommandsInvoker& commandsInvoker, Icons& icons, TreeMenuItemType type):
	TreeContextMenu(commandsInvoker, icons),
	_type(type)
{
	wxMenuItem* item = Append(static_cast<int>(TreeContextMenuType::NewTab), _("Open in New Tab"));
	item->SetBitmap(_icons.GetIconByType(IconType::Tab));

	AppendSeparator();

	Append(static_cast<int>(TreeContextMenuType::NewAccount), _("New Account..."));
	Append(static_cast<int>(TreeContextMenuType::NewTransaction), _("New Transaction..."));

	Bind(wxEVT_COMMAND_MENU_SELECTED, &AccountsContextMenu::OnMenuSelect, this);
}

void AccountsContextMenu::OnMenuSelect(wxCommandEvent& event) {
	TreeContextMenuType type = static_cast<TreeContextMenuType>(event.GetId());

	if (type == TreeContextMenuType::NewTab) {
		if (_type == TreeMenuItemType::Accounts) {
			_commandsInvoker.OpenAccountsTab();
		}
		else {
			_commandsInvoker.OpenAccountsTab(GetAccountTypeByMenuType(_type));
		}
	} else if (type == TreeContextMenuType::NewTransaction) {
		_commandsInvoker.NewTransaction(-1);
	} else if (type == TreeContextMenuType::NewAccount) {
		_commandsInvoker.NewAccount(GetAccountTypeByMenuType(_type));
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