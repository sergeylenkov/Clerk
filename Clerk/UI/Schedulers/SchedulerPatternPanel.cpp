#include "SchedulerPatternPanel.h"

SchedulerPatternPanel::SchedulerPatternPanel(wxWindow* parent, const wxPoint& position, const wxSize& size):
	wxPanel(parent, wxID_ANY, position, size)
{
	int indent = FromDIP(5);
	wxSize labelSize = FromDIP(wxSize(40, -1));

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

	wxBoxSizer* mainSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Recurrence pattern")), wxHORIZONTAL);
	wxBoxSizer* buttonsSizer = new wxBoxSizer(wxVERTICAL);

	_dailyButton = new wxRadioButton(this, wxID_ANY, _("Daily"));
	buttonsSizer->Add(_dailyButton, 0, wxBOTTOM, indent);

	_weeklyButton = new wxRadioButton(this, wxID_ANY, _("Weekly"));
	buttonsSizer->Add(_weeklyButton, 0, wxBOTTOM, indent);

	_monthlyButton = new wxRadioButton(this, wxID_ANY, _("Monthly"));
	buttonsSizer->Add(_monthlyButton, 0, wxBOTTOM, indent);

	_yearlyButton = new wxRadioButton(this, wxID_ANY, _("Yearly"));
	buttonsSizer->Add(_yearlyButton, 0, wxBOTTOM, indent);

	mainSizer->Add(buttonsSizer, 0, wxALL, indent);

	wxStaticLine* staticLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	mainSizer->Add(staticLine, 0, wxEXPAND | wxLEFT | wxRIGHT, indent);

	wxPanel* patternPanel = new wxPanel(this, wxID_ANY);
	mainSizer->Add(patternPanel, 1, wxEXPAND);

	wxBoxSizer* patternSizer = new wxBoxSizer(wxVERTICAL);

	patternPanel->SetSizer(patternSizer);
	patternPanel->Layout();

	patternSizer->Fit(patternPanel);

	// Daily

	_dailyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* dailyPatternLabel = new wxStaticText(_dailyPatternPanel, wxID_ANY, _("Every"));
	horizontalSizer->Add(dailyPatternLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	_dailyDayField = new wxTextCtrl(_dailyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, daysValidator);
	horizontalSizer->Add(_dailyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText* daysLabel = new wxStaticText(_dailyPatternPanel, wxID_ANY, _("days"));
	horizontalSizer->Add(daysLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	_dailyPatternPanel->SetSizer(horizontalSizer);
	_dailyPatternPanel->Layout();

	horizontalSizer->Fit(_dailyPatternPanel);

	patternSizer->Add(_dailyPatternPanel, 0, wxALIGN_TOP);

	// Weekly

	_weeklyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* m_staticText1011 = new wxStaticText(_weeklyPatternPanel, wxID_ANY, _("Every"));
	horizontalSizer->Add(m_staticText1011, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	_weeklyWeekField = new wxTextCtrl(_weeklyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, weekValidator);
	horizontalSizer->Add(_weeklyWeekField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText* m_staticText911 = new wxStaticText(_weeklyPatternPanel, wxID_ANY, _("weeks on:"));
	horizontalSizer->Add(m_staticText911, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	verticalSizer->Add(horizontalSizer, 0, wxALIGN_TOP | wxBOTTOM, 5);

	wxWrapSizer* wrapSizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

	_mondayCheckBox = new wxRadioButton(_weeklyPatternPanel, wxID_ANY, _("Monday"));
	wrapSizer->Add(_mondayCheckBox, 0, wxALL, 5);

	_tuesdayCheckBox = new wxRadioButton(_weeklyPatternPanel, wxID_ANY, _("Tuesday"));
	wrapSizer->Add(_tuesdayCheckBox, 0, wxALL, 5);

	_wednesdayCheckBox = new wxRadioButton(_weeklyPatternPanel, wxID_ANY, _("Wednesday"));
	wrapSizer->Add(_wednesdayCheckBox, 0, wxALL, 5);

	_thursdayCheckBox = new wxRadioButton(_weeklyPatternPanel, wxID_ANY, _("Thursday"));
	wrapSizer->Add(_thursdayCheckBox, 0, wxALL, 5);

	_fridayCheckBox = new wxRadioButton(_weeklyPatternPanel, wxID_ANY, _("Friday"));
	wrapSizer->Add(_fridayCheckBox, 0, wxALL, 5);

	_saturdayCheckBox = new wxRadioButton(_weeklyPatternPanel, wxID_ANY, _("Saturday"));
	wrapSizer->Add(_saturdayCheckBox, 0, wxALL, 5);

	_sundayCheckBox = new wxRadioButton(_weeklyPatternPanel, wxID_ANY, _("Sunday"));
	wrapSizer->Add(_sundayCheckBox, 0, wxALL, 5);

	verticalSizer->Add(wrapSizer, 1, wxEXPAND);

	_weeklyPatternPanel->SetSizer(verticalSizer);
	_weeklyPatternPanel->Layout();

	wrapSizer->Fit(_weeklyPatternPanel);

	patternSizer->Add(_weeklyPatternPanel, 1, wxEXPAND);

	// Monthly

	_monthlyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* m_staticText912 = new wxStaticText(_monthlyPatternPanel, wxID_ANY, _("Day"));
	horizontalSizer->Add(m_staticText912, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	_monthlyDayField = new wxTextCtrl(_monthlyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, dayValidator);
	horizontalSizer->Add(_monthlyDayField, 0, wxLEFT | wxRIGHT, 5);

	wxStaticText* m_staticText1012 = new wxStaticText(_monthlyPatternPanel, wxID_ANY, _("every"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText1012, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	_monthlyMonthField = new wxTextCtrl(_monthlyPatternPanel, wxID_ANY, "1", wxDefaultPosition, wxSize(40, -1), wxTE_RIGHT, monthValidator);
	horizontalSizer->Add(_monthlyMonthField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxStaticText* m_staticText10121 = new wxStaticText(_monthlyPatternPanel, wxID_ANY, _("month(s)"), wxDefaultPosition, wxDefaultSize, 0);
	horizontalSizer->Add(m_staticText10121, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	_monthlyPatternPanel->SetSizer(horizontalSizer);
	_monthlyPatternPanel->Layout();

	horizontalSizer->Fit(_monthlyPatternPanel);

	patternSizer->Add(_monthlyPatternPanel, 0, wxALIGN_TOP);

	// Yearly

	_yearlyPatternPanel = new wxPanel(patternPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* everyLabel = new wxStaticText(_yearlyPatternPanel, wxID_ANY, _("Every"));
	horizontalSizer->Add(everyLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	_yearlyDayField = new wxTextCtrl(_yearlyPatternPanel, wxID_ANY, "1", wxDefaultPosition, labelSize, wxTE_RIGHT, dayValidator);
	horizontalSizer->Add(_yearlyDayField, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxString months[] = { _("January"), _("February"), _("March"), _("April"), _("May"), _("June"), _("Jule"), _("August"), _("September"), _("October"), _("November"), _("December") };

	_yearlyMonthChoice = new wxComboBox(_yearlyPatternPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 12, months, wxCB_READONLY);
	_yearlyMonthChoice->SetSelection(0);
	horizontalSizer->Add(_yearlyMonthChoice, 0, wxLEFT | wxRIGHT, 5);

	_yearlyPatternPanel->SetSizer(horizontalSizer);
	_yearlyPatternPanel->Layout();

	horizontalSizer->Fit(_yearlyPatternPanel);

	patternSizer->Add(_yearlyPatternPanel, 0, wxALIGN_TOP);

	SetSizer(mainSizer);
	Layout();

	_dailyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerPatternPanel::OnPatternSelect, this);
	_weeklyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerPatternPanel::OnPatternSelect, this);
	_monthlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerPatternPanel::OnPatternSelect, this);
	_yearlyButton->Bind(wxEVT_RADIOBUTTON, &SchedulerPatternPanel::OnPatternSelect, this);
}

void SchedulerPatternPanel::SetViewModel(SchedulerViewModel* viewModel) {
	_viewModel = viewModel;

	_viewModel->OnUpdate = [&](SchedulerViewModelField field) {
		if (field == SchedulerViewModelField::Type) {
			UpdatePatternType(_viewModel->GetType());
		}
	};

	Update();
}

void SchedulerPatternPanel::Update() {
	UpdatePatternType(_viewModel->GetType());
	SelectWeekday(_viewModel->GetDay());

	_dailyDayField->SetValue(wxString::Format("%d", _viewModel->GetDay()));
	_weeklyWeekField->SetValue(wxString::Format("%d", _viewModel->GetWeek()));
	_monthlyDayField->SetValue(wxString::Format("%d", _viewModel->GetDay()));
	_monthlyMonthField->SetValue(wxString::Format("%d", _viewModel->GetMonth()));
	_yearlyDayField->SetValue(wxString::Format("%d", _viewModel->GetDay()));
	_yearlyMonthChoice->SetSelection(_viewModel->GetMonth());
}

void SchedulerPatternPanel::UpdatePatternType(SchedulerType type) {
	_dailyPatternPanel->Hide();
	_weeklyPatternPanel->Hide();
	_monthlyPatternPanel->Hide();
	_yearlyPatternPanel->Hide();

	if (type == SchedulerType::Daily) {
		_dailyButton->SetValue(true);
		_dailyPatternPanel->Show();
	}

	if (type == SchedulerType::Weekly) {
		_weeklyButton->SetValue(true);
		_weeklyPatternPanel->Show();
	}

	if (type == SchedulerType::Monthly) {
		_monthlyButton->SetValue(true);
		_monthlyPatternPanel->Show();
	}

	if (type == SchedulerType::Yearly) {
		_yearlyButton->SetValue(true);
		_yearlyPatternPanel->Show();
	}

	Layout();
}

void SchedulerPatternPanel::SelectWeekday(int day) {
	_mondayCheckBox->SetValue(false);
	_tuesdayCheckBox->SetValue(false);
	_wednesdayCheckBox->SetValue(false);
	_thursdayCheckBox->SetValue(false);
	_fridayCheckBox->SetValue(false);
	_saturdayCheckBox->SetValue(false);
	_sundayCheckBox->SetValue(false);

	switch (day)
	{
	case 1:
		_mondayCheckBox->SetValue(true);
		break;

	case 2:
		_tuesdayCheckBox->SetValue(true);
		break;

	case 3:
		_wednesdayCheckBox->SetValue(true);
		break;

	case 4:
		_thursdayCheckBox->SetValue(true);
		break;

	case 5:
		_fridayCheckBox->SetValue(true);
		break;

	case 6:
		_saturdayCheckBox->SetValue(true);
		break;

	case 7:
		_sundayCheckBox->SetValue(true);
		break;

	default:
		_mondayCheckBox->SetValue(true);
		break;
	}
}

void SchedulerPatternPanel::OnPatternSelect(wxCommandEvent& event) {
	if (_dailyButton->GetValue()) {
		_viewModel->SetType(SchedulerType::Daily);
	}

	if (_weeklyButton->GetValue()) {
		_viewModel->SetType(SchedulerType::Weekly);
	}

	if (_monthlyButton->GetValue()) {
		_viewModel->SetType(SchedulerType::Monthly);
	}

	if (_yearlyButton->GetValue()) {
		_viewModel->SetType(SchedulerType::Yearly);
	}
}