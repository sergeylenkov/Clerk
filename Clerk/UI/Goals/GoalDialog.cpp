#include "GoalDialog.h"

GoalDialog::GoalDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, Icons& icons):
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
	_icons(icons)
{
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));

	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize labelSize = FromDIP(wxSize(70, -1));
	wxSize listSize = FromDIP(wxSize(100, -1));

	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *nameLabel = new wxStaticText(mainPanel, wxID_ANY, _("Name:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL);

	_nameField = new wxTextCtrl(mainPanel, wxID_ANY);
	horizontalSizer->Add(_nameField, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *periodLabel = new wxStaticText(mainPanel, wxID_ANY, _("Date:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL);
	
	_datePicker = new wxDatePickerCtrl(mainPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	horizontalSizer->Add(_datePicker);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *amountLabel = new wxStaticText(mainPanel, wxID_ANY, _("Amount:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(amountLabel, 0, wxALIGN_CENTER_VERTICAL);

	_amountField = new AmountField(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, listSize);
	horizontalSizer->Add(_amountField);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* accountLabel = new wxStaticText(mainPanel, wxID_ANY, _("Accounts:"), wxDefaultPosition, labelSize);
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

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, indent * 2);

	SetSizer(mainSizer);
	Layout();

	Centre(wxBOTH);

	_accountsList->Bind(wxEVT_LIST_ITEM_CHECKED, &GoalDialog::OnAccountsChange, this);
	_accountsList->Bind(wxEVT_LIST_ITEM_UNCHECKED, &GoalDialog::OnAccountsChange, this);

	_nameField->Bind(wxEVT_KILL_FOCUS, &GoalDialog::OnNameKillFocus, this);
	_amountField->Bind(wxEVT_KILL_FOCUS, &GoalDialog::OnAmountKillFocus, this);
	_datePicker->Bind(wxEVT_DATE_CHANGED, &GoalDialog::OnDateChanged, this);

	okButton->Bind(wxEVT_BUTTON, &GoalDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &GoalDialog::OnCancel, this);

	Bind(wxEVT_CHAR_HOOK, &GoalDialog::OnKeyDown, this);
}

GoalDialog::~GoalDialog() {
	delete _viewModel;
}

void GoalDialog::SetViewModel(GoalViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate = [&](GoalViewModelField field) {
		
	};

	Update();
}

void GoalDialog::Update() {
	_nameField->SetValue(_viewModel->GetName());
	_datePicker->SetValue(_viewModel->GetDate());
	_amountField->SetValue(Format::Amount(_viewModel->GetAmount()));

	UpdateAccounts();
}

void GoalDialog::UpdateAccounts() {
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

void GoalDialog::OnAccountsChange(wxListEvent& event) {
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

void GoalDialog::OnNameKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetName(_nameField->GetValue());
}

void GoalDialog::OnAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetAmount(_amountField->GetFloatValue());
}

void GoalDialog::OnDateChanged(wxDateEvent& event) {
	_viewModel->SetDate(_datePicker->GetValue());
}

void GoalDialog::OnOK(wxCommandEvent& event) {
	_viewModel->Save();

	Close();
}

void GoalDialog::OnCancel(wxCommandEvent& event) {
	Close();
}

void GoalDialog::OnKeyDown(wxKeyEvent& event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}