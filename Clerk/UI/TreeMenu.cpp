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
}

TreeMenu::~TreeMenu()
{
	delete treeMenu;
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

	for (int i = 0; i <= 19; i++) {
		wxString path = wxString::Format("Resources\\%d.png", i);
		if (image.LoadFile(path, wxBITMAP_TYPE_PNG))
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}
}

void TreeMenu::Update() {
	selectedAccountId = -1; //Settings::GetInstance().GetSelectedAccountId();

	wxTreeItemId rootItem = treeMenu->GetRootItem();

	accounts.clear();
	reports.clear();

	treeMenu->DeleteChildren(rootItem);

	TreeMenuItemData *itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuHome;

	wxTreeItemId homeItem = treeMenu->AppendItem(rootItem, "Home", 15, 15, itemData);
	wxTreeItemId accountsItem = treeMenu->AppendItem(homeItem, "Accounts", 33, 33);
	wxTreeItemId reportsItem = treeMenu->AppendItem(homeItem, "Reports", 51, 51);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuBudget;

	wxTreeItemId budgetsItem = treeMenu->AppendItem(homeItem, "Budgets", 57, 57, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuDeposits;

	wxTreeItemId child = treeMenu->AppendItem(accountsItem, "Deposits", 32, 32, itemData);
	wxTreeItemId selectedItem = homeItem;

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

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuReceipts;

	child = treeMenu->AppendItem(accountsItem, "Receipts", 32, 32, itemData);

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

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuExpenses;

	child = treeMenu->AppendItem(accountsItem, "Expenes", 32, 32, itemData);

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

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	child = treeMenu->AppendItem(accountsItem, "Debt", 32, 32);

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

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	child = treeMenu->AppendItem(accountsItem, "Credits", 32, 32);

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

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	auto report = make_shared<Report>(-1);
	report->name = make_shared<wxString>("Expenses By Month");

	reports.push_back(report);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuReport;

	itemData->object = report;

	wxTreeItemId itemId = treeMenu->AppendItem(reportsItem, *report->name, 51, 51, itemData);

	/*treeMenu->Expand(accountsItem);

	if (selectedItem) {
		treeMenu->SelectItem(selectedItem);
	}*/
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

void TreeMenu::OnTreeSpecItemMenu(wxTreeEvent &event) {
	selectedMenuItem = event.GetItem();

	wxMenu *menu = new wxMenu();

	menu->Append(ID_OPEN_NEW_TAB, wxT("Open in New Tab"));
	menu->AppendSeparator();
	menu->Append(ID_ADD_ACCOUNT, wxT("Add Account..."));
	menu->Append(ID_EDIT_ACCOUNT, wxT("Edit Account..."));
	menu->Append(ID_DELETE_ACCOUNT, wxT("Delete Account"));
	menu->AppendSeparator();
	menu->Append(ID_ADD_TRANSACTION, wxT("Add Transaction..."));

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddAccount, this, ID_ADD_ACCOUNT);
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuEditAccount, this, ID_EDIT_ACCOUNT);
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuDeleteAccount, this, ID_DELETE_ACCOUNT);
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnMenuAddTransaction, this, ID_ADD_TRANSACTION);
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &TreeMenu::OnOpenNewTab, this, ID_OPEN_NEW_TAB);

	PopupMenu(menu, event.GetPoint());

	delete menu;
}

void TreeMenu::OnTreeItemSelect(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			auto account = std::static_pointer_cast<Account>(item->object);
			selectedAccountId = account->id;

			Settings::GetInstance().SetSelectedAccountId(selectedAccountId);

			if (OnAccountSelect) {
				OnAccountSelect(account);
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuHome) {
			if (OnHomeSelect) {
				OnHomeSelect();
			}
		}
		else if (item->type == TreeMenuItemTypes::MenuBudget) {
			if (OnBudgetsSelect) {
				OnBudgetsSelect();
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
	auto account = GetAccount();

	if (OnEditAccount) {
		OnEditAccount(account);
	}
}

void TreeMenu::OnMenuDeleteAccount(wxCommandEvent &event) {
	auto account = GetAccount();

	if (OnDeleteAccount) {
		OnDeleteAccount(account);
	}
}

void TreeMenu::OnMenuAddTransaction(wxCommandEvent &event) {
	if (OnAddTransaction) {
		OnAddTransaction();
	}
}

void TreeMenu::OnOpenNewTab(wxCommandEvent &event) {
	if (OnNewTab) {
		OnNewTab();
	}

	if (selectedMenuItem) {
		treeMenu->SelectItem(selectedMenuItem);
	}
}