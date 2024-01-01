#include "ReportExpensesForPeriodPanel.h"

ReportExpensesForPeriodPanel::ReportExpensesForPeriodPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);

	_periodFilterPanel = new PeriodFilterPanel(this, PeriodFilterType::Report);
	_periodFilterPanel->OnChange = [&]() {
		Update();
	};

	filterSizer->Add(_periodFilterPanel, 1, wxALIGN_CENTER_VERTICAL);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	filterSizer->Fit(filterPanel);
	mainSizer->Add(filterPanel, 0, wxEXPAND | wxALL, 5);

	wxPanel *reportPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer *reportSizer = new wxBoxSizer(wxVERTICAL);

	_chart = new BarChart(reportPanel, wxID_ANY);
	
	reportSizer->Add(_chart, 1, wxEXPAND | wxALL, 40);

	reportPanel->SetSizer(reportSizer);
	reportPanel->Layout();

	reportSizer->Fit(reportPanel);
	mainSizer->Add(reportPanel, 1, wxEXPAND | wxALL, 0);

	SetSizer(mainSizer);
	Layout();

	RestoreFilterSettings();
}

ReportExpensesForPeriodPanel::~ReportExpensesForPeriodPanel() {
	SaveFilterSettings();
}

void ReportExpensesForPeriodPanel::Update() {
	std::vector<StringValueViewModel> values = _context.GetReportingService().GetExpensesByAccount(_periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());
	_chart->SetValues(values);
}

void ReportExpensesForPeriodPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(3);

	_periodFilterPanel->SetFromDate(settings.fromDate);
	_periodFilterPanel->SetToDate(settings.toDate);
	_periodFilterPanel->SetPeriod(settings.period);
}

void ReportExpensesForPeriodPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(3, "", _periodFilterPanel->GetPeriod(), _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());
}