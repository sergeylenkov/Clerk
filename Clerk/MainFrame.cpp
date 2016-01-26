#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	wxLogDebug("%s", wxStandardPaths::Get().GetUserConfigDir().c_str());
	wxFileConfig *config = new wxFileConfig("", "", "C:\\Temp\\config.txt", "", wxCONFIG_USE_LOCAL_FILE);

	this->SetSize(wxSize(config->ReadLong("WindowWidth", 1000), config->ReadLong("WindowHeight", 800)));
	selectedAccountId = config->ReadLong("SelectedAccount", -1);

	delete config;

	wxImage image;

	imageList = new wxImageList(16, 16, true);

	if (image.LoadFile(wxT("Resources\\1.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\2.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\3.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\4.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\5.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\6.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\7.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\8.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\14.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\15.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\16.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\17.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\18.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\19.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\20.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\21.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\22.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\23.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\24.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\25.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\26.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\27.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\28.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	if (image.LoadFile(wxT("Resources\\29.png"), wxBITMAP_TYPE_PNG))
	{
		wxBitmap *bitmap = new wxBitmap(image);
		imageList->Add(*bitmap);

		delete bitmap;
	}

	wxMenu *menuFile = new wxMenu();

	menuFile->Append(wxID_ABOUT, "&About...");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, "E&xit");

	wxMenu *menuAccounts = new wxMenu();
	menuAccounts->Append(ID_ADD_ACCOUNT, wxT("Add Account..."));

	wxMenu *menuTransactions = new wxMenu();
	menuTransactions->Append(ID_ADD_TRANSACTION, wxT("Add Transaction..."));

	wxMenuBar *menuBar = new wxMenuBar;

	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuAccounts, "&Accounts");
	menuBar->Append(menuTransactions, "&Transactions");

	SetMenuBar(menuBar);

	wxSplitterWindow *splittermain = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);
	splittermain->SetSashGravity(0.5);
	splittermain->SetMinimumPaneSize(300);

	wxPanel *panel4 = new wxPanel(splittermain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	panel4->SetBackgroundColour(*wxWHITE);
	wxBoxSizer *boxSizer4 = new wxBoxSizer(wxVERTICAL);

	treeMenu = new wxTreeCtrl(panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT | wxTR_TWIST_BUTTONS | wxBORDER_NONE);

	treeMenu->AssignImageList(imageList);
	treeMenu->AddRoot("Accounts", -1, -1, 0);

	boxSizer4->Add(treeMenu, 1, wxEXPAND | wxALL, 0);
	panel4->SetSizer(boxSizer4);

	CreateStatusBar();
	SetStatusText("");

	wxPanel *panel = new wxPanel(splittermain, wxID_ANY);

	panel2 = new wxPanel(panel, wxID_ANY);
	panel3 = new wxPanel(panel, wxID_ANY);

	transactionList = new TransactionsListPanel(panel2, wxID_ANY);
	transactionList->OnEditTransaction = std::bind(&MainFrame::EditTransaction, this);
	transactionList->OnPeriodChanged = std::bind(&MainFrame::UpdateStatus, this);
	//transactionList->OnSearchChanged = std::bind(&MainFrame::UpdateStatus, this);

	homePanel = new HomePanel(panel3, wxID_ANY);

	wxBoxSizer *boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(transactionList, 1, wxEXPAND | wxALL, 0);
	panel2->SetSizer(boxSizer);

	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(homePanel, 1, wxEXPAND | wxALL, 0);
	panel3->SetSizer(boxSizer);

	vbox = new wxBoxSizer(wxVERTICAL);

	panel->SetSizer(vbox);

	splittermain->SplitVertically(panel4, panel, 1);

	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
	menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);

	menuAccounts->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddAccount, this, ID_ADD_ACCOUNT);
	menuTransactions->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddTransaction, this, ID_ADD_TRANSACTION);

	treeMenu->Bind(wxEVT_TREE_SEL_CHANGED, &MainFrame::OnTreeItemSelect, this);
	treeMenu->Bind(wxEVT_TREE_ITEM_MENU, &MainFrame::OnTreeSpecItemMenu, this);

	UpdateAccountsTree();
}

