#include "ReportExpensesByMonthPanel.h"

ReportExpensesByMonthPanel::ReportExpensesByMonthPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	chart = new LineChart(this, wxID_ANY);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));
	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);	

	wxStaticText *st3 = new wxStaticText(filterPanel, wxID_ANY, wxT("Accounts:"));
	
	accountsComboBox = new AccountsComboBox(filterPanel, wxID_ANY, wxEmptyString, wxPoint(0, 0), wxSize(200, 20));
	
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
	filterSizer->Add(accountsComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
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

	auto account = make_shared<Account>();
	account->name = make_shared<wxString>("All");
	account->id = -1;
	account->iconId = -1;

	accounts.push_back(account);

	for (auto &account : DataHelper::GetInstance().GetAccountsByType(AccountType::Expens))
	{
		accounts.push_back(account);
	}

	chartPopup = new ExpensesTooltipPopup(this);

	accountsComboBox->OnChange = std::bind(&ReportExpensesByMonthPanel::OnAccountSelect, this, std::placeholders::_1);
	periodList->Bind(wxEVT_COMBOBOX, &ReportExpensesByMonthPanel::OnPeriodSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesByMonthPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesByMonthPanel::OnDateChanged, this);	

	chart->OnShowPopup = std::bind(&ReportExpensesByMonthPanel::ShowPopup, this);
	chart->OnHidePopup = std::bind(&ReportExpensesByMonthPanel::HidePopup, this);
	chart->OnUpdatePopup = std::bind(&ReportExpensesByMonthPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	periodList->Select(3);
	accountsComboBox->SetAccounts(accounts);

	selectedIds = {};

	RestoreFilterSettings();
}

ReportExpensesByMonthPanel::~ReportExpensesByMonthPanel() {
	SaveFilterSettings();
}

void ReportExpensesByMonthPanel::Update() {
	wxDateTime fromDate = fromDatePicker->GetValue();
	wxDateTime toDate = toDatePicker->GetValue();
	
	if (selectedIds.count(-1) > 0) {
		values = DataHelper::GetInstance().GetExpensesByMonth(&fromDate, &toDate);
	}
	else {
		values = DataHelper::GetInstance().GetExpensesByMonth(GetSelectedAccounsIds(), &fromDate, &toDate);
	}	

	std::vector<StringValue> chartValues;

	for (auto value : values)
	{
		StringValue chartValue = { value.date.Format("%B"), value.value };
		chartValues.push_back(chartValue);
	}

	chart->SetValues(chartValues);
}

void ReportExpensesByMonthPanel::OnAccountSelect(std::set<int> ids) {
	selectedIds = ids;
	Update();
}

void ReportExpensesByMonthPanel::OnDateChanged(wxDateEvent &event) {
	periodFromDate = fromDatePicker->GetValue();
	periodToDate = toDatePicker->GetValue();

	SaveFilterSettings();
	Update();
}

void ReportExpensesByMonthPanel::OnPeriodSelect(wxCommandEvent &event) {
	SaveFilterSettings();

	CalculatePeriod();
	Update();
}

void ReportExpensesByMonthPanel::ShowPopup() {
	chartPopup->Show();
}

void ReportExpensesByMonthPanel::HidePopup() {
	chartPopup->Hide();
}

void ReportExpensesByMonthPanel::UpdatePopup(int x, int y, int index) {
	wxDateTime date = values[index].date;

	wxDateTime fromDate = date;
	wxDateTime toDate = wxDateTime(date);
	toDate.SetToLastMonthDay();

	vector<StringValue> popupValues;

	if (selectedIds.count(-1) > 0) {
		popupValues = DataHelper::GetInstance().GetExpensesByAccount(&fromDate, &toDate);
	}
	else {
		popupValues = DataHelper::GetInstance().GetExpensesByAccount(GetSelectedAccounsIds(), &fromDate, &toDate);
	}

	wxPoint position = chart->ClientToScreen(wxPoint(x, y));
	chartPopup->SetPosition(position);

	chartPopup->Update(date.Format("%B"), popupValues);
}

void ReportExpensesByMonthPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(1);

	this->selectedIds = {};

	std::string str = settings.accountIds.mb_str();

	std::stringstream ss(str);

	int i;

	while (ss >> i)
	{
		selectedIds.insert(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	periodList->SetSelection(settings.period);

	periodFromDate = settings.fromDate;
	periodToDate = settings.toDate;

	accountsComboBox->SetSelection(this->selectedIds);

	CalculatePeriod();
}

void ReportExpensesByMonthPanel::SaveFilterSettings() {

	Settings::GetInstance().SetReportFilterSettings(1, GetSelectedAccounsIds(), periodList->GetSelection(), periodFromDate, periodToDate);
}

void ReportExpensesByMonthPanel::CalculatePeriod() {
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
			Utils::CalculatePeriod(PeriodTypes::PreviousYear, fromDate, toDate);
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

wxString ReportExpensesByMonthPanel::GetSelectedAccounsIds() {
	wxString ids = "";

	for (auto &account : accounts) {
		if (selectedIds.count(account->id) > 0) {
			ids = ids + wxString::Format("%i,", account->id);
		}
	}

	ids.RemoveLast();

	return ids;
}