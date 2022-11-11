#include "MainMenu.h"

using namespace Clerk::UI;

const int transactionsOffset = 1000;

MainMenu::MainMenu(CommandsInvoker& commandsInvoker): _commandsInvoker(commandsInvoker) {
	_menuFile = new wxMenu();	
	
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuType::NewAccount), wxT("New Account..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewBudget), wxT("New Budget..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewGoal), wxT("New Goal..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewScheduler), wxT("New Scheduler..."));
	_menuFile->Append(static_cast<int>(MainMenuType::NewAlert), wxT("New Alert..."));
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuType::Preferences), "Preferences...\tCtrl+P");
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuType::Exit), "E&xit\tCtrl+W");

	this->Append(_menuFile, "&File");

	wxMenu* menuHelp = new wxMenu();
	menuHelp->Append(static_cast<int>(MainMenuType::About), "&About...");

	this->Append(menuHelp, "&Help");

	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnMenuSelect, this);
}

MainMenu::~MainMenu() {
	delete _viewModel;
}

void MainMenu::SetViewModel(TransactionsMenuViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate([=]() {
		Update();
	});

	Update();
}

void MainMenu::Update() {
	auto transactions = _viewModel->GetRecents();

	if (transactions.size() == 0) {
		_menuFile->Insert(0, static_cast<int>(MainMenuType::NewTransaction), wxT("New Transaction...\tCtrl+T"));		
	}
	else {
		wxMenu* menu = new wxMenu();
		menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnTransactionSelect, this);

		menu->Append(static_cast<int>(MainMenuType::NewTransaction), wxT("New Transaction...\tCtrl+T"));
		menu->AppendSeparator();

		for (auto& transaction : transactions)
		{
			menu->Append(transactionsOffset + transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
		}

		_menuFile->Insert(0, static_cast<int>(MainMenuType::NewTransaction), wxT("New Transaction"), menu);
	}
}

void MainMenu::OnMenuSelect(wxCommandEvent& event) {
	MainMenuType type = static_cast<MainMenuType>(event.GetId());

	switch (type)
	{
		case Clerk::UI::MainMenuType::NewTransaction:
			_commandsInvoker.OnNewTransaction(-1);
			break;
		case Clerk::UI::MainMenuType::NewAccount:
			_commandsInvoker.OnNewAccount(AccountType::Deposit);
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
			_commandsInvoker.OnAbout();
			break;
		case Clerk::UI::MainMenuType::Preferences:
			_commandsInvoker.OnPreferences();
			break;
		case Clerk::UI::MainMenuType::Exit:
			_commandsInvoker.OnQuit();
			break;
		default:
			break;
	}
}

void MainMenu::OnTransactionSelect(wxCommandEvent& event) {
	int id = event.GetId();

	if (id >= transactionsOffset) {
		_commandsInvoker.OnCopyTransaction(id - transactionsOffset);
	}
	else if (id == static_cast<int>(MainMenuType::NewTransaction)) {
		_commandsInvoker.OnNewTransaction(-1);
	}
}