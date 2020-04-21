#include "ReportExpensesByMonthPanel.h"

ReportExpensesByMonthPanel::ReportExpensesByMonthPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	chart = new LineChart(this, wxID_ANY);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));
	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);	

	wxStaticText *st3 = new wxStaticText(filterPanel, wxID_ANY, wxT("Accounts:"));
	
	accountsComboBox = new AccountsComboBox(filterPanel, wxID_ANY, wxEmptyString, wxPoint(0, 0), wxSize(200, 20));
	//accountsList = new CheckboxComboPopup();

	//accountsComboBox->SetPopupControl(accountsList);

	//accountsList->EnableCheckBoxes(true);
	//accountsList->SetImageList(DataHelper::GetInstance().accountsImageList, wxIMAGE_LIST_SMALL);

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

	//accountsList->OnItemSelect = std::bind(&ReportExpensesByMonthPanel::OnAccountSelect, this, std::placeholders::_1);
	periodList->Bind(wxEVT_COMBOBOX, &ReportExpensesByMonthPanel::OnPeriodSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesByMonthPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesByMonthPanel::OnDateChanged, this);	

	chart->OnShowPopup = std::bind(&ReportExpensesByMonthPanel::ShowPopup, this);
	chart->OnHidePopup = std::bind(&ReportExpensesByMonthPanel::HidePopup, this);
	chart->OnUpdatePopup = std::bind(&ReportExpensesByMonthPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	periodList->Select(3);
	accountsComboBox->SetAccounts(accounts);

	selectedIds = {};

	//UpdateAccountsList();
	RestoreFilterSettings();
}

ReportExpensesByMonthPanel::~ReportExpensesByMonthPanel() {
	SaveFilterSettings();
}

void ReportExpensesByMonthPanel::Update() {
	wxDateTime fromDate = fromDatePicker->GetValue();
	wxDateTime toDate = toDatePicker->GetValue();

	wxString ids = "";

	for (auto id : selectedIds) {
		auto account = accounts[id];

		ids = ids + wxString::Format("%i,", account->id);
	}

	ids.RemoveLast();

	values = DataHelper::GetInstance().GetExpensesByMonth(ids, &fromDate, &toDate);
	//values = DataHelper::GetInstance().GetExpensesByMonth(&fromDate, &toDate);
	/*if (accountList->GetSelection() == 0) {
		values = DataHelper::GetInstance().GetExpensesByMonth(&fromDate, &toDate);
	}
	else {
		auto account = accounts[accountList->GetSelection()];
		values = DataHelper::GetInstance().GetExpensesByMonth(*account, &fromDate, &toDate);
	}*/	

	std::vector<StringValue> chartValues;

	for (auto value : values)
	{
		StringValue chartValue = { value.date.Format("%B"), value.value };
		chartValues.push_back(chartValue);
	}

	chart->SetValues(chartValues);
}

void ReportExpensesByMonthPanel::OnAccountSelect(int index) {
	if (selectedIds.count(index) == 0) {
		selectedIds.insert(index);
	}
	else {
		selectedIds.erase(index);
	}

	wxString names = "";

	for (auto id : selectedIds) {
		auto account = accounts[id];

		names = names + wxString::Format("%s, ", *account->name);
	}

	names.RemoveLast(2);

	accountsComboBox->SetValue(names);
	//accountsList->SetStringValue(names);
	
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

	wxString ids = "";

	for (auto id : selectedIds) {
		auto account = accounts[id];

		ids = ids + wxString::Format("%i,", account->id);
	}

	ids.RemoveLast();

	popupValues = DataHelper::GetInstance().GetExpensesByAccount(ids, &fromDate, &toDate);
	/*if (accountList->GetSelection() == 0) {
		popupValues = DataHelper::GetInstance().GetExpensesByAccount(&fromDate, &toDate);
	}
	else {
		auto account = accounts[accountList->GetSelection()];
		popupValues = DataHelper::GetInstance().GetExpensesForAccount(*account, &fromDate, &toDate);
	}*/	

	wxPoint position = chart->ClientToScreen(wxPoint(x, y));
	chartPopup->SetPosition(position);

	chartPopup->Update(date.Format("%B"), popupValues);
}

void ReportExpensesByMonthPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(1);

	/*for (unsigned int i = 0; i < accounts.size(); i++) {
		if (accounts[i]->id == settings.accountId) {
			accountList->SetSelection(i);
		}
	}*/

	periodList->SetSelection(settings.period);

	periodFromDate = settings.fromDate;
	periodToDate = settings.toDate;

	CalculatePeriod();
}

void ReportExpensesByMonthPanel::SaveFilterSettings() {
	/*Account *account = accounts[accountList->GetSelection()].get();

	Settings::GetInstance().SetReportFilterSettings(1, account->id, periodList->GetSelection(), periodFromDate, periodToDate);*/
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

/*void ReportExpensesByMonthPanel::UpdateAccountsList() {
	wxListItem column;

	column.SetId(0);
	column.SetText(_("Name"));
	column.SetWidth(180);

	accountsList->InsertColumn(0, column);

	int i = 0;

	for (auto& account : accounts)
	{
		wxListItem listItem;

		listItem.SetId(i);
		listItem.SetData(account->id);

		accountsList->InsertItem(i, *account->name, account->iconId);

		i++;
	}
}*/