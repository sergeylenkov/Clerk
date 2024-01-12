#include "SchedulerTransactionPanel.h"

SchedulerTransactionPanel::SchedulerTransactionPanel(wxWindow* parent, const wxPoint& position, const wxSize& size, Icons& icons, TagsService& tagsService):
	wxPanel(parent, wxID_ANY, position, size),
	_icons(icons),
	_tagsService(tagsService)
{
	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize labelSize = FromDIP(wxSize(40, -1));
	wxSize amountSize = FromDIP(wxSize(80, -1));

	wxStaticBoxSizer* mainSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Transaction")), wxVERTICAL);
	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* fromLabel = new wxStaticText(this, wxID_ANY, _("From:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL);

	_fromList = new wxBitmapComboBox(this, wxID_ANY, "0.0", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_fromList, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent * 2);

	_fromAmountField = new AmountField(this, wxID_ANY, wxEmptyString, wxDefaultPosition, amountSize);
	horizontalSizer->Add(_fromAmountField, 0, wxALIGN_CENTER_VERTICAL);

	_fromCurrencyLabel = new wxStaticText(this, wxID_ANY, "RUB");
	horizontalSizer->Add(_fromCurrencyLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	mainSizer->Add(horizontalSizer, 0, wxEXPAND | wxALL, indent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* toLabel = new wxStaticText(this, wxID_ANY, _("To:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL);

	_toList = new wxBitmapComboBox(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_toList, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent * 2);

	_toAmountField = new AmountField(this, wxID_ANY, wxEmptyString, wxDefaultPosition, amountSize);
	horizontalSizer->Add(_toAmountField, 0, wxALIGN_CENTER_VERTICAL);

	_toCurrencyLabel = new wxStaticText(this, wxID_ANY, "RUB");
	horizontalSizer->Add(_toCurrencyLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	mainSizer->Add(horizontalSizer, 0, wxEXPAND | wxALL, indent);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* tagsLabel = new wxStaticText(this, wxID_ANY, _("Tags:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(tagsLabel, 0);

	_tagsField = new TagsField(this, _tagsService);
	_tagsField->OnChange = [&]() {
		_viewModel->SetTags(_tagsField->GetTags());
		Layout();
	};

	horizontalSizer->Add(_tagsField, 1, wxEXPAND);

	mainSizer->Add(horizontalSizer, 0, wxEXPAND | wxALL, indent);

	SetSizer(mainSizer);
	Layout();

	_fromList->Bind(wxEVT_COMBOBOX, &SchedulerTransactionPanel::OnFromAccountSelect, this);
	_toList->Bind(wxEVT_COMBOBOX, &SchedulerTransactionPanel::OnToAccountSelect, this);
	_fromAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerTransactionPanel::OnFromAmountKillFocus, this);
	_toAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerTransactionPanel::OnToAmountKillFocus, this);
}

void SchedulerTransactionPanel::SetViewModel(SchedulerViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate([&](SchedulerViewModelField field) {
		if (field == SchedulerViewModelField::FromAmount || field == SchedulerViewModelField::ToAmount) {
			_fromAmountField->SetValue(Format::Amount(_viewModel->GetFromAmount()));
			_toAmountField->SetValue(Format::Amount(_viewModel->GetToAmount()));
		}

		if (field == SchedulerViewModelField::FromAccount) {
			UpdateToList();
			SelectToAccount(_viewModel->GetToAccountIndex());
		}

		if (field == SchedulerViewModelField::ToAccount) {
			UpdateFromList();
			SelectFromAccount(_viewModel->GetFromAccountIndex());
		}
	});

	Update();
}

void SchedulerTransactionPanel::Update() {
	UpdateFromList();
	UpdateToList();

	SelectFromAccount(_viewModel->GetFromAccountIndex());
	SelectToAccount(_viewModel->GetToAccountIndex());

	_fromAmountField->SetValue(Format::Amount(_viewModel->GetFromAmount()));
	_toAmountField->SetValue(Format::Amount(_viewModel->GetToAmount()));
	_tagsField->SetTags(_viewModel->GetTags());
}

void SchedulerTransactionPanel::UpdateFromList() {
	auto accounts = _viewModel->GetFromAccounts();

	_fromList->Clear();

	for (auto& account : accounts) {
		_fromList->Append(account->name, *_icons.GetAccountIcon(account->icon));
	}
}

void SchedulerTransactionPanel::UpdateToList() {
	auto accounts = _viewModel->GetToAccounts();

	_toList->Clear();

	for (auto& account : accounts) {
		_toList->Append(account->name, *_icons.GetAccountIcon(account->icon));
	}
}

void SchedulerTransactionPanel::SelectFromAccount(int index) {
	auto account = _viewModel->GetFromAccounts()[index];

	_fromList->Select(index);
	_fromCurrencyLabel->SetLabel(account->currency->shortName);
}

void SchedulerTransactionPanel::SelectToAccount(int index) {
	auto account = _viewModel->GetToAccounts()[index];

	_toList->Select(index);
	_toCurrencyLabel->SetLabel(account->currency->shortName);
}

void SchedulerTransactionPanel::OnFromAccountSelect(wxCommandEvent& event) {
	int index = _fromList->GetSelection();
	_viewModel->SetFromAccount(index);
}

void SchedulerTransactionPanel::OnToAccountSelect(wxCommandEvent& event) {
	int index = _toList->GetSelection();
	_viewModel->SetToAccount(index);
}

void SchedulerTransactionPanel::OnFromAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetFromAmount(_fromAmountField->GetFloatValue());
}

void SchedulerTransactionPanel::OnToAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetToAmount(_toAmountField->GetFloatValue());
}