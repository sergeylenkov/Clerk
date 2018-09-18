#include "SchedulerFrame.h"

SchedulerFrame::SchedulerFrame(wxFrame *parent, const wxChar *title, int x, int y, int width, int height) : wxFrame(parent, -1, title, wxPoint(x, y), wxSize(width, height), wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)) {
	SetBackgroundColour(wxColor(*wxWHITE));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

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

	wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	nameLabel = new wxStaticText(this, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(nameLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	nameField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer *staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Recurrence pattern")), wxHORIZONTAL);

	wxPanel *buttonsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(80, -1), wxTAB_TRAVERSAL);

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

	wxStaticLine *staticLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	staticBoxSizer->Add(staticLine, 0, wxEXPAND | wxALL, 5);	

	patternPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	staticBoxSizer->Add(patternPanel, 1, wxEXPAND | wxALL, 5);

	patternSizer = new wxBoxSizer(wxVERTICAL);

	patternPanel->SetSizer(patternSizer);
	patternPanel->Layout();

	patternSizer->Fit(patternPanel);

	mainSizer->Add(staticBoxSizer, 1, wxEXPAND | wxALL, 10);

	staticBoxSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Transaction")), wxVERTICAL);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	fromLabel = new wxStaticText(this, wxID_ANY, wxT("From:"), wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromList = new wxBitmapComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(fromList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountField = new wxTextCtrl(this, wxID_ANY, wxT("1000"), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	horizontalSizer->Add(fromAmountField, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromAmountLabel = new wxStaticText(this, wxID_ANY, wxT("RUB"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(fromAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	toLabel = new wxStaticText(this, wxID_ANY, wxT("To:"), wxDefaultPosition, wxSize(40, -1), 0);
	toLabel->Wrap(-1);
	horizontalSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toList = new wxBitmapComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
	horizontalSizer->Add(toList, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toAmountField = new wxTextCtrl(this, wxID_ANY, wxT("1000"), wxDefaultPosition, wxSize(80, -1), wxTE_RIGHT, amountValidator);
	horizontalSizer->Add(toAmountField, 0, wxALL, 5);

	toAmountLabel = new wxStaticText(this, wxID_ANY, wxT("RUB"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(toAmountLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	tagsLabel = new wxStaticText(this, wxID_ANY, wxT("Tags:"), wxDefaultPosition, wxSize(40, -1), 0);
	horizontalSizer->Add(tagsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	tagsField = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(tagsField, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	staticBoxSizer->Add(horizontalSizer, 0, wxALL | wxEXPAND, 5);

	mainSizer->Add(staticBoxSizer, 1, wxEXPAND | wxALL, 10);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	okButton = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(okButton, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	cancelButton = new wxButton(this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(horizontalSizer, 0, wxALIGN_RIGHT | wxALL, 5);

	//

	dailyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *m_staticText91 = new wxStaticText(dailyPatternPanel, wxID_ANY, wxT("Every"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText91, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	dailyDayField = new wxTextCtrl(dailyPatternPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, daysValidator);
	horizontalSizer->Add(dailyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText101 = new wxStaticText(dailyPatternPanel, wxID_ANY, wxT("days"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText101, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

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

	weeklyDayField = new wxTextCtrl(weeklyPatternPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, weekValidator);
	horizontalSizer->Add(weeklyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText *m_staticText911 = new wxStaticText(weeklyPatternPanel, wxID_ANY, wxT("weeks on:"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText911, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	verticalSizer->Add(horizontalSizer, 0, wxALIGN_TOP | wxBOTTOM, 5);

	wxWrapSizer *wrapSizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

	mondayCheckBox = new wxCheckBox(weeklyPatternPanel, wxID_ANY, wxT("Monday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(mondayCheckBox, 0, wxALL, 5);

	tuesdayCheckBox = new wxCheckBox(weeklyPatternPanel, wxID_ANY, wxT("Tuesday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(tuesdayCheckBox, 0, wxALL, 5);

	wednesdayCheckBox = new wxCheckBox(weeklyPatternPanel, wxID_ANY, wxT("Wednesday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(wednesdayCheckBox, 0, wxALL, 5);

	thursdayCheckBox = new wxCheckBox(weeklyPatternPanel, wxID_ANY, wxT("Thursday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(thursdayCheckBox, 0, wxALL, 5);

	fridayCheckBox = new wxCheckBox(weeklyPatternPanel, wxID_ANY, wxT("Friday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(fridayCheckBox, 0, wxALL, 5);

	saturdayCheckBox = new wxCheckBox(weeklyPatternPanel, wxID_ANY, wxT("Saturday"), wxDefaultPosition, wxDefaultSize, 0);
	wrapSizer->Add(saturdayCheckBox, 0, wxALL, 5);

	sundayCheckBox = new wxCheckBox(weeklyPatternPanel, wxID_ANY, wxT("Sunday"), wxDefaultPosition, wxDefaultSize, 0);
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

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	tagsPopup = new TagsPopup(this);
	tagsPopup->OnSelectTag = std::bind(&SchedulerFrame::OnSelectTag, this);

	okButton->Bind(wxEVT_BUTTON, &SchedulerFrame::OnOK, this);
	cancelButton->Bind(wxEVT_BUTTON, &SchedulerFrame::OnCancel, this);

	fromList->Bind(wxEVT_COMBOBOX, &SchedulerFrame::OnFromAccountSelect, this);
	toList->Bind(wxEVT_COMBOBOX, &SchedulerFrame::OnToAccountSelect, this);
	fromAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerFrame::OnFromAmountKillFocus, this);
	toAmountField->Bind(wxEVT_KILL_FOCUS, &SchedulerFrame::OnToAmountKillFocus, this);
	tagsField->Bind(wxEVT_KEY_UP, &SchedulerFrame::OnTextChanged, this);
	tagsField->Bind(wxEVT_KILL_FOCUS, &SchedulerFrame::OnTagsKillFocus, this);
	dailyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerFrame::OnPatternSelect, this);
	weeklyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerFrame::OnPatternSelect, this);
	monthlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerFrame::OnPatternSelect, this);
	yearlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerFrame::OnPatternSelect, this);

	fromValue = 0;
	toValue = 0;

	dailyButton->SetValue(true);
	dailyPatternPanel->Show();
	weeklyPatternPanel->Hide();
	monthlyPatternPanel->Hide();
	yearlyPatternPanel->Hide();

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Receipt))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Deposit))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Expens))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Debt))
	{
		accounts.push_back(account);
	}

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Credit))
	{
		accounts.push_back(account);
	}

	accountsImageList = new wxImageList(16, 16, false);

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		wxImage image(path);

		if (image.IsOk())
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}

	UpdateFromList();
	SelectFromAccount(0);

	UpdateToList(fromAccount);
	SelectToAccount(0);
}

SchedulerFrame::~SchedulerFrame() {	
	delete accountsImageList;
	delete tagsPopup;
}

void SchedulerFrame::SetScheduler(std::shared_ptr<Scheduler> scheduler) {
	this->scheduler = scheduler;

	/*fromAmountField->SetValue(wxString::Format("%.2f", transaction->fromAmount));
	toAmountField->SetValue(wxString::Format("%.2f", transaction->toAmount));
	tagsField->SetValue(*transaction->tags);
	noteField->SetValue(*transaction->note);	

	for (unsigned int i = 0; i < fromAccounts.size(); i++) {
		if (transaction->fromAccountId == fromAccounts[i]->id) {
			SelectFromAccount(i);
			UpdateToList(fromAccounts[i]);

			break;
		}
	}

	if (transaction->toAccountId != -1) {
		for (unsigned int i = 0; i < toAccounts.size(); i++) {
			if (transaction->toAccountId == toAccounts[i]->id) {
				SelectToAccount(i);
				break;
			}
		}
	}
	else {
		SelectToAccount(0);
	}*/

	fromAmountField->SetFocus();
	fromAmountField->SelectAll();
}

void SchedulerFrame::UpdateFromList() {
	for each (auto account in accounts) {
		if (account->type == AccountTypes::Receipt || account->type == AccountTypes::Deposit) {
			int icon = 0;

			if (account->iconId < accountsImageList->GetImageCount()) {
				icon = account->iconId;
			}

			fromList->Append(*account->name, accountsImageList->GetBitmap(icon));

			fromAccounts.push_back(account);
		}
	}
}

void SchedulerFrame::UpdateToList(std::shared_ptr<Account> account) {
	toList->Clear();
	toAccounts.clear();

	for each (auto toAccount in accounts)
	{
		if (account->id == toAccount->id) {
			continue;
		}

		if (account->type == AccountTypes::Receipt) {
			if (toAccount->type == AccountTypes::Deposit) {
				int icon = 0;

				if (toAccount->iconId < accountsImageList->GetImageCount()) {
					icon = toAccount->iconId;
				}

				toList->Append(*toAccount->name, accountsImageList->GetBitmap(icon));

				toAccounts.push_back(toAccount);
			}
		}
		else if (account->type == AccountTypes::Deposit) {
			if (toAccount->type == AccountTypes::Deposit || toAccount->type == AccountTypes::Expens || toAccount->type == AccountTypes::Debt || toAccount->type == AccountTypes::Credit) {
				int icon = 0;

				if (toAccount->iconId < accountsImageList->GetImageCount()) {
					icon = toAccount->iconId;
				}

				toList->Append(*toAccount->name, accountsImageList->GetBitmap(icon));

				toAccounts.push_back(toAccount);
			}
		}
	}
}

void SchedulerFrame::SelectFromAccount(int index) {
	auto account = fromAccounts[index];

	fromList->Select(index);
	fromAmountLabel->SetLabel(*account->currency->shortName);
	fromAccount = account;
}

void SchedulerFrame::SelectToAccount(int id) {
	auto account = toAccounts[id];

	toList->Select(id);
	toAmountLabel->SetLabel(*account->currency->shortName);

	toAccount = account;
}

void SchedulerFrame::SelectToAccount(std::shared_ptr<Account> account) {
	for (unsigned int i = 0; i < toAccounts.size(); i++) {
		if (toAccounts[i]->id == account->id) {
			SelectToAccount(i);
			return;
		}
	}

	SelectToAccount(0);
}

void SchedulerFrame::OnFromAccountSelect(wxCommandEvent &event) {
	SelectFromAccount(fromList->GetSelection());

	UpdateToList(fromAccount);
	SelectToAccount(toAccount);
}

void SchedulerFrame::OnToAccountSelect(wxCommandEvent &event) {
	SelectToAccount(toList->GetSelection());
}

void SchedulerFrame::OnOK(wxCommandEvent &event) {
	/*double val;

	wxString value = fromAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&val);

	fromValue = val;

	value = toAmountField->GetValue();
	value.Replace(" ", "");
	value.Replace(",", ".");
	value.ToDouble(&val);

	toValue = val;

	transaction->fromAccountId = fromAccounts[fromList->GetSelection()]->id;
	transaction->toAccountId = toAccounts[toList->GetSelection()]->id;
	transaction->fromAmount = fromValue;
	transaction->toAmount = toValue;
	transaction->note = make_shared<wxString>(noteField->GetValue());
	transaction->tags = make_shared<wxString>(tagsField->GetValue());

	transaction->Save();*/

	Close();

	if (OnClose) {
		OnClose();
	}
}

void SchedulerFrame::OnCancel(wxCommandEvent &event) {
	Close();
}

void SchedulerFrame::OnFromAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	wxString stringAmount = this->ClearAmountValue(fromAmountField->GetValue());
	fromAmountField->SetValue(stringAmount);

	int fromCurrencyId = fromAccounts[fromList->GetSelection()]->currency->id;
	int toCurrencyId = toAccounts[toList->GetSelection()]->currency->id;
	double val;

	toAmountField->GetValue().ToDouble(&val);

	if (val == 0 && fromCurrencyId == toCurrencyId) {
		toAmountField->SetValue(fromAmountField->GetValue());
	}
}

void SchedulerFrame::OnToAmountKillFocus(wxFocusEvent &event) {
	event.Skip();

	wxString stringAmount = this->ClearAmountValue(toAmountField->GetValue());
	toAmountField->SetValue(stringAmount);
}

void SchedulerFrame::OnTextChanged(wxKeyEvent &event) {
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
			auto tags = DataHelper::GetInstance().GetTagsBySearch(tokens.back());

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

void SchedulerFrame::OnTagsKillFocus(wxFocusEvent &event) {
	tagsPopup->Hide();
	event.Skip();
}

void SchedulerFrame::OnSelectTag() {
	AddTag();
	tagsPopup->Hide();
}

void SchedulerFrame::AddTag() {
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

wxString SchedulerFrame::ClearAmountValue(wxString &value) {
	value.Trim(true);
	value.Trim(false);
	value.Replace(",", ".", true);
	value.Replace(" ", "", true);

	return value;
}

void SchedulerFrame::OnPatternSelect(wxCommandEvent &event) {
	dailyPatternPanel->Hide();
	weeklyPatternPanel->Hide();
	monthlyPatternPanel->Hide();
	yearlyPatternPanel->Hide();

	if (dailyButton->GetValue()) {
		dailyPatternPanel->Show();
	}

	if (weeklyButton->GetValue()) {
		weeklyPatternPanel->Show();
	}

	if (monthlyButton->GetValue()) {
		monthlyPatternPanel->Show();
	}

	if (yearlyButton->GetValue()) {
		yearlyPatternPanel->Show();
	}

	patternPanel->Layout();
}