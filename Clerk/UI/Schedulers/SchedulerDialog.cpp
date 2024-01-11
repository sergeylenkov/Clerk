#include "SchedulerDialog.h"

SchedulerDialog::SchedulerDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height, Icons& icons, DataContext& context):
	wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)),
	_icons(icons),
	_context(context)
{
	SetBackgroundColour(wxColor(*wxWHITE));

	SetIcon(wxICON(APP_ICON));

	int indent = FromDIP(5);
	int bottomIndent = FromDIP(15);
	wxSize labelSize = FromDIP(wxSize(40, -1));
	wxSize listSize = FromDIP(wxSize(100, -1));
	wxSize amountSize = FromDIP(wxSize(80, -1));

	int daysValue;
	int dayValue;
	int monthValue;
	int weekValue;

	wxIntegerValidator<int> daysValidator(&daysValue, wxNUM_VAL_DEFAULT);
	daysValidator.SetRange(1, 365);

	wxIntegerValidator<int> dayValidator(&dayValue, wxNUM_VAL_DEFAULT);
	dayValidator.SetRange(1, 31);

	wxIntegerValidator<int> weekValidator(&weekValue, wxNUM_VAL_DEFAULT);
	weekValidator.SetRange(1, 52);

	wxIntegerValidator<int> monthValidator(&monthValue, wxNUM_VAL_DEFAULT);
	monthValidator.SetRange(1, 12);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* nameLabel = new wxStaticText(mainPanel, wxID_ANY, _("Name:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL);

	_nameField = new wxTextCtrl(mainPanel, wxID_ANY);
	horizontalSizer->Add(_nameField, 1, wxEXPAND);

	panelSizer->Add(horizontalSizer, 0, wxEXPAND | wxBOTTOM, bottomIndent);

	wxBoxSizer *staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, _("Recurrence pattern")), wxHORIZONTAL);

	wxPanel *buttonsPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(80, -1), wxTAB_TRAVERSAL);

	wxBoxSizer *buttonsSizer = new wxBoxSizer(wxVERTICAL);

	dailyButton = new wxRadioButton(buttonsPanel, wxID_ANY, _("Daily"));
	buttonsSizer->Add(dailyButton, 0, wxALL, 5);

	weeklyButton = new wxRadioButton(buttonsPanel, wxID_ANY, _("Weekly"));
	buttonsSizer->Add(weeklyButton, 0, wxALL, 5);

	monthlyButton = new wxRadioButton(buttonsPanel, wxID_ANY, _("Monthly"));
	buttonsSizer->Add(monthlyButton, 0, wxALL, 5);

	yearlyButton = new wxRadioButton(buttonsPanel, wxID_ANY, _("Yearly"));
	buttonsSizer->Add(yearlyButton, 0, wxALL, 5);

	buttonsPanel->SetSizer(buttonsSizer);
	buttonsPanel->Layout();

	staticBoxSizer->Add(buttonsPanel, 0, wxALL, 5);

	wxStaticLine *staticLine = new wxStaticLine(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	staticBoxSizer->Add(staticLine, 0, wxEXPAND | wxALL, 5);	

	patternPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	staticBoxSizer->Add(patternPanel, 1, wxEXPAND | wxALL, 5);

	patternSizer = new wxBoxSizer(wxVERTICAL);

	patternPanel->SetSizer(patternSizer);
	patternPanel->Layout();

	patternSizer->Fit(patternPanel);

	panelSizer->Add(staticBoxSizer, 1, wxEXPAND | wxALL, 10);

	staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Transaction")), wxVERTICAL);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* fromLabel = new wxStaticText(mainPanel, wxID_ANY, _("From:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_fromList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_fromList, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent * 2);

	_fromAmountField = new AmountField(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, amountSize);
	horizontalSizer->Add(_fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_fromCurrencyLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB");
	horizontalSizer->Add(_fromCurrencyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* toLabel = new wxStaticText(mainPanel, wxID_ANY, _("To:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	_toList = new wxBitmapComboBox(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(_toList, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, indent * 2);

	_toAmountField = new AmountField(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, amountSize);
	horizontalSizer->Add(_toAmountField, 0, wxALL, 5);

	_toCurrencyLabel = new wxStaticText(mainPanel, wxID_ANY, "RUB");
	horizontalSizer->Add(_toCurrencyLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* tagsLabel = new wxStaticText(mainPanel, wxID_ANY, _("Tags:"), wxDefaultPosition, labelSize);
	horizontalSizer->Add(tagsLabel);

	_tagsField = new TagsField(mainPanel, context.GetTagsService());
	_tagsField->OnChange = [&]() {
		_viewModel->SetTags(_tagsField->GetTags());
		Layout();
	};

	horizontalSizer->Add(_tagsField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	panelSizer->Add(staticBoxSizer, 1, wxEXPAND | wxALL, 10);

	//

	dailyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *dailyPatternLabel = new wxStaticText(dailyPatternPanel, wxID_ANY, _("Every"));
	horizontalSizer->Add(dailyPatternLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	dailyDayField = new wxTextCtrl(dailyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, daysValidator);
	horizontalSizer->Add(dailyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *daysLabel = new wxStaticText(dailyPatternPanel, wxID_ANY, _("days"));
	horizontalSizer->Add(daysLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	dailyPatternPanel->SetSizer(horizontalSizer);
	dailyPatternPanel->Layout();

	horizontalSizer->Fit(dailyPatternPanel);

	patternSizer->Add(dailyPatternPanel, 0, wxALIGN_TOP);

	//

	weeklyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *m_staticText1011 = new wxStaticText(weeklyPatternPanel, wxID_ANY, _("Every"));
	horizontalSizer->Add(m_staticText1011, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	weeklyWeekField = new wxTextCtrl(weeklyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, weekValidator);
	horizontalSizer->Add(weeklyWeekField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText911 = new wxStaticText(weeklyPatternPanel, wxID_ANY, _("weeks on:"));
	horizontalSizer->Add(m_staticText911, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	verticalSizer->Add(horizontalSizer, 0, wxALIGN_TOP | wxBOTTOM, 5);

	wxWrapSizer *wrapSizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

	mondayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, _("Monday"));
	wrapSizer->Add(mondayCheckBox, 0, wxALL, 5);

	tuesdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, _("Tuesday"));
	wrapSizer->Add(tuesdayCheckBox, 0, wxALL, 5);

	wednesdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, _("Wednesday"));
	wrapSizer->Add(wednesdayCheckBox, 0, wxALL, 5);

	thursdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, _("Thursday"));
	wrapSizer->Add(thursdayCheckBox, 0, wxALL, 5);

	fridayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, _("Friday"));
	wrapSizer->Add(fridayCheckBox, 0, wxALL, 5);

	saturdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, _("Saturday"));
	wrapSizer->Add(saturdayCheckBox, 0, wxALL, 5);

	sundayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, _("Sunday"));
	wrapSizer->Add(sundayCheckBox, 0, wxALL, 5);

	verticalSizer->Add(wrapSizer, 1, wxEXPAND);

	weeklyPatternPanel->SetSizer(verticalSizer);
	weeklyPatternPanel->Layout();

	wrapSizer->Fit(weeklyPatternPanel);

	patternSizer->Add(weeklyPatternPanel, 1, wxEXPAND);

	//

	monthlyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *m_staticText912 = new wxStaticText(monthlyPatternPanel, wxID_ANY, _("Day"));
	horizontalSizer->Add(m_staticText912, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	monthlyDayField = new wxTextCtrl(monthlyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, dayValidator);
	horizontalSizer->Add(monthlyDayField, 0, wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText1012 = new wxStaticText(monthlyPatternPanel, wxID_ANY, _("every"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText1012, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	monthlyMonthField = new wxTextCtrl(monthlyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, monthValidator);
	horizontalSizer->Add(monthlyMonthField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText10121 = new wxStaticText(monthlyPatternPanel, wxID_ANY, _("month(s)"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText10121, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	monthlyPatternPanel->SetSizer(horizontalSizer);
	monthlyPatternPanel->Layout();

	horizontalSizer->Fit(monthlyPatternPanel);
	
	patternSizer->Add(monthlyPatternPanel, 0, wxALIGN_TOP);

	//

	yearlyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *m_staticText913 = new wxStaticText(yearlyPatternPanel, wxID_ANY, _("Every"));
	horizontalSizer->Add(m_staticText913, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	yearlyDayField = new wxTextCtrl(yearlyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, dayValidator);
	horizontalSizer->Add(yearlyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxString months[] = { _("January"), _("February"), _("March"), _("April"), _("May"), _("June"), _("Jule"), _("August"), _("September"), _("October"), _("November"), _("December") };

	yearlyMonthChoice = new wxComboBox(yearlyPatternPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 12, months, wxCB_READONLY);
	yearlyMonthChoice->SetSelection(0);
	horizontalSizer->Add(yearlyMonthChoice, 0, wxLEFT | wxRIGHT, 5);

	yearlyPatternPanel->SetSizer(horizontalSizer);
	yearlyPatternPanel->Layout();

	horizontalSizer->Fit(yearlyPatternPanel);
	
	patternSizer->Add(yearlyPatternPanel, 0, wxALIGN_TOP);

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

	okButton->Bind(wxEVT_BUTTON, &SchedulerDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &SchedulerDialog::OnCancel, this);

	_fromList->Bind(wxEVT_COMBOBOX, &SchedulerDialog::OnFromAccountSelect, this);
	_toList->Bind(wxEVT_COMBOBOX, &SchedulerDialog::OnToAccountSelect, this);
	_fromAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerDialog::OnFromAmountKillFocus, this);
	_toAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerDialog::OnToAmountKillFocus, this);
	dailyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);
	weeklyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);
	monthlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);
	yearlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);

	Bind(wxEVT_CHAR_HOOK, &SchedulerDialog::OnKeyDown, this);
}

SchedulerDialog::~SchedulerDialog() {
	delete _viewModel;
}

void SchedulerDialog::SetViewModel(SchedulerViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate = [&](SchedulerViewModelField field) {
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
	};

	Update();

	_nameField->SetFocus();
}

void SchedulerDialog::Update() {
	UpdateFromList();
	UpdateToList();

	SelectFromAccount(_viewModel->GetFromAccountIndex());
	SelectToAccount(_viewModel->GetToAccountIndex());

	_nameField->SetValue(_viewModel->GetName());
	_fromAmountField->SetValue(Format::Amount(_viewModel->GetFromAmount()));
	_toAmountField->SetValue(Format::Amount(_viewModel->GetToAmount()));
	_tagsField->SetTags(_viewModel->GetTags());
}

	/*nameField->SetValue(*scheduler->name);
	fromAmountField->SetValue(wxString::Format("%.2f", scheduler->fromAmount));
	toAmountField->SetValue(wxString::Format("%.2f", scheduler->toAmount));
	tagsField->SetValue(*scheduler->tags);

	if (scheduler->fromAccount) {
		for (unsigned int i = 0; i < fromAccounts.size(); i++) {
			if (scheduler->fromAccount->id == fromAccounts[i]->id) {
				SelectFromAccount(i);
				UpdateToList(fromAccounts[i]);

				break;
			}
		}
	}
	else {
		SelectFromAccount(0);
	}

	if (scheduler->toAccount) {
		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (scheduler->toAccount->id == toAccounts[i]->id) {
				SelectToAccount(i);
				break;
			}
		}
	}
	else {
		SelectToAccount(0);
	}

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();

	SelectPatternType(scheduler->type);

	if (scheduler->type == Scheduler::Type::Daily) {
		dailyDayField->SetValue(wxString::Format("%d", scheduler->day));
	}

	if (scheduler->type == Scheduler::Type::Weekly) {
		weeklyWeekField->SetValue(wxString::Format("%d", scheduler->week));
		SelectWeekday(scheduler->day);		
	}

	if (scheduler->type == Scheduler::Type::Monthly) {
		monthlyDayField->SetValue(wxString::Format("%d", scheduler->day));
		monthlyMonthField->SetValue(wxString::Format("%d", scheduler->month));
	}

	if (scheduler->type == Scheduler::Type::Yearly) {
		yearlyDayField->SetValue(wxString::Format("%d", scheduler->day));
		yearlyMonthChoice->SetSelection(scheduler->month);
	}

	nameField->SetFocus();*/

void SchedulerDialog::UpdateFromList() {
	auto accounts = _viewModel->GetFromAccounts();

	_fromList->Clear();

	for (auto& account : accounts) {
		_fromList->Append(account->name, *_icons.GetAccountIcon(account->icon));
	}
}

void SchedulerDialog::UpdateToList() {
	auto accounts = _viewModel->GetToAccounts();

	_toList->Clear();

	for (auto& account : accounts) {
		_toList->Append(account->name, *_icons.GetAccountIcon(account->icon));
	}
}

void SchedulerDialog::SelectFromAccount(int index) {
	auto account = _viewModel->GetFromAccounts()[index];

	_fromList->Select(index);
	_fromCurrencyLabel->SetLabel(account->currency->shortName);
}

void SchedulerDialog::SelectToAccount(int index) {
	auto account = _viewModel->GetToAccounts()[index];

	_toList->Select(index);
	_toCurrencyLabel->SetLabel(account->currency->shortName);
}

void SchedulerDialog::OnFromAccountSelect(wxCommandEvent& event) {
	int index = _fromList->GetSelection();
	_viewModel->SetFromAccount(index);
}

void SchedulerDialog::OnToAccountSelect(wxCommandEvent& event) {
	int index = _toList->GetSelection();
	_viewModel->SetToAccount(index);
}

void SchedulerDialog::OnFromAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetFromAmount(_fromAmountField->GetFloatValue());
}

void SchedulerDialog::OnToAmountKillFocus(wxFocusEvent& event) {
	event.Skip();

	_viewModel->SetToAmount(_toAmountField->GetFloatValue());
}

void SchedulerDialog::OnOK(wxCommandEvent &event) {
	/*scheduler->name = make_shared<wxString>(nameField->GetValue());
	scheduler->fromAccount = fromAccounts[fromList->GetSelection()];
	scheduler->toAccount = toAccounts[toList->GetSelection()];
	scheduler->tags = make_shared<wxString>(tagsField->GetValue());
	scheduler->type = type;

	double amountValue;

	wxString value = fromAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&amountValue);

	scheduler->fromAmount = amountValue;

	value = toAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&amountValue);

	scheduler->toAmount = amountValue;

	unsigned long intValue;

	if (type == Scheduler::Type::Daily) {
		dailyDayField->GetValue().ToULong(&intValue);
		scheduler->day = intValue;
	}

	if (type == Scheduler::Type::Weekly) {
		weeklyWeekField->GetValue().ToULong(&intValue);
		scheduler->week = intValue;

		scheduler->day = 1;

		if (mondayCheckBox->GetValue()) {
			scheduler->day = 1;
		}

		if (tuesdayCheckBox->GetValue()) {
			scheduler->day = 2;
		}

		if (wednesdayCheckBox->GetValue()) {
			scheduler->day = 3;
		}

		if (thursdayCheckBox->GetValue()) {
			scheduler->day = 4;
		}

		if (fridayCheckBox->GetValue()) {
			scheduler->day = 5;
		}

		if (saturdayCheckBox->GetValue()) {
			scheduler->day = 6;
		}

		if (sundayCheckBox->GetValue()) {
			scheduler->day = 7;
		}
	}

	if (type == Scheduler::Type::Monthly) {
		monthlyDayField->GetValue().ToULong(&intValue);
		scheduler->day = intValue;

		monthlyMonthField->GetValue().ToULong(&intValue);
		scheduler->month = intValue;
	}

	if (type == Scheduler::Type::Yearly) {
		yearlyDayField->GetValue().ToULong(&intValue);
		scheduler->day = intValue;

		scheduler->month = yearlyMonthChoice->GetSelection();
	}

	//TODO moved method to interactor
	//scheduler->Save();

	Close();

	if (OnClose) {
		OnClose();
	}*/
	_viewModel->Save();

	Close();
}

void SchedulerDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void SchedulerDialog::OnPatternSelect(wxCommandEvent &event) {
	if (dailyButton->GetValue()) {
		SelectPatternType(SchedulerType::Daily);
	}

	if (weeklyButton->GetValue()) {
		SelectPatternType(SchedulerType::Weekly);
	}

	if (monthlyButton->GetValue()) {
		SelectPatternType(SchedulerType::Monthly);
	}

	if (yearlyButton->GetValue()) {
		SelectPatternType(SchedulerType::Yearly);
	}
}

void SchedulerDialog::SelectPatternType(SchedulerType type) {
	type = type;

	dailyPatternPanel->Hide();
	weeklyPatternPanel->Hide();
	monthlyPatternPanel->Hide();
	yearlyPatternPanel->Hide();

	if (type == SchedulerType::Daily) {
		dailyButton->SetValue(true);
		dailyPatternPanel->Show();
	}

	if (type == SchedulerType::Weekly) {
		weeklyButton->SetValue(true);
		weeklyPatternPanel->Show();
	}

	if (type == SchedulerType::Monthly) {
		monthlyButton->SetValue(true);
		monthlyPatternPanel->Show();
	}

	if (type == SchedulerType::Yearly) {
		yearlyButton->SetValue(true);
		yearlyPatternPanel->Show();
	}

	patternPanel->Layout();
}

void SchedulerDialog::SelectWeekday(int day) {
	mondayCheckBox->SetValue(false);
	tuesdayCheckBox->SetValue(false);
	wednesdayCheckBox->SetValue(false);
	thursdayCheckBox->SetValue(false);
	fridayCheckBox->SetValue(false);
	saturdayCheckBox->SetValue(false);
	sundayCheckBox->SetValue(false);

	switch (day)
	{
	case 1:
		mondayCheckBox->SetValue(true);
		break;

	case 2:
		tuesdayCheckBox->SetValue(true);
		break;

	case 3:
		wednesdayCheckBox->SetValue(true);
		break;

	case 4:
		thursdayCheckBox->SetValue(true);
		break;

	case 5:
		fridayCheckBox->SetValue(true);
		break;

	case 6:
		saturdayCheckBox->SetValue(true);
		break;

	case 7:
		sundayCheckBox->SetValue(true);
		break;

	default:
		mondayCheckBox->SetValue(true);
		break;
	}
}

void SchedulerDialog::OnKeyDown(wxKeyEvent &event) {
	if ((int)event.GetKeyCode() == 27) {
		event.StopPropagation();
		Close();
	}
	else {
		event.Skip();
	}
}