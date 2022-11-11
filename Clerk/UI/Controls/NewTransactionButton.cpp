﻿#include "NewTransactionButton.h"

using namespace Clerk::UI;

NewTransactionButton::NewTransactionButton(wxWindow* parent, CommandsInvoker& commandsInvoker) : DropDownButton(parent, wxID_ANY, wxT("Add Transaction"), wxDefaultPosition, wxSize(-1, 34)), _commandsInvoker(commandsInvoker) {
	this->SetBackgroundColour(wxColour(255, 255, 255));

	this->Bind(wxEVT_BUTTON, &NewTransactionButton::OnAddTransaction, this);
}

NewTransactionButton::~NewTransactionButton() {
	delete _viewModel;
}

void NewTransactionButton::SetViewModel(TransactionsMenuViewModel* viewModel) {
	_viewModel = viewModel;
	_viewModel->OnUpdate([=]() {
		Update();
		});

	Update();
}

void NewTransactionButton::Update() {
	wxMenu* menu = this->GetMenu();
	
	wxMenuItem* item = (wxMenuItem*)menu->GetMenuItems().GetLast();

	while (item != NULL)
	{
		menu->Destroy(item);
		item = (wxMenuItem*)menu->GetMenuItems().GetLast();
	}

	for (auto& transaction : _viewModel->GetRecents())
	{
		menu->Append(transaction->id, wxString::Format("%s > %s (%s)", transaction->fromAccount->name, transaction->toAccount->name, transaction->tagsString));
	}

	menu->Bind(wxEVT_COMMAND_MENU_SELECTED, &NewTransactionButton::OnMenuSelect, this);
}

void NewTransactionButton::OnAddTransaction(wxCommandEvent& event) {
	_commandsInvoker.OnNewTransaction(-1);
}

void NewTransactionButton::OnMenuSelect(wxCommandEvent& event) {
	_commandsInvoker.OnCopyTransaction(event.GetId());
}