#include "ReportExpensesByMonthPanel.h"

ReportExpensesByMonthPanel::ReportExpensesByMonthPanel(wxWindow *parent, DataContext& context) : DataPanel(parent, context) {
	chart = new LineChart(this, wxID_ANY);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));
	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);	

	wxStaticText *accountsLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("Accounts:"));	
	accountsComboBox = new AccountsComboBox(filterPanel, wxID_ANY, wxEmptyString, wxPoint(0, 0), wxSize(200, 20));
	
	wxStaticText *periodLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("Period:"));

	wxArrayString *values = new wxArrayString();

	values->Add(wxT("3 Months"));
	values->Add(wxT("6 Months"));
	values->Add(wxT("Current Year"));
	values->Add(wxT("Previous Year"));
	values->Add(wxT("Custom"));

	periodList = new wxComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(120, 20), *values, wxCB_DROPDOWN | wxCB_READONLY);
	delete values;

	wxStaticText *fromLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("From:"));
	fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	wxStaticText *toLabel = new wxStaticText(filterPanel, wxID_ANY, wxT("To:"));
	toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	averageCheckbox = new wxCheckBox(filterPanel, wxID_ANY, wxT("Show average"));

	filterSizer->Add(accountsLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(accountsComboBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(periodList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	filterSizer->Add(0, 0, 1, wxEXPAND, 5);
	filterSizer->Add(averageCheckbox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

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

	auto account = new AccountPresentationModel();
	account->name = wxString("All");
	account->id = -1;
	account->icon = -1;

	_accounts.push_back(account);

	for (auto &account : _context.GetAccountsService().GetByType(AccountType::Expens))
	{
		_accounts.push_back(account);
	}

	chartPopup = new ExpensesTooltipPopup(this);

	accountsComboBox->OnChange = std::bind(&ReportExpensesByMonthPanel::OnAccountSelect, this, std::placeholders::_1);
	periodList->Bind(wxEVT_COMBOBOX, &ReportExpensesByMonthPanel::OnPeriodSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesByMonthPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesByMonthPanel::OnDateChanged, this);	
	averageCheckbox->Bind(wxEVT_CHECKBOX, &ReportExpensesByMonthPanel::OnDrawAverageCheck, this);

	chart->OnShowPopup = std::bind(&ReportExpensesByMonthPanel::ShowPopup, this);
	chart->OnHidePopup = std::bind(&ReportExpensesByMonthPanel::HidePopup, this);
	chart->OnUpdatePopup = std::bind(&ReportExpensesByMonthPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	periodList->Select(3);
	accountsComboBox->SetAccounts(_accounts);

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
		values = _context.GetReportingService().GetExpensesByMonth(fromDate, toDate);
	}
	else {
		values = _context.GetReportingService().GetExpensesByMonth(selectedIds, fromDate, toDate);
	}	

	std::vector<StringValueViewModel> chartValues;

	for (auto &value : values)
	{
		StringValueViewModel chartValue = { value.date.Format("%B"), value.value };
		chartValues.push_back(chartValue);
	}

	chart->SetDrawAverage(averageCheckbox->IsChecked());
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

void ReportExpensesByMonthPanel::OnDrawAverageCheck(wxCommandEvent& event) {
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

	std::vector<StringValueViewModel> popupValues;

	if (selectedIds.count(-1) > 0) {
		popupValues = _context.GetReportingService().GetExpensesByAccount(fromDate, toDate);
	}
	else {
		popupValues = _context.GetReportingService().GetExpensesByAccount(selectedIds, fromDate, toDate);
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

	averageCheckbox->SetValue(settings.average);

	CalculatePeriod();
}

void ReportExpensesByMonthPanel::SaveFilterSettings() {

	Settings::GetInstance().SetReportFilterSettings(1, GetSelectedAccounsIds(), periodList->GetSelection(), periodFromDate, periodToDate, averageCheckbox->IsChecked());
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

wxString ReportExpensesByMonthPanel::GetSelectedAccounsIds() {
	wxString ids = "";

	for (auto &account : _accounts) {
		if (selectedIds.count(account->id) > 0) {
			ids = ids + wxString::Format("%i,", account->id);
		}
	}

	ids.RemoveLast();

	return ids;
}