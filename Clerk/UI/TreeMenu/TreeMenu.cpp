#include "TreeMenu.h"

using namespace Clerk::UI;

TreeMenu::TreeMenu(wxWindow* parent, Icons& icons, CommandsInvoker& commandsInvoker) : wxPanel(parent), _icons(icons), _commandsInvoker(commandsInvoker)
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	_treeMenu = new wxTreeCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS | wxTR_NO_LINES | wxTR_FULL_ROW_HIGHLIGHT |  wxBORDER_NONE);
	_treeMenu->SetBackgroundColour(wxColour(245, 245, 245, 1));	
	_treeMenu->SetForegroundColour(wxColour(68, 68, 68, 1));	
	_treeMenu->AssignImageList(_icons.GetImageList());

	mainSizer->Add(_treeMenu, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();	

	_treeMenu->Bind(wxEVT_TREE_SEL_CHANGED, &TreeMenu::OnTreeItemSelect, this);
	_treeMenu->Bind(wxEVT_TREE_ITEM_MENU, &TreeMenu::OnTreeSpecItemMenu, this);
	_treeMenu->Bind(wxEVT_TREE_ITEM_EXPANDED, &TreeMenu::OnTreeItemExpanded, this);
	_treeMenu->Bind(wxEVT_TREE_ITEM_COLLAPSED, &TreeMenu::OnTreeItemCollapsed, this);
	_treeMenu->Bind(wxEVT_TREE_BEGIN_DRAG, &TreeMenu::OnBeginDrag, this);
	_treeMenu->Bind(wxEVT_TREE_END_DRAG, &TreeMenu::OnEndDrag, this);

	CreateMenu();
}

TreeMenu::~TreeMenu() {
	delete _viewModel;
}

void TreeMenu::SetViewModel(TreeMenuViewModel* viewModel) {
	_viewModel = viewModel;

	Update();
}