MainFrame::~MainFrame() 
{
	wxFileConfig *config = new wxFileConfig("", "", "C:\\Temp\\config.txt", "", wxCONFIG_USE_LOCAL_FILE);
	
	config->Write("SelectedAccount", selectedAccountId);
	config->Write("WindowWidth", this->GetSize().GetWidth());
	config->Write("WindowHeight", this->GetSize().GetHeight());

	config->Flush();

	delete config;

	delete treeMenu;
	delete transactionList;
}

void MainFrame::UpdateAccountsTree() 
{
	wxTreeItemId rootItem = treeMenu->GetRootItem();

	accounts.clear();

	treeMenu->DeleteChildren(rootItem);

	TreeMenuItemData *itemData = new TreeMenuItemData();
	itemData->type = TreeMenuItemTypes::MenuHome;

	wxTreeItemId homeItem = treeMenu->AppendItem(rootItem, "Home", 14, 14, itemData);
	wxTreeItemId accountsItem = treeMenu->AppendItem(homeItem, "Accounts", 20, 20);
	wxTreeItemId reportsItem = treeMenu->AppendItem(homeItem, "Reports", 16, 16);

	wxTreeItemId child = treeMenu->AppendItem(accountsItem, "Deposits", 17, 17);
	wxTreeItemId selectedItem = homeItem;

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Deposit))
	{
		int icon = 21;

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);
		accounts.push_back(account);

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	child = treeMenu->AppendItem(accountsItem, "Receipts", 18, 18);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Receipt))
	{
		int icon = 22;

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);
		accounts.push_back(account);

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	child = treeMenu->AppendItem(accountsItem, "Expenes", 19, 19);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Expens))
	{
		int icon = 23;

		TreeMenuItemData *itemData = new TreeMenuItemData();
		itemData->type = TreeMenuItemTypes::MenuAccount;
		itemData->object = account;

		wxTreeItemId itemId = treeMenu->AppendItem(child, *account->name, icon, icon, itemData);
		accounts.push_back(account);

		if (account->id == selectedAccountId) {
			selectedItem = itemId;
		}
	}

	treeMenu->ExpandAll();

	if (selectedItem) {
		treeMenu->SelectItem(selectedItem);
	}
}

void MainFrame::UpdateTransactionList(Account *account) 
{
	transactionList->SetAccount(account);
	transactionList->Update();

	float amount = 0;

	if (account->type == AccountTypes::Deposit) {
		amount = DataHelper::GetInstance().GetBalance(account);
	} else if (account->type == AccountTypes::Expens) {
		amount = transactionList->GetBalance();
	} else {
		amount = DataHelper::GetInstance().GetToAmountSum(account, &transactionList->GetFromDate(), &transactionList->GetToDate());
	}

	if (account->creditLimit > 0.0) {
		SetStatusText(wxString::Format("%s: %.2f (%.2f %.2f) %s", static_cast<const char*>(account->name->c_str()), account->creditLimit + amount, account->creditLimit, amount, static_cast<const char*>(account->currency->shortName->c_str())));
	} else {
		SetStatusText(wxString::Format("%s: %.2f %s", static_cast<const char*>(account->name->c_str()), amount, static_cast<const char*>(account->currency->shortName->c_str())));
	}
}

void MainFrame::UpdateStatus() {
	wxTreeItemId itemId = treeMenu->GetSelection();

	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			UpdateTransactionList(account.get());
		}
	}
}

void MainFrame::OnQuit(wxCommandEvent &event)
{
	Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox("Personal finance application", "About Clerk", wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnTreeSpecItemMenu(wxTreeEvent &event)
{
	wxMenu *menu = new wxMenu();

	menu->Append(ID_ADD_ACCOUNT, wxT("Add Account..."));
	menu->Append(ID_EDIT_ACCOUNT, wxT("Edit Account..."));
	menu->Append(ID_DELETE_ACCOUNT, wxT("Delete Account..."));

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAddAccount, this, ID_ADD_ACCOUNT);
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnEditAccount, this, ID_EDIT_ACCOUNT);
	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnDeleteAccount, this, ID_DELETE_ACCOUNT);

	PopupMenu(menu, event.GetPoint());

	delete menu;
}

