#include "ReportExpensesForPeriodPanel.h"

ReportExpensesForPeriodPanel::ReportExpensesForPeriodPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st4 = new wxStaticText(filterPanel, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("3 Months"));
	values->Add(wxT("6 Months"));
	values->Add(wxT("This Year"));
	values->Add(wxT("Previous Year"));
	values->Add(wxT("Custom"));

	periodList = new wxComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(120, 20), *values, wxCB_DROPDOWN | wxCB_READONLY);
	delete values;

	wxStaticText *fromLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("From"), wxDefaultPosition, wxDefaultSize, 0);
	fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);

	wxStaticText *toLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("To"), wxDefaultPosition, wxDefaultSize, 0);
	toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);

	filterSizer->Add(st4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(periodList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	filterSizer->Fit(filterPanel);
	mainSizer->Add(filterPanel, 0, wxEXPAND | wxALL, 5);

	wxPanel *reportPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *reportSizer = new wxBoxSizer(wxVERTICAL);

	chart = new BarChart(reportPanel, wxID_ANY);
	
	reportSizer->Add(chart, 1, wxEXPAND | wxALL, 40);

	reportPanel->SetSizer(reportSizer);
	reportPanel->Layout();

	reportSizer->Fit(reportPanel);
	mainSizer->Add(reportPanel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(mainSizer);
	this->Layout();

	this->Centre(wxBOTH);

	periodList->Bind(wxEVT_COMBOBOX, &ReportExpensesForPeriodPanel::OnPeriodSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesForPeriodPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesForPeriodPanel::OnDateChanged, this);

	periodList->Select(3);

	RestoreFilterSettings();
}

ReportExpensesForPeriodPanel::~ReportExpensesForPeriodPanel() {
	SaveFilterSettings();
}

void ReportExpensesForPeriodPanel::Update() {
	wxDateTime fromDate = fromDatePicker->GetValue();
	wxDateTime toDate = toDatePicker->GetValue();

	fromDate.SetDay(1);

	vector<StringValue> values = DataHelper::GetInstance().GetExpensesByAccount(&fromDate, &toDate);

	std::vector<StringValue> chartValues;

	for (auto value : values)
	{
		StringValue chartValue = { value.string, value.value };
		chartValues.push_back(chartValue);
	}

	chart->SetValues(chartValues);
}

void ReportExpensesForPeriodPanel::OnDateChanged(wxDateEvent &event) {
	periodFromDate = fromDatePicker->GetValue();
	periodToDate = toDatePicker->GetValue();

	SaveFilterSettings();
	Update();
}

void ReportExpensesForPeriodPanel::OnPeriodSelect(wxCommandEvent &event) {
	SaveFilterSettings();

	CalculatePeriod();
	Update();
}

void ReportExpensesForPeriodPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(3);

	periodList->SetSelection(settings.period);

	periodFromDate = settings.fromDate;
	periodToDate = settings.toDate;

	CalculatePeriod();
}

void ReportExpensesForPeriodPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(3, -1, periodList->GetSelection(), fromDatePicker->GetValue(), toDatePicker->GetValue());
}

void ReportExpensesForPeriodPanel::CalculatePeriod() {
	int index = periodList->GetSelection();

	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDatePicker->Disable();
	toDatePicker->Disable();

	switch (index)
	{
	case 0:
		fromDate.Subtract(wxDateSpan::wxDateSpan(0, 3, 0, 0));
		fromDate.SetDay(1);
		break;

	case 1:
		fromDate.Subtract(wxDateSpan::wxDateSpan(0, 6, 0, 0));
		fromDate.SetDay(1);
		break;

	case 2:
		fromDate.SetMonth(wxDateTime::Month::Jan);
		fromDate.SetDay(1);
		break;

	case 3:
		fromDate.Subtract(wxDateSpan::wxDateSpan(1, 0, 0, 0));
		fromDate.SetMonth(wxDateTime::Month::Jan);
		fromDate.SetDay(1);

		toDate.Subtract(wxDateSpan::wxDateSpan(1, 0, 0, 0));
		toDate.SetMonth(wxDateTime::Month::Dec);
		toDate.SetToLastMonthDay(wxDateTime::Month::Dec);
		break;

	case 4:
		fromDate = periodFromDate;
		toDate = periodToDate;

		fromDatePicker->Enable();
		toDatePicker->Enable();
		break;

	default:
		break;
	}

	fromDatePicker->SetValue(fromDate);
	toDatePicker->SetValue(toDate);
}