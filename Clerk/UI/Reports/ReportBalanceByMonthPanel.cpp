#include "ReportBalanceByMonthPanel.h"

ReportBalanceByMonthPanel::ReportBalanceByMonthPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	SetBackgroundColour(*wxWHITE);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);
		
	wxStaticText* accountsLabel = new wxStaticText(this, wxID_ANY, _("Accounts:"));
	filterSizer->Add(accountsLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	_accountsComboBox = new AccountsComboBox(this, wxDefaultPosition, FromDIP(wxSize(200, 20)), true);
	filterSizer->Add(_accountsComboBox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	wxStaticText* periodLabel = new wxStaticText(this, wxID_ANY, _("Period:"));
	filterSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	_periodFilterPanel = new PeriodFilterPanel(this, PeriodFilterType::Report);
	_periodFilterPanel->OnChange = [&]() {
		Update();
	};

	filterSizer->Add(_periodFilterPanel, 1, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(filterSizer, 0, wxEXPAND | wxALL, FromDIP(10));

	_chart = new LineChart(this, wxID_ANY);

	wxBoxSizer *chartSizer = new wxBoxSizer(wxHORIZONTAL);

	_chart->SetMinSize(FromDIP(wxSize(-1, 600)));
	_chart->SetMaxSize(FromDIP(wxSize(-1, 600)));

	chartSizer->Add(_chart, 1, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(chartSizer, 1, wxEXPAND | wxALL, FromDIP(10));

	SetSizer(mainSizer);	

	_chartPopup = new ExpensesTooltipPopup(this);

	_accountsComboBox->OnChange = std::bind(&ReportBalanceByMonthPanel::OnAccountSelect, this, std::placeholders::_1);

	_chart->OnShowPopup = std::bind(&ReportBalanceByMonthPanel::ShowPopup, this);
	_chart->OnHidePopup = std::bind(&ReportBalanceByMonthPanel::HidePopup, this);
	_chart->OnUpdatePopup = std::bind(&ReportBalanceByMonthPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	_selectedIds = {};

	_accounts = _context.GetAccountsService().GetDeposits();

	std::sort(_accounts.begin(), _accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_accountsComboBox->SetAccounts(_accounts);

	RestoreFilterSettings();
}

ReportBalanceByMonthPanel::~ReportBalanceByMonthPanel() {
	SaveFilterSettings();
}

void ReportBalanceByMonthPanel::Update() {
	_values = _context.GetReportingService().GetBalanceByMonth(_selectedIds, _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());

	std::vector<StringValueViewModel> chartValues;

	for (auto& value : _values)
	{
		StringValueViewModel chartValue = { value.date.Format("%B"), value.value };
		chartValues.push_back(chartValue);
	}

	_chart->SetValues(chartValues);
}

void ReportBalanceByMonthPanel::OnAccountSelect(std::set<int> ids) {
	_selectedIds = ids;
	Update();
}

void ReportBalanceByMonthPanel::ShowPopup() {
	_chartPopup->Show();
}

void ReportBalanceByMonthPanel::HidePopup() {
	_chartPopup->Hide();
}

void ReportBalanceByMonthPanel::UpdatePopup(int x, int y, int index) {
	std::vector<StringValueViewModel> popupValues;

	wxDateTime date = _values[index].date;

	StringValueViewModel value = { date.Format("%B"), _values[index].value };

	popupValues.push_back(value);

	wxPoint pos = _chart->ClientToScreen(wxPoint(x, y));
	_chartPopup->SetPosition(pos);

	_chartPopup->Update(date.Format("%B"), popupValues);
}

void ReportBalanceByMonthPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(static_cast<int>(ReportType::BalanceByMonth));

	std::vector<int> ids = String::Split(settings.accountIds.ToStdWstring(), ',');

	std::set<int> s(ids.begin(), ids.end());
	_selectedIds = s;

	_periodFilterPanel->SetFromDate(settings.fromDate);
	_periodFilterPanel->SetToDate(settings.toDate);
	_periodFilterPanel->SetPeriod(settings.period);

	_accountsComboBox->SetSelection(_selectedIds);
}

void ReportBalanceByMonthPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(static_cast<int>(ReportType::BalanceByMonth), String::Join(_selectedIds, ","), _periodFilterPanel->GetPeriod(), _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate(), false);
}