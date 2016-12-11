#include "ReportPanel.h"

ReportPanel::ReportPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
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

	filterSizer->Add(periodSizer, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxEXPAND, 5);

	filterPanel->SetSizer(filterSizer);
	filterPanel->Layout();

	mainSizer->Add(filterPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 0);
	mainSizer->Add(chart, 1, wxALL | wxEXPAND, 0);

	this->SetSizer(mainSizer);

	auto account = make_shared<Account>();
	account->name = make_shared<string>("All");
	account->id = -1;
	account->iconId = 0;

	accounts.push_back(account);

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Expens))
	{
		accounts.push_back(account);
	}

	accountsImageList = new wxImageList(16, 16, true);
	wxImage image;

	for (int i = 0; i <= 50; i++) {
		wxString path = wxString::Format("Resources\\Accounts Icons\\%d.png", i);
		if (image.LoadFile(path, wxBITMAP_TYPE_PNG))
		{
			wxBitmap *bitmap = new wxBitmap(image);
			accountsImageList->Add(*bitmap);

			delete bitmap;
		}
	}

	for each (auto account in accounts) {
		int icon = 0;

		if (account->iconId < accountsImageList->GetImageCount()) {
			icon = account->iconId;
		}

		wxString name = wxString::FromUTF8(account->name.get()->c_str());

		accountList->Append(name, accountsImageList->GetBitmap(icon));
	}

	accountList->Select(0);

	chartPopup = new GraphPopup(this);

	accountList->Bind(wxEVT_COMBOBOX, &ReportPanel::OnAccountSelect, this);
	fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportPanel::OnDateChanged, this);
	toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportPanel::OnDateChanged, this);

	chart->OnShowPopup = std::bind(&ReportPanel::ShowPopup, this);
	chart->OnHidePopup = std::bind(&ReportPanel::HidePopup, this);
	chart->OnUpdatePopup = std::bind(&ReportPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

ReportPanel::~ReportPanel() {
	delete chart;
	delete accountsImageList;
	delete accountList;
	delete fromDatePicker;
	delete toDatePicker;
	delete chartPopup;
}

void ReportPanel::Update() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	fromDate.SetMonth(wxDateTime::Month::Jan);
	fromDate.SetDay(1);

	Account *account = accounts[accountList->GetSelection()].get();

	values = DataHelper::GetInstance().GetExpensesByMonth(account, &fromDate, &toDate);

	chart->SetValues(values);
}

void ReportPanel::OnAccountSelect(wxCommandEvent &event) {
	Update();
}

void ReportPanel::OnDateChanged(wxDateEvent &event) {
	Update();
}

void ReportPanel::ShowPopup() {
	chartPopup->Show();
}

void ReportPanel::HidePopup() {
	chartPopup->Hide();
}

void ReportPanel::UpdatePopup(int x, int y, int index) {
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