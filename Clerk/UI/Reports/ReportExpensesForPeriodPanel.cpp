#include "ReportExpensesForPeriodPanel.h"

ReportExpensesForPeriodPanel::ReportExpensesForPeriodPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *fromLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("From"), wxDefaultPosition, wxDefaultSize, 0);
	filterSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
	filterSizer->Add(fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	wxStaticText *toLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("To"), wxDefaultPosition, wxDefaultSize, 0);
	filterSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN);
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

	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesForPeriodPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesForPeriodPanel::OnDateChanged, this);

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
	Update();
}

void ReportExpensesForPeriodPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(3);

	fromDatePicker->SetValue(settings.fromDate);
	toDatePicker->SetValue(settings.toDate);
}

void ReportExpensesForPeriodPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(3, -1, 0, fromDatePicker->GetValue(), toDatePicker->GetValue());
}