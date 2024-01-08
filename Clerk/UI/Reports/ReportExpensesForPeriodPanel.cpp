#include "ReportExpensesForPeriodPanel.h"

ReportExpensesForPeriodPanel::ReportExpensesForPeriodPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	SetBackgroundColour(*wxWHITE);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);

	_periodFilterPanel = new PeriodFilterPanel(this, PeriodFilterType::Report);
	_periodFilterPanel->OnChange = [&]() {
		Update();
	};

	filterSizer->Add(_periodFilterPanel, 1, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(filterSizer, 0, wxEXPAND | wxALL, FromDIP(10));

	_chart = new BarChart(this);
	
	_chart->SetMinSize(FromDIP(wxSize(-1, 600)));
	_chart->SetMaxSize(FromDIP(wxSize(-1, 600)));

	wxBoxSizer* chartSizer = new wxBoxSizer(wxHORIZONTAL);

	chartSizer->Add(_chart, 1, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(chartSizer, 1, wxEXPAND | wxALL, FromDIP(10));

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
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(static_cast<int>(ReportType::ExpensesForPeriod));

	_periodFilterPanel->SetFromDate(settings.fromDate);
	_periodFilterPanel->SetToDate(settings.toDate);
	_periodFilterPanel->SetPeriod(settings.period);
}

void ReportExpensesForPeriodPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(static_cast<int>(ReportType::ExpensesForPeriod), "", _periodFilterPanel->GetPeriod(), _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());
}