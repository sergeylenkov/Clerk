#include "TreeMenu.h"

TreeMenu::TreeMenu(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
	CreateImageList();

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	treeMenu = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS | wxTR_NO_LINES | wxTR_FULL_ROW_HIGHLIGHT |  wxBORDER_NONE);
	treeMenu->SetBackgroundColour(wxColour(245, 245, 245, 1));	
	treeMenu->SetForegroundColour(wxColour(68, 68, 68, 1));	
	treeMenu->AssignImageList(imageList);
	treeMenu->AddRoot("", -1, -1, 0);

	mainSizer->Add(treeMenu, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();	

	treeMenu->Bind(wxEVT_TREE_SEL_CHANGED, &TreeMenu::OnTreeItemSelect, this);
	treeMenu->Bind(wxEVT_TREE_ITEM_MENU, &TreeMenu::OnTreeSpecItemMenu, this);
	treeMenu->Bind(wxEVT_TREE_ITEM_EXPANDED, &TreeMenu::OnTreeItemExpanded, this);
	treeMenu->Bind(wxEVT_TREE_ITEM_COLLAPSED, &TreeMenu::OnTreeItemCollapsed, this);
	treeMenu->Bind(wxEVT_TREE_BEGIN_DRAG, &TreeMenu::OnBeginDrag, this);
	treeMenu->Bind(wxEVT_TREE_END_DRAG, &TreeMenu::OnEndDrag, this);
}

void TreeMenu::CreateImageList() {
	wxImage image;

	imageList = new wxImageList(16, 16, true);

	for (int i = 0; i < DataHelper::GetInstance().accountsImageList->GetImageCount(); i++) {
		wxBitmap bitmap = DataHelper::GetInstance().accountsImageList->GetBitmap(i);
		imageList->Add(bitmap);		
	}

	menuImageIndex = DataHelper::GetInstance().accountsImageList->GetImageCount();

	for (int i = 0; i <= 12; i++) {
		wxString path = wxString::Format("Resources\\Menu Icons\\%d.png", i);

		if (image.LoadFile(path, wxBITMAP_TYPE_PNG))
		{
			wxBitmap *bitmap = new wxBitmap(image);
			imageList->Add(*bitmap);

			delete bitmap;
		}
	}
}