void MainFrame::OnTreeItemSelect(wxTreeEvent &event)
{
	wxTreeItemId itemId = event.GetItem();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {		
		vbox->Detach(panel2);
		vbox->Detach(panel3);

		if (item->type == TreeMenuItemTypes::MenuAccount) {
			vbox->Add(panel2, 1, wxEXPAND | wxALL, 0);

			panel2->Show();
			panel3->Hide();

			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			UpdateTransactionList(account.get());

			selectedAccountId = account->id;
		} else {
			vbox->Add(panel3, 1, wxEXPAND | wxALL, 0);

			panel2->Hide();
			panel3->Show();

			homePanel->Update();
		}

		vbox->Layout();
	}
}

void MainFrame::OnAddTransaction(wxCommandEvent &event) {
	AddTransaction();
}

void MainFrame::AddTransaction() {
	transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 300);
	
	transactionFrame->Show(true);
	transactionFrame->CenterOnParent();

	wxTreeItemId itemId = treeMenu->GetSelection();
	
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			shared_ptr<Account> account = static_pointer_cast<Account>(item->object);
			auto transaction = make_shared<Transaction>();

			transactionFrame->SetTransaction(transaction);
			transactionFrame->SetAccount(account);
		}
	}

	transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);
}

void MainFrame::EditTransaction() {
	auto transaction = transactionList->GetTransaction();

	if (transaction) {
		transactionFrame = new TransactionFrame(this, wxT("Transaction"), 0, 0, 450, 300);

		transactionFrame->Show(true);
		transactionFrame->CenterOnParent();

		transactionFrame->SetTransaction(transaction);
		transactionFrame->OnClose = std::bind(&MainFrame::OnTransactionClose, this);
	}
}

void MainFrame::OnTransactionClose() {
	delete transactionFrame;

	wxTreeItemId itemId = treeMenu->GetSelection();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			UpdateTransactionList(account.get());
		}
	}

	homePanel->Update();
}

void MainFrame::OnAddAccount(wxCommandEvent &event) {
	AddAccount();
}

void MainFrame::OnEditAccount(wxCommandEvent &event) {
	EditAccount();
}

void MainFrame::OnDeleteAccount(wxCommandEvent &event) {
	DeleteAccount();
}

void MainFrame::AddAccount() {
	accountFrame = new AccountFrame(this, wxT("Account"), 0, 0, 300, 330);

	accountFrame->Show(true);
	accountFrame->CenterOnParent();

	std::shared_ptr<Account> account = make_shared<Account>();

	account->id = -1;
	account->name = make_shared<wxString>("");
	account->note = make_shared<wxString>("");
	account->type = AccountTypes::Deposit;
	account->iconId = 0;
	account->orderId = 1000;
	account->currency = make_shared<Currency>(152);
	
	accountFrame->SetAccount(account);

	accountFrame->OnClose = std::bind(&MainFrame::OnAccountClose, this);
}

void MainFrame::EditAccount() {
	wxTreeItemId itemId = treeMenu->GetSelection();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			accountFrame = new AccountFrame(this, wxT("Account"), 0, 0, 300, 330);

			accountFrame->Show(true);
			accountFrame->CenterOnParent();

			accountFrame->OnClose = std::bind(&MainFrame::OnAccountClose, this);

			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			accountFrame->SetAccount(account);
		}
	}
}

void MainFrame::DeleteAccount() {
	wxTreeItemId itemId = treeMenu->GetSelection();
	TreeMenuItemData *item = (TreeMenuItemData *)treeMenu->GetItemData(itemId);

	if (item != NULL) {
		if (item->type == TreeMenuItemTypes::MenuAccount) {
			std::shared_ptr<Account> account = std::static_pointer_cast<Account>(item->object);
			account->Delete();

			UpdateAccountsTree();
		}
	}
}

void MainFrame::OnAccountClose() {
	delete accountFrame;

	UpdateAccountsTree();
}