void TreeMenu::CreateMenu() {
	wxTreeItemId rootItem = _treeMenu->AddRoot("", -1, -1, 0);

	TreeMenuItemData* itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Dashboard;

	wxTreeItemId homeItem = _treeMenu->AppendItem(rootItem, "Dashboard", 0, 0, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Accounts;

	wxTreeItemId accountsItem = _treeMenu->AppendItem(rootItem, "Accounts", 1, 1, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Receipts;

	_receiptsItem = _treeMenu->AppendItem(accountsItem, "Receipts", 2, 2, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Deposits;

	_depositsItem = _treeMenu->AppendItem(accountsItem, "Deposits", 2,  2, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Virtual;

	_virtualItem = _treeMenu->AppendItem(accountsItem, "Virtual", 2, 2, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Expenses;

	_expensesItem = _treeMenu->AppendItem(accountsItem, "Expenses", 2, 2, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Debts;

	_debtsItem = _treeMenu->AppendItem(accountsItem, "Debts", 2, 2, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Archive;

	_archiveItem = _treeMenu->AppendItem(accountsItem, "Archive", 3, 3, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Reports;

	_reportsItem = _treeMenu->AppendItem(rootItem, "Reports", 4, 4, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Budgets;

	wxTreeItemId budgetsItem = _treeMenu->AppendItem(rootItem, "Budgets", 6, 6, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Goals;

	wxTreeItemId goalsItem = _treeMenu->AppendItem(rootItem, "Goals", 8, 8, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Schedulers;

	wxTreeItemId schedulersItem = _treeMenu->AppendItem(rootItem, "Schedulers", 7, 7, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Alerts;

	wxTreeItemId alertsItem = _treeMenu->AppendItem(rootItem, "Alerts", 12, 12, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Tags;

	wxTreeItemId tagsItem = _treeMenu->AppendItem(rootItem, "Tags", 9, 9, itemData);

	itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Trash;

	_trashItem = _treeMenu->AppendItem(rootItem, "Trash", 0, 0, itemData);
}

void TreeMenu::Update() {
	for (auto& account : _viewModel->GetReceiptsAccounts())
	{
		AddAccountItem(_receiptsItem, account);
	}

	for (auto& account : _viewModel->GetDepositsAccounts())
	{
		AddAccountItem(_depositsItem, account);
	}

	for (auto& account : _viewModel->GetExpensesAccounts())
	{
		AddAccountItem(_expensesItem, account);
	}

	for (auto& account : _viewModel->GetDebtsAccounts())
	{
		AddAccountItem(_debtsItem, account);
	}

	for (auto& account : _viewModel->GetVirtualsAccounts())
	{
		AddAccountItem(_virtualItem, account);
	}

	for (auto& account : _viewModel->GetArchiveAccounts())
	{
		AddAccountItem(_archiveItem, account);
	}

	for (auto& report : _viewModel->GetReports())
	{
		TreeMenuItemData* itemData = new TreeMenuItemData();

		itemData->type = TreeMenuItemType::Report;
		//itemData->object = report;

		wxTreeItemId itemId = _treeMenu->AppendItem(_reportsItem, report->name, 5, 5, itemData);
	}

	SetIsTrashEmpty(_viewModel->IsTrashEmpty());
}

void TreeMenu::AddAccountItem(wxTreeItemId& parent, AccountPresentationModel* account) {
	int iconIndex = _icons.GetIconIndexForAccount(account->icon);

	TreeMenuItemData* itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemType::Account;
	itemData->object = account;

	wxTreeItemId itemId = _treeMenu->AppendItem(parent, account->name, iconIndex, iconIndex, itemData);
}

void TreeMenu::RestoreState() {
	wxTreeItemId rootItem = _treeMenu->GetRootItem();
	ExpandItem(rootItem);	
}

void TreeMenu::ExpandItem(wxTreeItemId &item) {
	wxTreeItemIdValue cookie;
	wxTreeItemId child = _treeMenu->GetFirstChild(item, cookie);

	while (child.IsOk())
	{
		TreeMenuItemData *data = (TreeMenuItemData *)_treeMenu->GetItemData(child);

		if (data != NULL && Settings::GetInstance().IsMenuExpanded(static_cast<int>(data->type))) {
			_treeMenu->Expand(child);
		}

		if (_treeMenu->HasChildren(child)) {
			ExpandItem(child);
		}

		child = _treeMenu->GetNextChild(item, cookie);
	}
}

AccountPresentationModel* TreeMenu::GetContextMenuAccount() {
	if (_contextMenuItem != NULL) {
		TreeMenuItemData *item = (TreeMenuItemData *)_treeMenu->GetItemData(_contextMenuItem);

		if (item->type == TreeMenuItemType::Account) {
			AccountPresentationModel* account = (AccountPresentationModel*)item->object;
			return account;
		}
	}

	return nullptr;
}

void TreeMenu::OnTreeSpecItemMenu(wxTreeEvent &event) {
	_contextMenuItem = event.GetItem();

	wxMenu* menu = nullptr;

	TreeMenuItemData* item = (TreeMenuItemData *)_treeMenu->GetItemData(_contextMenuItem);

	if (item->type == TreeMenuItemType::Account) {
		auto account = GetContextMenuAccount();

		if (account) {
			auto transactions = _viewModel->GetRecentsTransactions(*account);
			menu = new AccountContextMenu(_commandsInvoker, *account, transactions);
		}
	} else if (item->type == TreeMenuItemType::Accounts || item->type == TreeMenuItemType::Deposits || item->type == TreeMenuItemType::Receipts
		      || item->type == TreeMenuItemType::Expenses || item->type == TreeMenuItemType::Virtual || item->type == TreeMenuItemType::Debts) {
		menu = new AccountsContextMenu(_commandsInvoker, item->type);
	}
	else if (item->type != TreeMenuItemType::Reports) {
		menu = new DefaultContextMenu(_commandsInvoker, item->type);
	}

	if (menu != nullptr) {
		PopupMenu(menu, event.GetPoint());
		delete menu;
	}
}

void TreeMenu::OnTreeItemSelect(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)_treeMenu->GetItemData(itemId);
	wxTreeItemId oldItem = event.GetOldItem();
}

void TreeMenu::OnTreeItemExpanded(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)_treeMenu->GetItemData(itemId);

	if (item != NULL) {
		Settings::GetInstance().AddExpandedMenu(static_cast<int>(item->type));
	}
}

void TreeMenu::OnTreeItemCollapsed(wxTreeEvent &event) {
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)_treeMenu->GetItemData(itemId);

	if (item != NULL) {
		Settings::GetInstance().RemoveExpandedMenu(static_cast<int>(item->type));
	}
}

void TreeMenu::OnBeginDrag(wxTreeEvent &event) {
	_draggedMenuItem = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)_treeMenu->GetItemData(_draggedMenuItem);

	if (item->type == TreeMenuItemType::Account) {
		event.Allow();
	}
}

void TreeMenu::OnEndDrag(wxTreeEvent &event) {	
	wxTreeItemId itemId = event.GetItem();

	if (itemId.IsOk()) {
		wxTreeItemId parent = _treeMenu->GetItemParent(itemId);

		TreeMenuItemData *draggedData = (TreeMenuItemData *)_treeMenu->GetItemData(_draggedMenuItem);
		TreeMenuItemData *itemData = (TreeMenuItemData *)_treeMenu->GetItemData(itemId);

		if (draggedData->type == itemData->type) {
			auto account = (AccountPresentationModel*)draggedData->object;

			TreeMenuItemData *itemData = new TreeMenuItemData();
			itemData->type = draggedData->type;
			itemData->object = account;

			_treeMenu->InsertItem(parent, itemId, account->name, account->icon, account->icon, itemData);
			
			_treeMenu->Delete(_draggedMenuItem);

			ReorderAccounts(parent);
		}
	}
}

void TreeMenu::ReorderAccounts(wxTreeItemId &item) {
	wxTreeItemIdValue cookie;
	wxTreeItemId child = _treeMenu->GetFirstChild(item, cookie);
	int orderId = 0;

	while (child.IsOk())
	{
		TreeMenuItemData *data = (TreeMenuItemData *)_treeMenu->GetItemData(child);
		auto account = (AccountPresentationModel*)data->object;

		//TODO moved methos ti interactor
		//account->orderId = orderId++;
		//account->Save();

		child = _treeMenu->GetNextChild(item, cookie);
	}
}

void TreeMenu::SetIsTrashEmpty(bool isEmpty) {	
	int icon = 10;

	if (isEmpty) {
		icon = 11;
	}

	_treeMenu->SetItemImage(_trashItem, icon, wxTreeItemIcon_Normal);
	_treeMenu->SetItemImage(_trashItem, icon, wxTreeItemIcon_Selected);
}