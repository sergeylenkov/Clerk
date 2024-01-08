#include "ReportExpensesByMonthPanel.h"

ReportExpensesByMonthPanel::ReportExpensesByMonthPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	SetBackgroundColour(*wxWHITE);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);	

	wxStaticText *accountsLabel = new wxStaticText(this, wxID_ANY, _("Accounts:"));
	filterSizer->Add(accountsLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	_accountsComboBox = new AccountsComboBox(this, wxDefaultPosition, FromDIP(wxSize(200, 20)), true);
	_accountsComboBox->OnChange = [&](std::set<int> ids) {
		_selectedIds = ids;
		Update();
	};

	filterSizer->Add(_accountsComboBox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	_periodFilterPanel = new PeriodFilterPanel(this, PeriodFilterType::Report);
	_periodFilterPanel->OnChange = [&]() {
		Update();
	};

	filterSizer->Add(_periodFilterPanel, 1, wxALIGN_CENTER_VERTICAL);
	filterSizer->Add(0, 0, 1, wxEXPAND);

	_averageCheckbox = new wxCheckBox(this, wxID_ANY, _("Show average"));		
	
	filterSizer->Add(_averageCheckbox, 0, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(filterSizer, 0, wxEXPAND | wxALL, FromDIP(10));

	_chart = new LineChart(this);

	_chart->SetMinSize(FromDIP(wxSize(-1, 600)));
	_chart->SetMaxSize(FromDIP(wxSize(-1, 600)));

	wxBoxSizer *chartSizer = new wxBoxSizer(wxHORIZONTAL);

	chartSizer->Add(_chart, 1, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(chartSizer, 1, wxEXPAND | wxALL, FromDIP(10));

	SetSizer(mainSizer);

	_chartPopup = new ExpensesTooltipPopup(this);

	_averageCheckbox->Bind(wxEVT_CHECKBOX, &ReportExpensesByMonthPanel::OnDrawAverageCheck, this);

	_chart->OnShowPopup = [&]() {
		ShowPopup();
	};

	_chart->OnHidePopup = [&]() {
		HidePopup();
	};

	_chart->OnUpdatePopup = [&](int x, int y, int index) {
		UpdatePopup(x, y, index);
	};

	_selectedIds = {};

	_accounts = _context.GetAccountsService().GetExpenses();

	std::sort(_accounts.begin(), _accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_accountsComboBox->SetAccounts(_accounts);

	RestoreFilterSettings();
}

ReportExpensesByMonthPanel::~ReportExpensesByMonthPanel() {
	SaveFilterSettings();
}

void ReportExpensesByMonthPanel::Update() {	
	_values = _context.GetReportingService().GetExpensesByMonth(_selectedIds, _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());
	std::vector<StringValueViewModel> chartValues;

	for (auto &value : _values)
	{
		StringValueViewModel chartValue = { value.date.Format("%B"), value.value };
		chartValues.push_back(chartValue);
	}

	_chart->SetDrawAverage(_averageCheckbox->IsChecked());
	_chart->SetValues(chartValues);
}

void ReportExpensesByMonthPanel::OnDrawAverageCheck(wxCommandEvent& event) {
	Update();
}

void ReportExpensesByMonthPanel::ShowPopup() {
	_chartPopup->Show();
}

void ReportExpensesByMonthPanel::HidePopup() {
	_chartPopup->Hide();
}

void ReportExpensesByMonthPanel::UpdatePopup(int x, int y, int index) {
	wxDateTime date = _values[index].date;

	wxDateTime fromDate = date;
	wxDateTime toDate = wxDateTime(date);
	toDate.SetToLastMonthDay();

	std::vector<StringValueViewModel> popupValues;

	popupValues = _context.GetReportingService().GetExpensesByAccount(_selectedIds, fromDate, toDate);

	wxPoint position = _chart->ClientToScreen(wxPoint(x, y));
	_chartPopup->SetPosition(position);

	_chartPopup->Update(date.Format("%B"), popupValues);
}

void ReportExpensesByMonthPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(static_cast<int>(ReportType::ExpensesByMonth));

	std::vector<int> ids = String::Split(settings.accountIds.ToStdWstring(), ',');

	std::set<int> s(ids.begin(), ids.end());
	_selectedIds = s;

	_periodFilterPanel->SetFromDate(settings.fromDate);
	_periodFilterPanel->SetToDate(settings.toDate);
	_periodFilterPanel->SetPeriod(settings.period);

	_accountsComboBox->SetSelection(_selectedIds);
	_averageCheckbox->SetValue(settings.average);
}

void ReportExpensesByMonthPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(static_cast<int>(ReportType::ExpensesByMonth), String::Join(_selectedIds, ","), _periodFilterPanel->GetPeriod(), _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate(), _averageCheckbox->IsChecked());
}