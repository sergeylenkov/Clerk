#include "AddTransactionButton.h"

using namespace Clerk::UI;

AddTransactionButton::AddTransactionButton(wxWindow* parent, CommandsInvoker& commandsInvoker) : DropDownButton(parent, wxID_ANY, wxT("Add Transaction"), wxDefaultPosition, wxSize(-1, 34)), _commandsInvoker(commandsInvoker) {
	this->SetBackgroundColour(wxColour(255, 255, 255));

	this->Bind(wxEVT_BUTTON, &AddTransactionButton::OnAddTransaction, this);
}

void AddTransactionButton::SetTransactions(std::vector<std::shared_ptr<TransactionViewModel>> transactions) {
	wxMenu* menu = this->GetMenu();
	
	wxMenuItem* item = (wxMenuItem*)menu->GetMenuItems().GetLast();

	while (item != NULL)
	{
		menu->Destroy(item);
		item = (wxMenuItem*)menu->GetMenuItems().GetLast();
	}

	for (auto& transaction : transactions)
	{
		menu->Append(transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
	}

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &AddTransactionButton::OnMenuAddTransaction, this);
}

void AddTransactionButton::OnAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnNewTransaction(-1);
}

void AddTransactionButton::OnMenuAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnNewTransaction(event.GetId());
}