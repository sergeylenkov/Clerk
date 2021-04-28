#include "ReportBalanceByMonthPanel.h"

ReportBalancePanel::ReportBalancePanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	chart = new LineChart(this, wxID_ANY);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));
	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st3 = new wxStaticText(filterPanel, wxID_ANY, wxT("Account:"));

	accountList = new wxBitmapComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(200, 20), 0, NULL, wxCB_READONLY);

	wxStaticText *st4 = new wxStaticText(filterPanel, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("3 Months"));
	values->Add(wxT("6 Months"));
	values->Add(wxT("Current Year"));
	values->Add(wxT("Previous Year"));
	values->Add(wxT("Custom"));

	periodList = new wxComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(120, 20), *values, wxCB_DROPDOWN | wxCB_READONLY);
	delete values;

	wxStaticText *st1 = new wxStaticText(filterPanel, wxID_ANY, wxT("From:"));
	fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	wxStaticText *st2 = new wxStaticText(filterPanel, wxID_ANY, wxT("To:"));
	toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	filterSizer->Add(st3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(accountList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(st4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(periodList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(st1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(st2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	mainSizer->Add(filterPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);

	wxPanel *chartPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

	wxBoxSizer *chartSizer = new wxBoxSizer(wxHORIZONTAL);

	chart->SetMinSize(wxSize(-1, 600));
	chart->SetMaxSize(wxSize(-1, 600));

	chartSizer->Add(chart, 1, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	chartPanel->SetSizer(chartSizer);
	chartPanel->Layout();

	mainSizer->Add(chartPanel, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);

	/*auto account = std::make_shared<AccountViewModel>();
	account->name = wxString("All");
	account->id = -1;
	account->icon = -1;

	_accounts.push_back(account);*/

	for (auto &account : _context.GetAccountsService().GetByType(AccountType::Deposit))
	{
		_accounts.push_back(account);
	}

	/*for (auto &account : _accounts) {
		int iconId = 0;

		if (account->icon < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->icon;
		}

		accountList->Append(account->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));
	}*/

	chartPopup = new ExpensesTooltipPopup(this);

	accountList->Bind(wxEVT_COMBOBOX, &ReportBalancePanel::OnAccountSelect, this);
	periodList->Bind(wxEVT_COMBOBOX, &ReportBalancePanel::OnPeriodSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportBalancePanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportBalancePanel::OnDateChanged, this);

	chart->OnShowPopup = std::bind(&ReportBalancePanel::ShowPopup, this);
	chart->OnHidePopup = std::bind(&ReportBalancePanel::HidePopup, this);
	chart->OnUpdatePopup = std::bind(&ReportBalancePanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	accountList->Select(0);
	periodList->Select(3);

	RestoreFilterSettings();
}

ReportBalancePanel::~ReportBalancePanel() {
	SaveFilterSettings();
}

void ReportBalancePanel::Update() {
	wxDateTime fromDate = fromDatePicker->GetValue();
	wxDateTime toDate = toDatePicker->GetValue();

	if (accountList->GetSelection() < _accounts.size()) {
		auto account = _accounts[accountList->GetSelection()];

		values = _context.GetReportingService().GetBalanceByMonth(*account, fromDate, toDate);

		std::vector<StringValueViewModel> chartValues;

		for (auto& value : values)
		{
			StringValueViewModel chartValue = { value.date.Format("%B"), value.value };
			chartValues.push_back(chartValue);
		}

		chart->SetValues(chartValues);
	}
}

void ReportBalancePanel::OnAccountSelect(wxCommandEvent &event) {
	Update();
}

void ReportBalancePanel::OnDateChanged(wxDateEvent &event) {
	periodFromDate = fromDatePicker->GetValue();
	periodToDate = toDatePicker->GetValue();

	SaveFilterSettings();
	Update();
}

void ReportBalancePanel::OnPeriodSelect(wxCommandEvent &event) {
	SaveFilterSettings();

	CalculatePeriod();
	Update();
}

void ReportBalancePanel::ShowPopup() {
	chartPopup->Show();
}

void ReportBalancePanel::HidePopup() {
	chartPopup->Hide();
}

void ReportBalancePanel::UpdatePopup(int x, int y, int index) {
	std::vector<StringValueViewModel> popupValues;

	wxDateTime date = values[index].date;

	StringValueViewModel value = { date.Format("%B"), values[index].value };

	popupValues.push_back(value);

	wxPoint pos = chart->ClientToScreen(wxPoint(x, y));
	chartPopup->SetPosition(pos);

	chartPopup->Update(date.Format("%B"), popupValues);
}

void ReportBalancePanel::RestoreFilterSettings() {
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

void ReportBalancePanel::SaveFilterSettings() {
	/*Account *account = _accounts[accountList->GetSelection()].get();

	Settings::GetInstance().SetReportFilterSettings(2, wxString::Format("%d", account->id), periodList->GetSelection(), periodFromDate, periodToDate);*/
}

void ReportBalancePanel::CalculatePeriod() {
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
		Periods::Calculate(Periods::Type::PreviousYear, fromDate, toDate);
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