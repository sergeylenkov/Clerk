#include "AlertDialog.h"

AlertDialog::AlertDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, Icons& icons):
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
	_icons(icons)
{
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));

	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize labelSize = FromDIP(wxSize(70, -1));
	wxSize listSize = FromDIP(wxSize(100, -1));

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *nameLabel = new wxStaticText(mainPanel, wxID_ANY, _("Name:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL);

	_nameField = new wxTextCtrl(mainPanel, wxID_ANY);
	horizontalSizer->Add(_nameField, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* messageLabel = new wxStaticText(mainPanel, wxID_ANY, _("Message:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(messageLabel, 0, wxALIGN_CENTER_VERTICAL);

	_messageField = new wxTextCtrl(mainPanel, wxID_ANY);
	horizontalSizer->Add(_messageField, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);	

	wxStaticText* typeLabel = new wxStaticText(mainPanel, wxID_ANY, _("Type:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(typeLabel, 0, wxALIGN_CENTER_VERTICAL);

	_typeList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, listSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_typeList);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* periodLabel = new wxStaticText(mainPanel, wxID_ANY, _("Period:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL);

	_periodList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, listSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_periodList);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* conditionLabel = new wxStaticText(mainPanel, wxID_ANY, _("Condition:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(conditionLabel, 0, wxALIGN_CENTER_VERTICAL);

	_conditionList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, listSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_conditionList);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* importanceLabel = new wxStaticText(mainPanel, wxID_ANY, _("Importance:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(importanceLabel, 0, wxALIGN_CENTER_VERTICAL);

	_importanceList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, listSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_importanceList);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *amountLabel = new wxStaticText(mainPanel, wxID_ANY, _("Amount:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL);

	_amountField = new AmountField(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, listSize);
	horizontalSizer->Add(_amountField);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *accountLabel = new wxStaticText(mainPanel, wxID_ANY, _("Accounts:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(accountLabel);

	_accountsList = new wxListCtrl(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_NO_HEADER);
	_accountsList->SetImageList(_icons.GetImageList(), wxIMAGE_LIST_SMALL);

	horizontalSizer->Add(_accountsList, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 1, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxButton* okButton = new wxButton(mainPanel, wxID_ANY, _("OK"));
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent);

	wxButton* cancelButton = new wxButton(mainPanel, wxID_ANY, _("Cancel"));
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, indent * 2);

	SetSizer(mainSizer);
	Layout();

	Centre(wxBOTH);

	_typeList->AppendString(_("Receipt"));
	_typeList->AppendString(_("Expense"));
	_typeList->AppendString(_("Balance"));	

	_periodList->AppendString(_("Week"));
	_periodList->AppendString(_("Month"));
	_periodList->AppendString(_("Year"));	

	_conditionList->AppendString(_("Less"));
	_conditionList->AppendString(_("More"));
	_conditionList->AppendString(_("Equal"));

	_importanceList->AppendString(_("Low"));
	_importanceList->AppendString(_("Medium"));
	_importanceList->AppendString(_("High"));

	_typeList->Bind(wxEVT_COMBOBOX, &AlertDialog::OnTypeSelect, this);
	_periodList->Bind(wxEVT_COMBOBOX, &AlertDialog::OnPeriodSelect, this);
	_conditionList->Bind(wxEVT_COMBOBOX, &AlertDialog::OnConditionSelect, this);
	_importanceList->Bind(wxEVT_COMBOBOX, &AlertDialog::OnImportanceSelect, this);
	_accountsList->Bind(wxEVT_LIST_ITEM_CHECKED, &AlertDialog::OnAccountsChange, this);
	_accountsList->Bind(wxEVT_LIST_ITEM_UNCHECKED, &AlertDialog::OnAccountsChange, this);
	
	_nameField->Bind(wxEVT_KILL_FOCUS, &AlertDialog::OnNameKillFocus, this);
	_messageField->Bind(wxEVT_KILL_FOCUS, &AlertDialog::OnMessageKillFocus, this);
	_amountField->Bind(wxEVT_KILL_FOCUS, &AlertDialog::OnAmountKillFocus, this);

	okButton->Bind(wxEVT_BUTTON, &AlertDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &AlertDialog::OnCancel, this);

	Bind(wxEVT_CHAR_HOOK, &AlertDialog::OnKeyDown, this);
}

AlertDialog::~AlertDialog() {
	delete _viewModel;
}

void AlertDialog::SetViewModel(AlertViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate = [&](AlertViewModelField field) {
		if (field == AlertViewModelField::Type) {
			UpdateAccounts();
		}
	};

	Update();
}

void AlertDialog::Update() {
	_nameField->SetValue(_viewModel->GetName());
	_messageField->SetValue(_viewModel->GetMessage());
	_typeList->SetSelection(static_cast<int>(_viewModel->GetType()));
	_periodList->SetSelection(static_cast<int>(_viewModel->GetPeriod()));
	_conditionList->SetSelection(static_cast<int>(_viewModel->GetCondition()));
	_importanceList->SetSelection(static_cast<int>(_viewModel->GetImportance()));
	_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));

	UpdateAccounts();
}

void AlertDialog::UpdateAccounts() {
	_accountsList->ClearAll();
	_accountsList->EnableCheckBoxes(true);

	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(280);

	_accountsList->InsertColumn(0, column);	

	int i = 0;
	auto selectedIds = _viewModel->GetAccountsIds();
	_accounts = _viewModel->GetAccounts();

	for (auto& account : _accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(account->id);

		_accountsList->InsertItem(listItem);
		_accountsList->SetItem(i, 0, account->name);

		_accountsList->SetItemImage(listItem, _icons.GetIconIndexForAccount(account->icon));

		if (std::find(selectedIds.begin(), selectedIds.end(), account->id) != selectedIds.end()) {
			_accountsList->CheckItem(i, true);
		}

		i++;
	}
}

void AlertDialog::OnTypeSelect(wxCommandEvent &event) {
	_viewModel->SetType(static_cast<AlertType>(_typeList->GetSelection()));
}

void AlertDialog::OnPeriodSelect(wxCommandEvent &event) {
	_viewModel->SetPeriod(static_cast<AlertPeriod>(_periodList->GetSelection()));
}

void AlertDialog::OnConditionSelect(wxCommandEvent& event) {
	_viewModel->SetCondition(static_cast<AlertCondition>(_conditionList->GetSelection()));
}

void AlertDialog::OnImportanceSelect(wxCommandEvent& event) {
	_viewModel->SetImportance(static_cast<AlertImportance>(_importanceList->GetSelection()));
}

void AlertDialog::OnAccountsChange(wxListEvent& event) {
	long itemIndex = -1;
	std::vector<int> selectedIds;

	for (;;) {
		itemIndex = _accountsList->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);

		if (itemIndex == -1) {
			break;
		}

		bool checked = _accountsList->IsItemChecked(itemIndex);

		if (checked) {
			auto account = _accounts[itemIndex];
			selectedIds.push_back(account->id);
		}
	}

	_viewModel->SetAccountsIds(selectedIds);
}

void AlertDialog::OnNameKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetName(_nameField->GetValue());
}

void AlertDialog::OnMessageKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetMessage(_messageField->GetValue());
}

void AlertDialog::OnAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetAmount(_amountField->GetFloatValue());
}

void AlertDialog::OnOK(wxCommandEvent &event) {
	_viewModel->Save();

	Close();
}

void AlertDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void AlertDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}