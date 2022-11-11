#include "AddTransactionButton.h"

using namespace Clerk::UI;

AddTransactionButton::AddTransactionButton(wxWindow* parent, CommandsInvoker& commandsInvoker) : DropDownButton(parent, wxID_ANY, wxT("Add Transaction"), wxDefaultPosition, wxSize(-1, 34)), _commandsInvoker(commandsInvoker) {
	this->SetBackgroundColour(wxColour(255, 255, 255));

	this->Bind(wxEVT_BUTTON, &AddTransactionButton::OnAddTransaction, this);
}

AddTransactionButton::~AddTransactionButton() {
	delete _viewModel;
}

void AddTransactionButton::SetViewModel(TransactionsMenuViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate([=]() {
		Update();
		});

	Update();
}

void AddTransactionButton::Update() {
	wxMenu* menu = this->GetMenu();
	
	wxMenuItem* item = (wxMenuItem*)menu->GetMenuItems().GetLast();

	while (item != NULL)
	{
		menu->Destroy(item);
		item = (wxMenuItem*)menu->GetMenuItems().GetLast();
	}

	for (auto& transaction : _viewModel->GetRecents())
	{
		menu->Append(transaction->id, wxString::Format("%s › %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
	}

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &AddTransactionButton::OnMenuSelect, this);
}

void AddTransactionButton::OnAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnNewTransaction(-1);
}

void AddTransactionButton::OnMenuSelect(wxCommandEvent& event) {
	_commandsInvoker.OnCopyTransaction(event.GetId());
}