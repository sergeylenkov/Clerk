#include "ReportBalanceByMonthPanel.h"

ReportBalanceByMonthPanel::ReportBalanceByMonthPanel(wxWindow *parent, DataContext& context, Icons& icons):
	DataPanel(parent, context, icons)
{
	_chart = new LineChart(this, wxID_ANY);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));
	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st3 = new wxStaticText(filterPanel, wxID_ANY, wxT("Account:"));

	_accountList = new wxBitmapComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(200, 20), 0, NULL, wxCB_READONLY);

	wxStaticText *st4 = new wxStaticText(filterPanel, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("3 Months"));
	values->Add(wxT("6 Months"));
	values->Add(wxT("Current Year"));
	values->Add(wxT("Previous Year"));
	values->Add(wxT("Custom"));

	_periodList = new wxComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(120, 20), *values, wxCB_DROPDOWN | wxCB_READONLY);
	delete values;

	wxStaticText *st1 = new wxStaticText(filterPanel, wxID_ANY, wxT("From:"));
	_fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	wxStaticText *st2 = new wxStaticText(filterPanel, wxID_ANY, wxT("To:"));
	_toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	filterSizer->Add(st3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(_accountList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(st4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(_periodList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(st1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(_fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(st2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(_toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	mainSizer->Add(filterPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);

	wxPanel *chartPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	wxBoxSizer *chartSizer = new wxBoxSizer(wxHORIZONTAL);

	_chart->SetMinSize(wxSize(-1, 600));
	_chart->SetMaxSize(wxSize(-1, 600));

	chartSizer->Add(_chart, 1, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	chartPanel->SetSizer(chartSizer);
	chartPanel->Layout();

	mainSizer->Add(chartPanel, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);

	for (auto &account : _context.GetAccountsService().GetByType(AccountType::Deposit))
	{
		_accounts.push_back(account);
	}

	_chartPopup = new ExpensesTooltipPopup(this);

	_accountList->Bind(wxEVT_COMBOBOX, &ReportBalanceByMonthPanel::OnAccountSelect, this);
	_periodList->Bind(wxEVT_COMBOBOX, &ReportBalanceByMonthPanel::OnPeriodSelect, this);
	_fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportBalanceByMonthPanel::OnDateChanged, this);
	_toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportBalanceByMonthPanel::OnDateChanged, this);

	_chart->OnShowPopup = std::bind(&ReportBalanceByMonthPanel::ShowPopup, this);
	_chart->OnHidePopup = std::bind(&ReportBalanceByMonthPanel::HidePopup, this);
	_chart->OnUpdatePopup = std::bind(&ReportBalanceByMonthPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	_accountList->Select(0);
	_periodList->Select(3);

	RestoreFilterSettings();
}

ReportBalanceByMonthPanel::~ReportBalanceByMonthPanel() {
	SaveFilterSettings();
}

void ReportBalanceByMonthPanel::Update() {
	wxDateTime fromDate = _fromDatePicker->GetValue();
	wxDateTime toDate = _toDatePicker->GetValue();

	if (_accountList->GetSelection() < _accounts.size()) {
		auto account = _accounts[_accountList->GetSelection()];

		_values = _context.GetReportingService().GetBalanceByMonth(*account, fromDate, toDate);

		std::vector<StringValueViewModel> chartValues;

		for (auto& value : _values)
		{
			StringValueViewModel chartValue = { value.date.Format("%B"), value.value };
			chartValues.push_back(chartValue);
		}

		_chart->SetValues(chartValues);
	}
}

void ReportBalanceByMonthPanel::OnAccountSelect(wxCommandEvent &event) {
	Update();
}

void ReportBalanceByMonthPanel::OnDateChanged(wxDateEvent &event) {
	_periodFromDate = _fromDatePicker->GetValue();
	_periodToDate = _toDatePicker->GetValue();

	SaveFilterSettings();
	Update();
}

void ReportBalanceByMonthPanel::OnPeriodSelect(wxCommandEvent &event) {
	SaveFilterSettings();

	CalculatePeriod();
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

	long accountId = -1;
	settings.accountIds.ToLong(&accountId);

	/*for (unsigned int i = 0; i < _accounts.size(); i++) {
		if (accounts[i]->id == accountId) {
			accountList->SetSelection(i);
		}
	}

	periodList->SetSelection(settings.period);

	periodFromDate = settings.fromDate;
	periodToDate = settings.toDate;

	CalculatePeriod();*/
}

void ReportBalanceByMonthPanel::SaveFilterSettings() {
	/*Account *account = _accounts[accountList->GetSelection()].get();

	Settings::GetInstance().SetReportFilterSettings(2, wxString::Format("%d", account->id), periodList->GetSelection(), periodFromDate, periodToDate);*/
}

void ReportBalanceByMonthPanel::CalculatePeriod() {
	int index = _periodList->GetSelection();

	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	_fromDatePicker->Disable();
	_toDatePicker->Disable();

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
		Periods::Calculate(Periods::Type::PreviousYear, fromDate, toDate);
		break;

	case 4:
		fromDate = _periodFromDate;
		toDate = _periodToDate;

		_fromDatePicker->Enable();
		_toDatePicker->Enable();
		break;

	default:
		break;
	}

	_fromDatePicker->SetValue(fromDate);
	_toDatePicker->SetValue(toDate);
}