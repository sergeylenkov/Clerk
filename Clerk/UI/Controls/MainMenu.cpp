#include "MainMenu.h"

using namespace Clerk::UI;

const int transactionsOffset = 1000;

MainMenu::MainMenu(TransactionsMenuViewModel& viewModel, CommandsInvoker& commandsInvoker, Icons& icons):
	_viewModel(viewModel),
	_commandsInvoker(commandsInvoker),
	_icons(icons)
{
	_menuFile = new wxMenu();	
	
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuType::NewAccount), _("New Account..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewBudget), _("New Budget..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewGoal), _("New Goal..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewScheduler), _("New Scheduler..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewAlert), _("New Alert..."));
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuType::Preferences), _("Preferences...\tCtrl+P"));
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuType::Exit), _("Exit\tCtrl+W"));

	Append(_menuFile, _("File"));

	wxMenu* menuHelp = new wxMenu();
	menuHelp->Append(static_cast<int>(MainMenuType::About), _("About..."));

	Append(menuHelp, _("Help"));
	
	_viewModel.OnUpdate([&]() {
		Update();
	});

	Update();

	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnMenuSelect, this);
}

MainMenu::~MainMenu() {
	delete& _viewModel;
}

void MainMenu::Update() {
	int menuId = static_cast<int>(MainMenuType::NewTransaction);
	wxMenuItem* menuItem = _menuFile->FindItem(menuId);

	if (menuItem) {
		_menuFile->Remove(menuItem);
		delete menuItem;
	}

	auto transactions = _viewModel.GetRecents();

	if (transactions.size() == 0) {
		wxMenuItem* item = _menuFile->Insert(0, menuId, _("New Transaction...\tCtrl+T"));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));
	}
	else {
		wxMenu* menu = new wxMenu();
		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnTransactionSelect, this);

		wxMenuItem* item = menu->Append(menuId, _("New Transaction...\tCtrl+T"));
		item->SetBitmap(wxBitmap("ICON_ADD", wxBITMAP_TYPE_PNG_RESOURCE));

		menu->AppendSeparator();

		for (auto& transaction : transactions)
		{
			wxMenuItem* transactionItem = menu->Append(transactionsOffset + transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
			transactionItem->SetBitmap(*_icons.GetAccountIcon(transaction->toAccount->icon));
		}

		_menuFile->Insert(0, menuId, _("New Transaction"), menu);
	}
}

void MainMenu::OnMenuSelect(wxCommandEvent& event) {
	MainMenuType type = static_cast<MainMenuType>(event.GetId());

	switch (type)
	{
		case Clerk::UI::MainMenuType::NewTransaction:
			_commandsInvoker.NewTransaction(-1);
			break;
		case Clerk::UI::MainMenuType::NewAccount:
			_commandsInvoker.NewAccount(AccountType::Deposit);
			break;
		case Clerk::UI::MainMenuType::NewBudget:
			break;
		case Clerk::UI::MainMenuType::NewScheduler:
			break;
		case Clerk::UI::MainMenuType::NewGoal:
			break;
		case Clerk::UI::MainMenuType::NewAlert:
			break;
		case Clerk::UI::MainMenuType::About:
			_commandsInvoker.OpenAbout();
			break;
		case Clerk::UI::MainMenuType::Preferences:
			_commandsInvoker.OpenPreferences();
			break;
		case Clerk::UI::MainMenuType::Exit:
			_commandsInvoker.Quit();
			break;
		default:
			break;
	}
}

void MainMenu::OnTransactionSelect(wxCommandEvent& event) {
	int id = event.GetId();

	if (id >= transactionsOffset) {
		_commandsInvoker.CopyTransaction(id - transactionsOffset);
	}
	else if (id == static_cast<int>(MainMenuType::NewTransaction)) {
		_commandsInvoker.NewTransaction(-1);
	}
}