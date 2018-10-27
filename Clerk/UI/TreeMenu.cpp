#include "TreeMenu.h"

TreeMenu::TreeMenu(wxWindow *parent, wxWindowID id) : wxPanel(parent, id)
{
	CreateImageList();

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	treeMenu = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS | wxBORDER_NONE);
	treeMenu->SetBackgroundColour(wxColour(245, 245, 245, 1));	
	treeMenu->SetForegroundColour(wxColour(68, 68, 68, 1));	
	treeMenu->AssignImageList(accountsImageList);
	treeMenu->AddRoot("Accounts", -1, -1, 0);

	mainSizer->Add(treeMenu, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();	

	treeMenu->Bind(wxEVT_TREE_SEL_CHANGED, &TreeMenu::OnTreeItemSelect, this);
	treeMenu->Bind(wxEVT_TREE_ITEM_MENU, &TreeMenu::OnTreeSpecItemMenu, this);
	treeMenu->Bind(wxEVT_TREE_ITEM_EXPANDED, &TreeMenu::OnTreeItemExpanded, this);
	treeMenu->Bind(wxEVT_TREE_ITEM_COLLAPSED, &TreeMenu::OnTreeItemCollapsed, this);
}

TreeMenu::~TreeMenu()
{	
}

void TreeMenu::CreateImageList() {
	wxImage image;

	accountsImageList = new wxImageList(16, 16, true);

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);

		if (image.LoadFile(path, wxBITMAP_TYPE_PNG))
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}

	for (int i = 0; i <= 52; i++) {
		wxString path = wxString::Format("Resources\\Menu Icons\\%d.png", i);

		if (image.LoadFile(path, wxBITMAP_TYPE_PNG))
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

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

	wxTreeItemId homeItem = treeMenu->AppendItem(rootItem, "Dashboard", 87, 87, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuAccounts;

	wxTreeItemId accountsItem = treeMenu->AppendItem(rootItem, "Accounts", 84, 84, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuReports;

	wxTreeItemId reportsItem = treeMenu->AppendItem(rootItem, "Reports", 86, 86, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuBudgets;

	wxTreeItemId budgetsItem = treeMenu->AppendItem(rootItem, "Budgets", 102, 102, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuSchedulers;

	wxTreeItemId schedulersItem = treeMenu->AppendItem(rootItem, "Schedulers", 103, 103, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuArchive;

	wxTreeItemId archivehItem = treeMenu->AppendItem(rootItem, "Archive", 96, 96, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuTrash;

	int count = DataHelper::GetInstance().GetDeletedTransactionsCount();
	int trashIcon = 94;

	if (count == 0) {
		trashIcon = 95;
	}

	wxTreeItemId trashItem = treeMenu->AppendItem(rootItem, "Trash", trashIcon, trashIcon, itemData);
	
	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuReceipts;

	wxTreeItemId child = treeMenu->AppendItem(accountsItem, "Receipts", 101, 101, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Receipt))
	{
		int icon = 27;

		if (account->iconId <= accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuDeposits;

	child = treeMenu->AppendItem(accountsItem, "Deposits", 101, 101, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Deposit))
	{
		int icon = 26;

		if (account->iconId <= accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);

		accounts.push_back(account);
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuExpenses;

	child = treeMenu->AppendItem(accountsItem, "Expenes", 101, 101, itemData);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Expens))
	{
		int icon = 28;

		if (account->iconId <= accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);

		accounts.push_back(account);
	}

	child = treeMenu->AppendItem(accountsItem, "Debt", 101, 101);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Debt))
	{
		int icon = 28;

		if (account->iconId <= accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);

		accounts.push_back(account);
	}

	child = treeMenu->AppendItem(accountsItem, "Credits", 101, 101);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Credit))
	{
		int icon = 28;

		if (account->iconId <= accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);

		accounts.push_back(account);
	}

	for each (auto report in DataHelper::GetInstance().GetReports())
	{
		itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuReport;

		itemData->object = report;

		wxTreeItemId itemId = treeMenu->AppendItem(reportsItem, *report->name, 90, 90, itemData);

		reports.push_back(report);
	}

	for each (auto account in DataHelper::GetInstance().GetArchiveAccounts())
	{
		int icon = 28;

		if (account->iconId <= accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(archivehItem, *account->name, icon, icon, itemData);

		accounts.push_back(account);
	}
}

void TreeMenu::RestoreState() {
	wxTreeItemId rootItem = treeMenu->GetRootItem();
	ExpandItem(rootItem);	
}

void TreeMenu::ExpandItem(wxTreeItemId item) {
	wxTreeItemIdValue cookie;
	wxTreeItemId child = treeMenu->GetFirstChild(item, cookie);

	while (child.IsOk())
	{
		TreeMenuItemData *data = (TreeMenuItemData *)treeMenu->GetItemData(child);

		if (data != NULL && Settings::GetInstance().IsMenuExpanded(data->type)) {
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
	
	wxMenu *menu = new wxMenu();

	menu->Append(static_cast<int>(TreeMenuTypes::NewTab), wxT("Open in New Tab"));
	menu->AppendSeparator();
	menu->Append(static_cast<int>(TreeMenuTypes::AddAccount), wxT("Add Account..."));
	menu->Append(static_cast<int>(TreeMenuTypes::EditAccount), wxT("Edit Account..."));
	menu->Append(static_cast<int>(TreeMenuTypes::DeleteAccount), wxT("Move Account to Archive"));
	menu->AppendSeparator();
	menu->Append(static_cast<int>(TreeMenuTypes::AddTransaction), wxT("Add Transaction..."));

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnOpenNewTab, this, static_cast<int>(TreeMenuTypes::NewTab));
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddAccount, this, static_cast<int>(TreeMenuTypes::AddAccount));
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEditAccount, this, static_cast<int>(TreeMenuTypes::EditAccount));
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuDeleteAccount, this, static_cast<int>(TreeMenuTypes::DeleteAccount));
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddTransaction, this, static_cast<int>(TreeMenuTypes::AddTransaction));

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
		else if (item->type == TreeMenuItemTypes::MenuReport) {
			auto report = std::static_pointer_cast<Report>(item->object);

			if (OnReportSelect) {
				OnReportSelect(report);
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuDeposits || item->type == TreeMenuItemTypes::MenuExpenses || item->type == TreeMenuItemTypes::MenuReceipts) {
			if (OnAccountsSelect) {
				OnAccountsSelect(item->type);
			}
		}
	}
}

void TreeMenu::OnMenuAddAccount(wxCommandEvent &event) {
	if (OnAddAccount) {
		OnAddAccount();
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

void TreeMenu::OnOpenNewTab(wxCommandEvent &event) {
	if (OnNewTab) {
		TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(contextMenuItem);

		if (item != NULL) {
			OnNewTab(item->type, item->object);			
		}		
	}
}

void TreeMenu::OnTreeItemExpanded(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		Settings::GetInstance().AddExpandedMenu(item->type);
	}
}

void TreeMenu::OnTreeItemCollapsed(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		Settings::GetInstance().RemoveExpandedMenu(item->type);
	}
}