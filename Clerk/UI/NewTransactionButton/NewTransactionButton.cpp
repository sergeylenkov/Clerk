#include "NewTransactionButton.h"

using namespace Clerk::UI;

NewTransactionButton::NewTransactionButton(NewTransactionViewModel& viewModel, CommandsInvoker& commandsInvoker, Icons& icons, wxWindow* parent, const wxPoint& pos, const wxSize& size):
	DropDownButton(parent, icons.GetIconByType(IconType::Plus).GetBitmap(wxSize(24, 24)), _("Add Transaction"), pos, size),
	_viewModel(viewModel),
	_commandsInvoker(commandsInvoker),
	_icons(icons)
{
	SetBackgroundColour(wxColour(255, 255, 255));
	SetSize(size);
	
	_viewModel.OnUpdate([&]() {
		Update();
	});

	Update();

	Bind(wxEVT_BUTTON, &NewTransactionButton::OnClick, this);
}

NewTransactionButton::~NewTransactionButton() {
	delete& _viewModel;
}

void NewTransactionButton::Update() {
	ClearMenu();

	wxMenu* menu = GetMenu();	

	for (auto& transaction : _viewModel.GetRecents())
	{
		wxMenuItem* transactionItem = menu->Append(transaction->id, wxString::Format("%s > %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
		transactionItem->SetBitmap(*_icons.GetAccountIcon(transaction->toAccount->icon));
	}

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &NewTransactionButton::OnMenuSelect, this);
}

void NewTransactionButton::OnClick(wxCommandEvent& event) {
	_commandsInvoker.NewTransaction(-1);
}

void NewTransactionButton::OnMenuSelect(wxCommandEvent& event) {
	_commandsInvoker.CopyTransaction(event.GetId());
}