#include "DefaultContextMenu.h"

DefaultContextMenu::DefaultContextMenu(CommandsInvoker& commandsInvoker, TreeMenuItemType type) : TreeContextMenu(commandsInvoker) {
	_type = type;
		
	wxMenuItem* item = this->Append(static_cast<int>(TreeContextMenuType::NewTab), _("Open in New Tab"));

	item->SetBitmap(wxBitmap("ICON_NEW_TAB"), wxBITMAP_TYPE_PNG_RESOURCE);

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &DefaultContextMenu::OnMenuSelect, this);
}

void DefaultContextMenu::OnMenuSelect(wxCommandEvent& event) {
	TabType tabType = TabType::Dashboard;

	switch (_type)
	{
		case Clerk::UI::TreeMenuItemType::Dashboard:
			tabType = TabType::Dashboard;
			break;
		case Clerk::UI::TreeMenuItemType::Accounts:
			tabType = TabType::Transactions;
			break;
		case Clerk::UI::TreeMenuItemType::Account:
			tabType = TabType::Transactions;
			break;
		case Clerk::UI::TreeMenuItemType::Expenses:
			tabType = TabType::Transactions;
			break;
		case Clerk::UI::TreeMenuItemType::Receipts:
			tabType = TabType::Transactions;
			break;
		case Clerk::UI::TreeMenuItemType::Deposits:
			tabType = TabType::Transactions;
			break;
		case Clerk::UI::TreeMenuItemType::Debts:
			tabType = TabType::Transactions;
			break;
		case Clerk::UI::TreeMenuItemType::Virtual:
			tabType = TabType::Transactions;
			break;
		case Clerk::UI::TreeMenuItemType::Reports:
			tabType = TabType::Reports;
			break;
		case Clerk::UI::TreeMenuItemType::Report:
			tabType = TabType::Reports;
			break;
		case Clerk::UI::TreeMenuItemType::Budgets:
			tabType = TabType::Budgets;
			break;
		case Clerk::UI::TreeMenuItemType::Schedulers:
			tabType = TabType::Schedulers;
			break;
		case Clerk::UI::TreeMenuItemType::Goals:
			tabType = TabType::Goals;
			break;
		case Clerk::UI::TreeMenuItemType::Tags:
			tabType = TabType::Tags;
			break;
		case Clerk::UI::TreeMenuItemType::Alerts:
			tabType = TabType::Alerts;
			break;
		case Clerk::UI::TreeMenuItemType::Trash:
			tabType = TabType::Trash;
			break;
		default:
			break;
	}

	_commandsInvoker.OnNewTab(tabType);
}