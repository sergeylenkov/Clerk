#include "MainMenu.h"

using namespace Clerk::UI;

MainMenu::MainMenu(CommandsInvoker& commandsInvoker): _commandsInvoker(commandsInvoker) {
	_menuFile = new wxMenu();
	_menuHelp = new wxMenu();
	_menuTransaction = new wxMenu();

	_menuHelp->Append(static_cast<int>(MainMenuTypes::About), "&About...");

	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuTypes::AddAccount), wxT("New Account..."));
	_menuFile->Append(static_cast<int>(MainMenuTypes::AddBudget), wxT("New Budget..."));
	_menuFile->Append(static_cast<int>(MainMenuTypes::AddGoal), wxT("New Goal..."));
	_menuFile->Append(static_cast<int>(MainMenuTypes::AddScheduler), wxT("New Scheduler..."));
	_menuFile->Append(static_cast<int>(MainMenuTypes::AddAlert), wxT("New Alert..."));
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuTypes::Preferences), "Preferences...\tCtrl+P");
	_menuFile->AppendSeparator();
	_menuFile->Append(static_cast<int>(MainMenuTypes::Exit), "E&xit\tCtrl+W");

	this->Append(_menuFile, "&File");
	this->Append(_menuHelp, "&Help");

	_menuHelp->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAbout, this, static_cast<int>(MainMenuTypes::About));
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAddAccount, this, static_cast<int>(MainMenuTypes::AddAccount));
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAddBudget, this, static_cast<int>(MainMenuTypes::AddBudget));
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAddGoal, this, static_cast<int>(MainMenuTypes::AddGoal));
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAddScheduler, this, static_cast<int>(MainMenuTypes::AddScheduler));
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnPreferences, this, static_cast<int>(MainMenuTypes::Preferences));
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnQuit, this, static_cast<int>(MainMenuTypes::Exit));
	_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAddAlert, this, static_cast<int>(MainMenuTypes::AddAlert));
}

void MainMenu::SetTransactions(std::vector<std::shared_ptr<TransactionViewModel>> transactions) {
	if (transactions.size() == 0) {
		_menuFile->Insert(0, static_cast<int>(MainMenuTypes::AddTransaction), wxT("New Transaction...\tCtrl+T"));
		_menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAddTransaction, this, static_cast<int>(MainMenuTypes::AddTransaction));
	}
	else {
		_menuFile->Insert(0, static_cast<int>(MainMenuTypes::AddTransaction), wxT("New Transaction"), _menuTransaction);

		wxMenuItem* item = (wxMenuItem*)_menuTransaction->GetMenuItems().GetLast();

		while (item != NULL)
		{
			_menuTransaction->Destroy(item);
			item = (wxMenuItem*)_menuTransaction->GetMenuItems().GetLast();
		}

		_menuTransaction->Append(static_cast<int>(MainMenuTypes::AddTransaction), wxT("New Transaction...\tCtrl+T"));
		_menuTransaction->AppendSeparator();

		for (auto& transaction : transactions)
		{
			_menuTransaction->Append(transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
		}

		_menuTransaction->Bind(wxEVT_COMMAND_MENU_SELECTED, &MainMenu::OnAddTransaction, this);
	}
}

void MainMenu::OnQuit(wxCommandEvent& event) {
	_commandsInvoker.OnQuit();
}

void MainMenu::OnAbout(wxCommandEvent& event) {
	_commandsInvoker.OnAbout();
}

void MainMenu::OnPreferences(wxCommandEvent& event) {
	_commandsInvoker.OnPreferences();
}

void MainMenu::OnAddAccount(wxCommandEvent& event) {

}

void MainMenu::OnAddBudget(wxCommandEvent& event) {

}

void MainMenu::OnAddScheduler(wxCommandEvent& event) {

}

void MainMenu::OnAddGoal(wxCommandEvent& event) {

}

void MainMenu::OnAddAlert(wxCommandEvent& event) {

}

void MainMenu::OnAddTransaction(wxCommandEvent& event) {

}