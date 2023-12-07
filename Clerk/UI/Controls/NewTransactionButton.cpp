#include "NewTransactionButton.h"

using namespace Clerk::UI;

NewTransactionButton::NewTransactionButton(wxWindow* parent, CommandsInvoker& commandsInvoker, Icons& icons) : DropDownButton(parent, wxID_ANY, _("Add Transaction"), wxDefaultPosition, wxDefaultSize),
	_commandsInvoker(commandsInvoker), _icons(icons) {
	SetBackgroundColour(wxColour(255, 255, 255));
	SetSize(FromDIP(wxSize(-1, 40)));

	Bind(wxEVT_BUTTON, &NewTransactionButton::OnAddTransaction, this);
}

NewTransactionButton::~NewTransactionButton() {
	delete _viewModel;
}

void NewTransactionButton::SetViewModel(TransactionsMenuViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate([&]() {
		Update();
	});

	Update();
}

void NewTransactionButton::Update() {
	ClearMenu();

	wxMenu* menu = GetMenu();	

	for (auto& transaction : _viewModel->GetRecents())
	{
		wxMenuItem* transactionItem = menu->Append(transaction->id, wxString::Format("%s > %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
		transactionItem->SetBitmap(*_icons.GetAccountIcon(transaction->toAccount->icon));
	}

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &NewTransactionButton::OnMenuSelect, this);
}

void NewTransactionButton::OnAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnNewTransaction(-1);
}

void NewTransactionButton::OnMenuSelect(wxCommandEvent& event) {
	_commandsInvoker.OnCopyTransaction(event.GetId());
}