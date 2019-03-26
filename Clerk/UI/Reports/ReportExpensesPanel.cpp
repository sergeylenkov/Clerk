#include "ReportExpensesPanel.h"

ReportExpensesPanel::ReportExpensesPanel(wxWindow *parent, wxWindowID id) : DataPanel(parent, id) {
	chart = new LineChart(this, wxID_ANY);

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxPanel *filterPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40));

	wxBoxSizer *filterSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *periodSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *st3 = new wxStaticText(filterPanel, wxID_ANY, wxT("Account:"));

	accountList = new wxBitmapComboBox(filterPanel, wxID_ANY, "", wxPoint(0, 0), wxSize(200, 20), 0, NULL, wxCB_READONLY);

	wxStaticText *st1 = new wxStaticText(filterPanel, wxID_ANY, wxT("From:"));
	fromDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);
	wxStaticText *st2 = new wxStaticText(filterPanel, wxID_ANY, wxT("To:"));
	toDatePicker = new wxDatePickerCtrl(filterPanel, wxID_ANY, wxDefaultDateTime, wxPoint(0, 0), wxSize(100, 20), wxDP_DROPDOWN);

	periodSizer->Add(st3, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(accountList, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(st1, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(st2, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	periodSizer->Add(toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	filterSizer->Add(periodSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 5);

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

	for each (auto account in DataHelper::GetInstance().GetAccountsByType(AccountTypes::Expens))
	{
		accounts.push_back(account);
	}

	for each (auto account in accounts) {
		int iconId = 0;

		if (account->iconId < DataHelper::GetInstance().accountsImageList->GetImageCount()) {
			iconId = account->iconId;
		}

		accountList->Append(*account->name, DataHelper::GetInstance().accountsImageList->GetBitmap(iconId));
	}

	accountList->Select(0);

	RestoreFilterSettings();

	chartPopup = new GraphPopup(this);

	accountList->Bind(wxEVT_COMBOBOX, &ReportExpensesPanel::OnAccountSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportExpensesPanel::OnDateChanged, this);

	chart->OnShowPopup = std::bind(&ReportExpensesPanel::ShowPopup, this);
	chart->OnHidePopup = std::bind(&ReportExpensesPanel::HidePopup, this);
	chart->OnUpdatePopup = std::bind(&ReportExpensesPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

ReportExpensesPanel::~ReportExpensesPanel() {
	SaveFilterSettings();
}

void ReportExpensesPanel::Update() {
	wxDateTime fromDate = fromDatePicker->GetValue();
	wxDateTime toDate = toDatePicker->GetValue();

	Account *account = accounts[accountList->GetSelection()].get();

	values = DataHelper::GetInstance().GetExpensesByMonth(account, &fromDate, &toDate);

	std::vector<StringValue> chartValues;

	for each (auto value in values)
	{
		StringValue chartValue = { value.date.Format("%B"), value.value };
		chartValues.push_back(chartValue);
	}

	chart->SetValues(chartValues);
}

void ReportExpensesPanel::OnAccountSelect(wxCommandEvent &event) {
	Update();
}

void ReportExpensesPanel::OnDateChanged(wxDateEvent &event) {
	Update();
}

void ReportExpensesPanel::ShowPopup() {
	chartPopup->Show();
}

void ReportExpensesPanel::HidePopup() {
	chartPopup->Hide();
}

void ReportExpensesPanel::UpdatePopup(int x, int y, int index) {
	wxDateTime date = values[index].date;

	wxDateTime fromDate = date;
	wxDateTime toDate = wxDateTime(date);
	toDate.SetToLastMonthDay();

	Account *account = accounts[accountList->GetSelection()].get();

	vector<StringValue> popupValues = DataHelper::GetInstance().GetExpensesForAccount(account, &fromDate, &toDate);

	wxPoint pos = chart->ClientToScreen(wxPoint(x, y));
	chartPopup->SetPosition(pos);

	chartPopup->Update(popupValues);
}

void ReportExpensesPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(1);

	for (unsigned int i = 0; i < accounts.size(); i++) {
		if (accounts[i]->id == settings.accountId) {
			accountList->SetSelection(i);
		}
	}

	fromDatePicker->SetValue(settings.fromDate);
	toDatePicker->SetValue(settings.toDate);
}

void ReportExpensesPanel::SaveFilterSettings() {
	Account *account = accounts[accountList->GetSelection()].get();

	Settings::GetInstance().SetReportFilterSettings(1, account->id, fromDatePicker->GetValue(), toDatePicker->GetValue());
}