#include "ReportBalanceByMonthPanel.h"

ReportBalanceByMonthPanel::ReportBalanceByMonthPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);
		
	_accountsComboBox = new AccountsComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, FromDIP(wxSize(200, 20)));
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

	wxPanel *chartPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	wxBoxSizer *chartSizer = new wxBoxSizer(wxHORIZONTAL);

	_chart->SetMinSize(wxSize(-1, 600));
	_chart->SetMaxSize(wxSize(-1, 600));

	chartSizer->Add(_chart, 1, wxALIGN_CENTER_VERTICAL | wxALL);

	mainSizer->Add(chartSizer, 1, wxEXPAND | wxALL, FromDIP(10));

	SetSizer(mainSizer);

	auto account = std::make_shared<AccountPresentationModel>();
	account->name = _("All");
	account->id = -1;
	account->icon = -1;

	_accounts.push_back(account);

	auto accounts = _context.GetAccountsService().GetReceipts();

	std::sort(accounts.begin(), accounts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_accounts.insert(_accounts.end(), accounts.begin(), accounts.end());

	_chartPopup = new ExpensesTooltipPopup(this);

	_accountsComboBox->OnChange = std::bind(&ReportBalanceByMonthPanel::OnAccountSelect, this, std::placeholders::_1);

	_chart->OnShowPopup = std::bind(&ReportBalanceByMonthPanel::ShowPopup, this);
	_chart->OnHidePopup = std::bind(&ReportBalanceByMonthPanel::HidePopup, this);
	_chart->OnUpdatePopup = std::bind(&ReportBalanceByMonthPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	_accountsComboBox->SetAccounts(_accounts);

	_selectedIds = {};

	RestoreFilterSettings();
}

ReportBalanceByMonthPanel::~ReportBalanceByMonthPanel() {
	SaveFilterSettings();
}

void ReportBalanceByMonthPanel::Update() {
	for (auto& account : _accounts) {
		if (_selectedIds.count(account->id) > 0) {
			_values = _context.GetReportingService().GetBalanceByMonth(*account, _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());

			std::vector<StringValueViewModel> chartValues;

			for (auto& value : _values)
			{
				StringValueViewModel chartValue = { value.date.Format("%B"), value.value };
				chartValues.push_back(chartValue);
			}

			_chart->SetValues(chartValues);
		}
	}
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
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(2);

	std::vector<int> ids = String::Split(settings.accountIds.ToStdWstring(), ',');

	std::set<int> s(ids.begin(), ids.end());
	_selectedIds = s;

	_periodFilterPanel->SetFromDate(settings.fromDate);
	_periodFilterPanel->SetToDate(settings.toDate);
	_periodFilterPanel->SetPeriod(settings.period);

	_accountsComboBox->SetSelection(_selectedIds);
}

void ReportBalanceByMonthPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(2, GetSelectedAccounsIds(), _periodFilterPanel->GetPeriod(), _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate(), false);
}

wxString ReportBalanceByMonthPanel::GetSelectedAccounsIds() {
	std::vector<string> ids;

	for (auto& account : _accounts) {
		if (_selectedIds.count(account->id) > 0) {
			ids.push_back(std::to_string(account->id));
		}
	}

	return String::Join(ids, ",");
}