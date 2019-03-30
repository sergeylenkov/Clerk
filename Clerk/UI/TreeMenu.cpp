#include "TreeMenu.h"

TreeMenu::TreeMenu(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
	CreateImageList();

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	treeMenu = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS | wxBORDER_NONE);
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

TreeMenu::~TreeMenu()
{	
	
}

void TreeMenu::CreateImageList() {
	wxImage image;

	imageList = new wxImageList(16, 16, true);

	for (int i = 0; i < DataHelper::GetInstance().accountsImageList->GetImageCount(); i++) {
		wxBitmap bitmap = DataHelper::GetInstance().accountsImageList->GetBitmap(i);
		imageList->Add(bitmap);		
	}

	menuImageIndex = DataHelper::GetInstance().accountsImageList->GetImageCount();

	for (int i = 0; i <= 11; i++) {
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
	itemData->type = TreeMenuItemTypes::MenuDashboard;

	wxTreeItemId homeItem = treeMenu->AppendItem(rootItem, "Dashboard", menuImageIndex, menuImageIndex, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuAccounts;

	wxTreeItemId accountsItem = treeMenu->AppendItem(rootItem, "Accounts", menuImageIndex + 1, menuImageIndex + 1, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuReports;

	wxTreeItemId reportsItem = treeMenu->AppendItem(rootItem, "Reports", menuImageIndex + 4, menuImageIndex + 4, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuBudgets;

	wxTreeItemId budgetsItem = treeMenu->AppendItem(rootItem, "Budgets", menuImageIndex + 6, menuImageIndex + 6, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuGoals;

	wxTreeItemId goalsItem = treeMenu->AppendItem(rootItem, "Goals", menuImageIndex + 8, menuImageIndex + 8, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuSchedulers;

	wxTreeItemId schedulersItem = treeMenu->AppendItem(rootItem, "Schedulers", menuImageIndex + 7, menuImageIndex + 7, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuTags;

	wxTreeItemId tagsItem = treeMenu->AppendItem(rootItem, "Tags", menuImageIndex + 9, menuImageIndex + 9, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuTrash;

	int count = DataHelper::GetInstance().GetDeletedTransactionsCount();
	int trashIcon = menuImageIndex + 10;

	if (count == 0) {
		trashIcon = menuImageIndex + 11;
	}

	wxTreeItemId trashItem = treeMenu->AppendItem(rootItem, "Trash", trashIcon, trashIcon, itemData);
	
	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuReceipts;

	wxTreeItemId child = treeMenu->AppendItem(accountsItem, "Receipts", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Receipt))
	{
		int iconId = 27;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuDeposits;

	child = treeMenu->AppendItem(accountsItem, "Deposits", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Deposit))
	{
		int iconId = 26;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuVirtual;

	child = treeMenu->AppendItem(accountsItem, "Virtual", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Virtual))
	{
		int iconId = 26;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuExpenses;

	child = treeMenu->AppendItem(accountsItem, "Expenes", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Expens))
	{
		int iconId = 28;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuDebt;

	child = treeMenu->AppendItem(accountsItem, "Debt", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Debt))
	{
		int iconId = 28;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuCredits;

	child = treeMenu->AppendItem(accountsItem, "Credits", menuImageIndex + 2, menuImageIndex + 2, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Credit))
	{
		int iconId = 28;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuArchive;

	wxTreeItemId archivehItem = treeMenu->AppendItem(accountsItem, "Archive", menuImageIndex + 3, menuImageIndex + 3, itemData);

	for each (auto account in DataHelper::GetInstance().GetArchiveAccounts())
	{
		int iconId = 28;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(archivehItem, *account->name, iconId, iconId, itemData);

		accounts.push_back(account);
	}

	for each (auto report in DataHelper::GetInstance().GetReports())
	{
		itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuReport;

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
		if (item->type == TreeMenuItemTypes::MenuAccount) {
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

		if (item->type == TreeMenuItemTypes::MenuAccount) {
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

	menu->Append(static_cast<int>(TreeMenuTypes::NewTab), wxT("Open in New Tab"));
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnOpenNewTab, this, static_cast<int>(TreeMenuTypes::NewTab));

	if (item->type == TreeMenuItemTypes::MenuAccount) {
		wxTreeItemId parent = treeMenu->GetItemParent(contextMenuItem);
		TreeMenuItemData *parentItem = (TreeMenuItemData *)treeMenu->GetItemData(parent);
		
		if (parentItem->type == TreeMenuItemTypes::MenuArchive) {
			menu->AppendSeparator();
			menu->Append(static_cast<int>(TreeMenuTypes::EditAccount), wxT("Edit Account..."));
			menu->Append(static_cast<int>(TreeMenuTypes::RestoreAccount), wxT("Restore from Archive"));

			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEditAccount, this, static_cast<int>(TreeMenuTypes::EditAccount));
			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuRestoreAccount, this, static_cast<int>(TreeMenuTypes::RestoreAccount));
		}
		else {
			menu->AppendSeparator();
			menu->Append(static_cast<int>(TreeMenuTypes::EditAccount), wxT("Edit Account..."));
			menu->Append(static_cast<int>(TreeMenuTypes::DeleteAccount), wxT("Move to Archive"));
			menu->AppendSeparator();
			menu->Append(static_cast<int>(TreeMenuTypes::AddTransaction), wxT("Add Transaction..."));

			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEditAccount, this, static_cast<int>(TreeMenuTypes::EditAccount));
			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuDeleteAccount, this, static_cast<int>(TreeMenuTypes::DeleteAccount));
			menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddTransaction, this, static_cast<int>(TreeMenuTypes::AddTransaction));
		}
	}
	else if (item->type == TreeMenuItemTypes::MenuAccounts || item->type == TreeMenuItemTypes::MenuReceipts || item->type == TreeMenuItemTypes::MenuDeposits 
		  || item->type == TreeMenuItemTypes::MenuExpenses || item->type == TreeMenuItemTypes::MenuCredits || item->type == TreeMenuItemTypes::MenuDebt) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(TreeMenuTypes::AddAccount), wxT("Add Account..."));
		menu->AppendSeparator();
		menu->Append(static_cast<int>(TreeMenuTypes::AddTransaction), wxT("Add Transaction..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddAccount, this, static_cast<int>(TreeMenuTypes::AddAccount));
		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddTransaction, this, static_cast<int>(TreeMenuTypes::AddTransaction));
	}
	else if (item->type == TreeMenuItemTypes::MenuBudgets) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(TreeMenuTypes::AddBudget), wxT("Add Budget..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddBudget, this, static_cast<int>(TreeMenuTypes::AddBudget));
	}
	else if (item->type == TreeMenuItemTypes::MenuSchedulers) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(TreeMenuTypes::AddScheduler), wxT("Add Scheduler..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddScheduler, this, static_cast<int>(TreeMenuTypes::AddScheduler));
	}
	else if (item->type == TreeMenuItemTypes::MenuGoals) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(TreeMenuTypes::AddGoal), wxT("Add Goal..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddGoal, this, static_cast<int>(TreeMenuTypes::AddGoal));
	}
	else if (item->type == TreeMenuItemTypes::MenuTrash) {
		menu->AppendSeparator();
		menu->Append(static_cast<int>(TreeMenuTypes::EmptyTrash), wxT("Empty Trash..."));

		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEmptyTrash, this, static_cast<int>(TreeMenuTypes::EmptyTrash));
	}

	PopupMenu(menu, event.GetPoint());

	delete menu;
}

void TreeMenu::OnTreeItemSelect(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);
	wxTreeItemId oldItem = event.GetOldItem();
	
	if (item != NULL && oldItem != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			auto account = std::static_pointer_cast<Account>(item->object);

			if (OnAccountSelect) {
				OnAccountSelect(account);
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuDashboard) {
			if (OnDashboardSelect) {
				OnDashboardSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuBudgets) {
			if (OnBudgetsSelect) {
				OnBudgetsSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuGoals) {
			if (OnGoalsSelect) {
				OnGoalsSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuSchedulers) {
			if (OnSchedulersSelect) {
				OnSchedulersSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuTrash) {
			if (OnTrashSelect) {
				OnTrashSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuTags) {
			if (OnTagsSelect) {
				OnTagsSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuReport) {
			auto report = std::static_pointer_cast<Report>(item->object);

			if (OnReportSelect) {
				OnReportSelect(report);
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuAccounts || item->type == TreeMenuItemTypes::MenuDeposits || item->type == TreeMenuItemTypes::MenuExpenses || item->type == TreeMenuItemTypes::MenuReceipts || item->type == TreeMenuItemTypes::MenuVirtual) {
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
			if (item->type == TreeMenuItemTypes::MenuAccount) {
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

	if (item->type == TreeMenuItemTypes::MenuAccount) {
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