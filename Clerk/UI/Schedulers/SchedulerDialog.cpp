#include "SchedulerDialog.h"

SchedulerDialog::SchedulerDialog(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetIcon(wxICON(APP_ICON));

	wxString allowedChars[13] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", ",", " " };
	wxArrayString chars(13, allowedChars);
	wxTextValidator amountValidator(wxFILTER_INCLUDE_CHAR_LIST);
	amountValidator.SetIncludes(chars);

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
	mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	nameLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer *staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(mainPanel, wxID_ANY, wxT("Recurrence pattern")), wxHORIZONTAL);

	wxPanel *buttonsPanel = new wxPanel(mainPanel, wxID_ANY, wxDefaultPosition, wxSize(80, -1), wxTAB_TRAVERSAL);

	wxBoxSizer *buttonsSizer = new wxBoxSizer(wxVERTICAL);

	dailyButton = new wxRadioButton(buttonsPanel, wxID_ANY, wxT("Daily"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(dailyButton, 0, wxALL, 5);

	weeklyButton = new wxRadioButton(buttonsPanel, wxID_ANY, wxT("Weekly"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(weeklyButton, 0, wxALL, 5);

	monthlyButton = new wxRadioButton(buttonsPanel, wxID_ANY, wxT("Monthly"), wxDefaultPosition, wxDefaultSize, 0);
	buttonsSizer->Add(monthlyButton, 0, wxALL, 5);

	yearlyButton = new wxRadioButton(buttonsPanel, wxID_ANY, wxT("Yearly"), wxDefaultPosition, wxDefaultSize, 0);
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

	staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Transaction")), wxVERTICAL);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	fromLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("From:"), wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountField = new wxTextCtrl(mainPanel, wxID_ANY, wxT("1000"), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	horizontalSizer->Add(fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("RUB"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	toLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("To:"), wxDefaultPosition, wxSize(40, -1), 0);
	toLabel->Wrap(-1);
	horizontalSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toList = new wxBitmapComboBox(mainPanel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountField = new wxTextCtrl(mainPanel, wxID_ANY, wxT("1000"), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	horizontalSizer->Add(toAmountField, 0, wxALL, 5);

	toAmountLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("RUB"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	tagsLabel = new wxStaticText(mainPanel, wxID_ANY, wxT("Tags:"), wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	tagsField = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(tagsField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	panelSizer->Add(staticBoxSizer, 1, wxEXPAND | wxALL, 10);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(mainPanel, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	cancelButton = new wxButton(mainPanel, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	panelSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	//

	dailyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *dailyPatternLabel = new wxStaticText(dailyPatternPanel, wxID_ANY, wxT("Every"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(dailyPatternLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	dailyDayField = new wxTextCtrl(dailyPatternPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, daysValidator);
	horizontalSizer->Add(dailyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *daysLabel = new wxStaticText(dailyPatternPanel, wxID_ANY, wxT("days"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(daysLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	dailyPatternPanel->SetSizer(horizontalSizer);
	dailyPatternPanel->Layout();

	horizontalSizer->Fit(dailyPatternPanel);

	patternSizer->Add(dailyPatternPanel, 0, wxALIGN_TOP | wxALL, 0);

	//

	weeklyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *m_staticText1011 = new wxStaticText(weeklyPatternPanel, wxID_ANY, wxT("Every"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText1011, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	weeklyWeekField = new wxTextCtrl(weeklyPatternPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, weekValidator);
	horizontalSizer->Add(weeklyWeekField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText911 = new wxStaticText(weeklyPatternPanel, wxID_ANY, wxT("weeks on:"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText911, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	verticalSizer->Add(horizontalSizer, 0, wxALIGN_TOP | wxBOTTOM, 5);

	wxWrapSizer *wrapSizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

	mondayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, wxT("Monday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(mondayCheckBox, 0, wxALL, 5);

	tuesdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, wxT("Tuesday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(tuesdayCheckBox, 0, wxALL, 5);

	wednesdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, wxT("Wednesday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(wednesdayCheckBox, 0, wxALL, 5);

	thursdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, wxT("Thursday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(thursdayCheckBox, 0, wxALL, 5);

	fridayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, wxT("Friday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(fridayCheckBox, 0, wxALL, 5);

	saturdayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, wxT("Saturday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(saturdayCheckBox, 0, wxALL, 5);

	sundayCheckBox = new wxRadioButton(weeklyPatternPanel, wxID_ANY, wxT("Sunday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(sundayCheckBox, 0, wxALL, 5);

	verticalSizer->Add(wrapSizer, 1, wxEXPAND, 5);

	weeklyPatternPanel->SetSizer(verticalSizer);
	weeklyPatternPanel->Layout();

	wrapSizer->Fit(weeklyPatternPanel);

	patternSizer->Add(weeklyPatternPanel, 1, wxEXPAND | wxALL, 0);

	//

	monthlyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *m_staticText912 = new wxStaticText(monthlyPatternPanel, wxID_ANY, wxT("Day"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText912, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	monthlyDayField = new wxTextCtrl(monthlyPatternPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, dayValidator);
	horizontalSizer->Add(monthlyDayField, 0, wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText1012 = new wxStaticText(monthlyPatternPanel, wxID_ANY, wxT("every"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText1012, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	monthlyMonthField = new wxTextCtrl(monthlyPatternPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, monthValidator);
	horizontalSizer->Add(monthlyMonthField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText10121 = new wxStaticText(monthlyPatternPanel, wxID_ANY, wxT("month(s)"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText10121, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	monthlyPatternPanel->SetSizer(horizontalSizer);
	monthlyPatternPanel->Layout();

	horizontalSizer->Fit(monthlyPatternPanel);
	
	patternSizer->Add(monthlyPatternPanel, 0, wxALIGN_TOP | wxALL, 0);

	//

	yearlyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxTAB_TRAVERSAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *m_staticText913 = new wxStaticText(yearlyPatternPanel, wxID_ANY, wxT("Every"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText913, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	yearlyDayField = new wxTextCtrl(yearlyPatternPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, dayValidator);
	horizontalSizer->Add(yearlyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxString months[] = { wxT("January"), wxT("February"), wxT("March"), wxT("April"), wxT("May"), wxT("June"), wxT("Jule"), wxT("August"), wxT("September"), wxT("October"), wxT("November"), wxT("December") };

	yearlyMonthChoice = new wxComboBox(yearlyPatternPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 12, months, wxCB_READONLY);
	yearlyMonthChoice->SetSelection(0);
	horizontalSizer->Add(yearlyMonthChoice, 0, wxLEFT | wxRIGHT, 5);

	yearlyPatternPanel->SetSizer(horizontalSizer);
	yearlyPatternPanel->Layout();

	horizontalSizer->Fit(yearlyPatternPanel);
	
	patternSizer->Add(yearlyPatternPanel, 0, wxALIGN_TOP | wxALL, 0);

	mainPanel->SetSizer(panelSizer);
	mainPanel->Layout();

	panelSizer->Fit(mainPanel);

	mainSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	tagsPopup = new TagsPopup(this);
	tagsPopup->OnSelectTag = std::bind(&SchedulerDialog::OnSelectTag, this);

	okButton->Bind(wxEVT_BUTTON, &SchedulerDialog::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &SchedulerDialog::OnCancel, this);

	fromList->Bind(wxEVT_COMBOBOX, &SchedulerDialog::OnFromAccountSelect, this);
	toList->Bind(wxEVT_COMBOBOX, &SchedulerDialog::OnToAccountSelect, this);
	fromAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerDialog::OnFromAmountKillFocus, this);
	toAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerDialog::OnToAmountKillFocus, this);
	tagsField->Bind(wxEVT_KEY_UP, &SchedulerDialog::OnTextChanged, this);
	tagsField->Bind(wxEVT_KILL_FOCUS, &SchedulerDialog::OnTagsKillFocus, this);
	dailyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);
	weeklyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);
	monthlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);
	yearlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerDialog::OnPatternSelect, this);

	Bind(wxEVT_CHAR_HOOK, &SchedulerDialog::OnKeyDown, this);

	fromValue = 0;
	toValue = 0;

	/*for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Receipt))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Deposit))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Virtual))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Expens))
	{
		accounts.push_back(account);
	}

	for (auto account : DataHelper::GetInstance().GetAccountsByType(AccountType::Debt))
	{
		accounts.push_back(account);
	}*/

	UpdateFromList();
	SelectFromAccount(0);

	UpdateToList(fromAccount);
	SelectToAccount(0);
	
	SelectPatternType(SchedulerType::Daily);
	SelectWeekday(1);

	nameField->SetFocus();
}

SchedulerDialog::~SchedulerDialog() {	
	delete tagsPopup;
}

void SchedulerDialog::SetScheduler(std::shared_ptr<SchedulerModel> scheduler) {
	this->scheduler = scheduler;

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
}

void SchedulerDialog::UpdateFromList() {
	/*for (auto account : accounts) {
		if (account->type == Account::Type::Receipt || account->type == Account::Type::Deposit || account->type == Account::Type::Virtual) {
			int iconId = 0;

			if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
				iconId = account->iconId;
			}

			//fromList->Append(*account->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));

			fromAccounts.push_back(account);
		}
	}*/
}

void SchedulerDialog::UpdateToList(std::shared_ptr<AccountModel> account) {
	toList->Clear();
	toAccounts.clear();

	/*for (auto toAccount : accounts)
	{
		if (account->id == toAccount->id) {
			continue;
		}

		if (account->type == Account::Type::Receipt) {
			if (toAccount->type == Account::Type::Deposit) {
				int iconId = 0;

				if (toAccount->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
					iconId = toAccount->iconId;
				}

				//toList->Append(*toAccount->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));

				toAccounts.push_back(toAccount);
			}
		}
		else if (account->type == Account::Type::Deposit || account->type == Account::Type::Virtual) {
			if (toAccount->type == Account::Type::Deposit || toAccount->type == Account::Type::Expens
				|| toAccount->type == Account::Type::Debt ||	toAccount->type == Account::Type::Virtual) {
				int iconId = 0;

				if (toAccount->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
					iconId = toAccount->iconId;
				}

				toList->Append(toAccount->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));

				toAccounts.push_back(toAccount);
			}
		}
	}*/
}

void SchedulerDialog::SelectFromAccount(int index) {
	auto account = fromAccounts[index];

	fromList->Select(index);
	//fromAmountLabel->SetLabel(account->currency->shortName);
	fromAccount = account;
}

void SchedulerDialog::SelectToAccount(int id) {
	auto account = toAccounts[id];

	toList->Select(id);
	//toAmountLabel->SetLabel(*account->currency->shortName);

	toAccount = account;
}

void SchedulerDialog::SelectToAccount(std::shared_ptr<AccountModel> account) {
	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (toAccounts[i]->id == account->id) {
			SelectToAccount(i);
			return;
		}
	}

	SelectToAccount(0);
}

void SchedulerDialog::OnFromAccountSelect(wxCommandEvent &event) {
	SelectFromAccount(fromList->GetSelection());

	UpdateToList(fromAccount);
	SelectToAccount(toAccount);
}

void SchedulerDialog::OnToAccountSelect(wxCommandEvent &event) {
	SelectToAccount(toList->GetSelection());
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
}

void SchedulerDialog::OnCancel(wxCommandEvent &event) {
	Close();
}

void SchedulerDialog::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	/*wxString stringAmount = this->ClearAmountValue(fromAmountField->GetValue());
	fromAmountField->SetValue(stringAmount);

	int fromCurrencyId = fromAccounts[fromList->GetSelection()]->currency->id;
	int toCurrencyId = toAccounts[toList->GetSelection()]->currency->id;
	double val;

	toAmountField->GetValue().ToDouble(&val);

	if (val == 0 && fromCurrencyId == toCurrencyId) {
		toAmountField->SetValue(fromAmountField->GetValue());
	}*/
}

void SchedulerDialog::OnToAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	wxString stringAmount = this->ClearAmountValue(toAmountField->GetValue());
	toAmountField->SetValue(stringAmount);
}

void SchedulerDialog::OnTextChanged(wxKeyEvent &event) {
	if (event.GetKeyCode() == WXK_ESCAPE) {
		tagsPopup->Hide();
	}
	else if (event.GetKeyCode() == WXK_UP) {
		tagsPopup->SelectPrev();
		event.StopPropagation();
	}
	else if (event.GetKeyCode() == WXK_DOWN) {
		tagsPopup->SelectNext();
	}
	else if (event.GetKeyCode() == WXK_RETURN) {
		AddTag();
		tagsPopup->Hide();
	}
	else {
		wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
		vector<wxString> tokens;

		while (tokenizer.HasMoreTokens()) {
			wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
			tokens.push_back(token);
		}

		if (!tokens.empty()) {
			//TODO
			auto tags = std::vector<std::shared_ptr<wxString>>(); //DataHelper::GetInstance().GetTagsBySearch(tokens.back());

			if (!tokens.empty()) {
				tagsPopup->Update(tags);

				wxPoint pos = tagsField->GetScreenPosition();
				wxSize size = tagsField->GetSize();

				tagsPopup->Position(wxPoint(pos.x - 200, pos.y - 200 + size.GetHeight()), wxSize(200, 200));
				tagsPopup->Show();
			}
			else {
				tagsPopup->Hide();
			}
		}
	}

	event.Skip();
}

void SchedulerDialog::OnTagsKillFocus(wxFocusEvent &event) {
	tagsPopup->Hide();
	event.Skip();
}

void SchedulerDialog::OnSelectTag() {
	AddTag();
	tagsPopup->Hide();
}

void SchedulerDialog::AddTag() {
	wxString tag = tagsPopup->GetSelectedTag();
	wxString result = "";

	wxStringTokenizer tokenizer(tagsField->GetValue(), ",");
	vector<wxString> tokens;

	while (tokenizer.HasMoreTokens()) {
		wxString token = tokenizer.GetNextToken().Trim(true).Trim(false);
		tokens.push_back(token);
	}

	for (unsigned int i = 0; i < tokens.size() - 1; i++) {
		result.Append(tokens[i]);
		result.Append(", ");
	}

	result.Append(tag);

	tagsField->SetValue(result);
	tagsField->SetInsertionPointEnd();
}

wxString SchedulerDialog::ClearAmountValue(wxString &value) {
	value.Trim(true);
	value.Trim(false);
	value.Replace(",", ".", true);
	value.Replace(" ", "", true);

	return value;
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
	this->type = type;

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