void TreeMenu::Update() {
	wxTreeItemId rootItem = treeMenu->GetRootItem();

	accounts.clear();
	reports.clear();

	treeMenu->DeleteChildren(rootItem);

	TreeMenuItemData *itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Dashboard;

	wxTreeItemId homeItem = treeMenu->AppendItem(rootItem, "Dashboard", menuImageIndex, menuImageIndex, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Accounts;

	wxTreeItemId accountsItem = treeMenu->AppendItem(rootItem, "Accounts", menuImageIndex + 1, menuImageIndex + 1, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Reports;

	wxTreeItemId reportsItem = treeMenu->AppendItem(rootItem, "Reports", menuImageIndex + 4, menuImageIndex + 4, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Budgets;

	wxTreeItemId budgetsItem = treeMenu->AppendItem(rootItem, "Budgets", menuImageIndex + 6, menuImageIndex + 6, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Goals;

	wxTreeItemId goalsItem = treeMenu->AppendItem(rootItem, "Goals", menuImageIndex + 8, menuImageIndex + 8, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Schedulers;

	wxTreeItemId schedulersItem = treeMenu->AppendItem(rootItem, "Schedulers", menuImageIndex + 7, menuImageIndex + 7, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Alerts;

	wxTreeItemId alertsItem = treeMenu->AppendItem(rootItem, "Alerts", menuImageIndex + 12, menuImageIndex + 12, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Tags;

	wxTreeItemId tagsItem = treeMenu->AppendItem(rootItem, "Tags", menuImageIndex + 9, menuImageIndex + 9, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Trash;

	int count = DataHelper::GetInstance().GetDeletedTransactionsCount();
	int trashIcon = menuImageIndex + 10;

	if (count == 0) {
		trashIcon = menuImageIndex + 11;
	}

	trashItem = treeMenu->AppendItem(rootItem, "Trash", trashIcon, trashIcon, itemData);
	
	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Receipts;

	wxTreeItemId child = treeMenu->AppendItem(accountsItem, "Receipts", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Receipt))
	{
		int iconId = 27;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::Account;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Deposits;

	child = treeMenu->AppendItem(accountsItem, "Deposits", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Deposit))
	{
		int iconId = 26;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::Account;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Virtual;

	child = treeMenu->AppendItem(accountsItem, "Virtual", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Virtual))
	{
		int iconId = 26;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::Account;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Expenses;

	child = treeMenu->AppendItem(accountsItem, "Expenes", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Expens))
	{
		int iconId = 28;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::Account;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Debt;

	child = treeMenu->AppendItem(accountsItem, "Debts", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Debt))
	{
		int iconId = 28;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::Account;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::Archive;

	wxTreeItemId archivehItem = treeMenu->AppendItem(accountsItem, "Archive", menuImageIndex + 3, menuImageIndex + 3, itemData);

	for (auto account : DataHelper::GetInstance().GetArchiveAccounts())
	{
		int iconId = 28;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::Account;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(archivehItem, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	for (auto report : DataHelper::GetInstance().GetReports())
	{
		itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::Report;

		itemData->object = report;

		wxTreeItemId itemId = treeMenu->AppendItem(reportsItem, *report->name, menuImageIndex + 5, menuImageIndex + 5, itemData);

		reports.push_back(report);
	}
}

void TreeMenu::RestoreState() {
	wxTreeItemId rootItem = treeMenu->GetRootItem();
	ExpandItem(rootItem);	
}

void TreeMenu::ExpandItem(wxTreeItemId &item) {
	wxTreeItemIdValue cookie;
	wxTreeItemId child = treeMenu->GetFirstChild(item, cookie);

	while (child.IsOk())
	{
		TreeMenuItemData *data = (TreeMenuItemData *)treeMenu->GetItemData(child);

		if (data != NULL && Settings::GetInstance().IsMenuExpanded(static_cast<int>(data->type))) {
			treeMenu->Expand(child);
		}

		if (treeMenu->HasChildren(child)) {
			ExpandItem(child);
		}

		child = treeMenu->GetNextChild(item, cookie);
	}
}

std::shared_ptr<Account> TreeMenu::GetAccount() {
	wxTreeItemId itemId = treeMenu->GetSelection();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::Account) {
			shared_ptr<Account> account = static_pointer_cast<Account>(item->object);
			return account;
		}
	}

	return nullptr;
}

vector<std::shared_ptr<Account>> TreeMenu::GetAccounts() {
	return accounts;
}

std::shared_ptr<Account> TreeMenu::GetContextMenuAccount() {
	if (contextMenuItem != NULL) {
		TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(contextMenuItem);

		if (item->type == TreeMenuItemTypes::Account) {
			shared_ptr<Account> account = static_pointer_cast<Account>(item->object);
			return account;
		}
	}

	return nullptr;
}

void TreeMenu::OnTreeSpecItemMenu(wxTreeEvent &event) {
	contextMenuItem = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(contextMenuItem);

	wxMenu *menu = new wxMenu();

	menu->Append(static_cast<int>(ContextMenuTypes::NewTab), wxT("Open in New Tab"));
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnOpenNewTab, this, static_cast<int>(ContextMenuTypes::NewTab));

	if (item->type == TreeMenuItemTypes::Account) {
		wxTreeItemId parent = treeMenu->GetItemParent(contextMenuItem);
		TreeMenuItemData *parentItem = (TreeMenuItemData *)treeMenu->GetItemData(parent);
		
		if (parentItem->type == TreeMenuItemTypes::Archive) {
			menu->AppendSeparator();
			menu->Append(static_cast<int>(ContextMenuTypes::EditAccount), wxT("Edit Account..."));
			menu->Append(static_cast<int>(ContextMenuTypes::RestoreAccount), wxT("Restore from Archive"));

			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEditAccount, this, static_cast<int>(ContextMenuTypes::EditAccount));
			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuRestoreAccount, this, static_cast<int>(ContextMenuTypes::RestoreAccount));
		}
		else {
			menu->AppendSeparator();
			menu->Append(static_cast<int>(ContextMenuTypes::EditAccount), wxT("Edit Account..."));
			menu->Append(static_cast<int>(ContextMenuTypes::DeleteAccount), wxT("Move to Archive"));
			menu->AppendSeparator();
			menu->Append(static_cast<int>(ContextMenuTypes::AddTransaction), wxT("Add Transaction..."));

			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEditAccount, this, static_cast<int>(ContextMenuTypes::EditAccount));
			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuDeleteAccount, this, static_cast<int>(ContextMenuTypes::DeleteAccount));
			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddTransaction, this, static_cast<int>(ContextMenuTypes::AddTransaction));
		}
	}
	else if (item->type == TreeMenuItemTypes::Accounts || item->type == TreeMenuItemTypes::Receipts || item->type == TreeMenuItemTypes::Deposits 
		  || item->type == TreeMenuItemTypes::Expenses || item->type == TreeMenuItemTypes::Credits || item->type == TreeMenuItemTypes::Debt) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(ContextMenuTypes::AddAccount), wxT("Add Account..."));
		menu->AppendSeparator();
		menu->Append(static_cast<int>(ContextMenuTypes::AddTransaction), wxT("Add Transaction..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddAccount, this, static_cast<int>(ContextMenuTypes::AddAccount));
		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddTransaction, this, static_cast<int>(ContextMenuTypes::AddTransaction));
	}
	else if (item->type == TreeMenuItemTypes::Budgets) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(ContextMenuTypes::AddBudget), wxT("Add Budget..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddBudget, this, static_cast<int>(ContextMenuTypes::AddBudget));
	}
	else if (item->type == TreeMenuItemTypes::Schedulers) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(ContextMenuTypes::AddScheduler), wxT("Add Scheduler..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddScheduler, this, static_cast<int>(ContextMenuTypes::AddScheduler));
	}
	else if (item->type == TreeMenuItemTypes::Goals) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(ContextMenuTypes::AddGoal), wxT("Add Goal..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddGoal, this, static_cast<int>(ContextMenuTypes::AddGoal));
	}
	else if (item->type == TreeMenuItemTypes::Alerts) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(ContextMenuTypes::AddAlert), wxT("Add Alert..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddAlert, this, static_cast<int>(ContextMenuTypes::AddAlert));
	}
	else if (item->type == TreeMenuItemTypes::Trash) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(ContextMenuTypes::EmptyTrash), wxT("Empty Trash..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEmptyTrash, this, static_cast<int>(ContextMenuTypes::EmptyTrash));
	}

	PopupMenu(menu, event.GetPoint());

	delete menu;
}

void TreeMenu::OnTreeItemSelect(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);
	wxTreeItemId oldItem = event.GetOldItem();
	
	if (item != NULL && oldItem != NULL) {
		if (item->type == TreeMenuItemTypes::Account) {
			auto account = std::static_pointer_cast<Account>(item->object);

			if (OnAccountSelect) {
				OnAccountSelect(account);
			}
		}
		else if (item->type == TreeMenuItemTypes::Dashboard) {
			if (OnDashboardSelect) {
				OnDashboardSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::Budgets) {
			if (OnBudgetsSelect) {
				OnBudgetsSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::Goals) {
			if (OnGoalsSelect) {
				OnGoalsSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::Schedulers) {
			if (OnSchedulersSelect) {
				OnSchedulersSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::Trash) {
			if (OnTrashSelect) {
				OnTrashSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::Tags) {
			if (OnTagsSelect) {
				OnTagsSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::Alerts) {
			if (OnAlertsSelect) {
				OnAlertsSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::Report) {
			auto report = std::static_pointer_cast<Report>(item->object);

			if (OnReportSelect) {
				OnReportSelect(report);
			}
		}
		else if (item->type == TreeMenuItemTypes::Accounts || item->type == TreeMenuItemTypes::Deposits || item->type == TreeMenuItemTypes::Expenses || item->type == TreeMenuItemTypes::Receipts || item->type == TreeMenuItemTypes::Virtual) {
			if (OnAccountsSelect) {
				OnAccountsSelect(item->type);
			}
		}
	}
}

void TreeMenu::OnMenuAddAccount(wxCommandEvent &event) {
	if (OnAddAccount) {
		TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(contextMenuItem);

		if (item != NULL) {
			OnAddAccount(item->type);
		}
	}
}

void TreeMenu::OnMenuEditAccount(wxCommandEvent &event) {
	auto account = GetContextMenuAccount();

	if (OnEditAccount) {
		OnEditAccount(account);
	}
}

void TreeMenu::OnMenuDeleteAccount(wxCommandEvent &event) {
	auto account = GetContextMenuAccount();

	if (OnArchiveAccount) {
		OnArchiveAccount(account);
	}
}

void TreeMenu::OnMenuRestoreAccount(wxCommandEvent &event) {
	auto account = GetContextMenuAccount();

	if (OnRestoreAccount) {
		OnRestoreAccount(account);
	}
}

void TreeMenu::OnMenuAddTransaction(wxCommandEvent &event) {
	if (OnAddTransaction) {
		TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(contextMenuItem);

		if (item != NULL) {
			if (item->type == TreeMenuItemTypes::Account) {
				auto account = std::static_pointer_cast<Account>(item->object);
				OnAddTransaction(account);
			}
		}
		else {
			OnAddTransaction(nullptr);
		}
	}
}

void TreeMenu::OnMenuAddBudget(wxCommandEvent &event) {
	if (OnAddBudget) {
		OnAddBudget();
	}
}

void TreeMenu::OnMenuAddScheduler(wxCommandEvent &event) {
	if (OnAddScheduler) {
		OnAddScheduler();
	}
}

void TreeMenu::OnMenuAddGoal(wxCommandEvent &event) {
	if (OnAddGoal) {
		OnAddGoal();
	}
}

void TreeMenu::OnMenuAddAlert(wxCommandEvent &event) {
	if (OnAddAlert) {
		OnAddAlert();
	}
}

void TreeMenu::OnOpenNewTab(wxCommandEvent &event) {
	if (OnNewTab) {
		TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(contextMenuItem);

		if (item != NULL) {
			OnNewTab(item->type, item->object);			
		}		
	}
}

void TreeMenu::OnMenuEmptyTrash(wxCommandEvent &event) {
	if (OnEmptyTrash) {
		OnEmptyTrash();
	}
}

void TreeMenu::OnTreeItemExpanded(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		Settings::GetInstance().AddExpandedMenu(static_cast<int>(item->type));
	}
}

void TreeMenu::OnTreeItemCollapsed(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		Settings::GetInstance().RemoveExpandedMenu(static_cast<int>(item->type));
	}
}

void TreeMenu::OnBeginDrag(wxTreeEvent &event) {
	draggedMenuItem = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(draggedMenuItem);

	if (item->type == TreeMenuItemTypes::Account) {
		event.Allow();
	}
}

void TreeMenu::OnEndDrag(wxTreeEvent &event) {	
	wxTreeItemId itemId = event.GetItem();

	if (itemId.IsOk()) {
		wxTreeItemId parent = treeMenu->GetItemParent(itemId);

		TreeMenuItemData *draggedData = (TreeMenuItemData *)treeMenu->GetItemData(draggedMenuItem);
		TreeMenuItemData *itemData = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

		if (draggedData->type == itemData->type) {
			auto account = std::static_pointer_cast<Account>(draggedData->object);

			TreeMenuItemData *itemData = new TreeMenuItemData();
			itemData->type = draggedData->type;
			itemData->object = account;

			treeMenu->InsertItem(parent, itemId, *account->name, account->iconId, account->iconId, itemData);

			treeMenu->Delete(draggedMenuItem);

			ReorderAccounts(parent);
		}
	}
}

void TreeMenu::ReorderAccounts(wxTreeItemId &item) {
	wxTreeItemIdValue cookie;
	wxTreeItemId child = treeMenu->GetFirstChild(item, cookie);
	int orderId = 0;

	while (child.IsOk())
	{
		TreeMenuItemData *data = (TreeMenuItemData *)treeMenu->GetItemData(child);
		auto account = std::static_pointer_cast<Account>(data->object);

		account->orderId = orderId++;
		account->Save();

		child = treeMenu->GetNextChild(item, cookie);
	}
}

void TreeMenu::UpdateTrashItem() {
	int count = DataHelper::GetInstance().GetDeletedTransactionsCount();
	int icon = menuImageIndex + 10;

	if (count == 0) {
		icon = menuImageIndex + 11;
	}

	treeMenu->SetItemImage(trashItem, icon, wxTreeItemIcon_Normal);
	treeMenu->SetItemImage(trashItem, icon, wxTreeItemIcon_Selected);
}