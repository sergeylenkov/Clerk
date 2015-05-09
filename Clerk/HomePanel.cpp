#include "HomePanel.h"

HomePanel::HomePanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	wxPanel *panel = new wxPanel(this, wxID_ANY);

	titleLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(10, 10), wxSize(200, 20));

	wxFont font = titleLabel->GetFont();
	font.SetPointSize(10);
	font.SetWeight(wxFONTWEIGHT_BOLD);

	titleLabel->SetFont(font);

	expensesLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(10, 40), wxSize(100, 20));
	receiptsLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(10, 60), wxSize(100, 20));

	accountsLabel = new wxStaticText(panel, wxID_ANY, "", wxPoint(240, 10), wxSize(200, 20));
	accountsLabel->SetFont(font);

	accountsList = new wxListCtrl(panel, wxID_ANY, wxPoint(235, 40), wxSize(400, 400), wxLC_REPORT | wxLC_NO_HEADER);

	barChart = new BarChart(this, wxID_ANY);
	pieChart = new PieChart(this, wxID_ANY);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(pieChart, 1, wxEXPAND);
	sizer->Add(barChart, 1, wxEXPAND);

	this->SetSizer(sizer);
	this->Layout();

	this->SetBackgroundColour(*(wxColor *)wxWHITE);
}

HomePanel::~HomePanel() {
	delete titleLabel;
	delete expensesLabel;
	delete receiptsLabel;
	delete barChart;
	delete pieChart;
}

void HomePanel::Update() {
	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	titleLabel->SetLabel(wxDateTime::Now().Format("Summary for %B"));

	fromDate.SetDay(1);
	toDate.SetToLastMonthDay();

	float expenses = DataHelper::GetInstance().GetExpenses(&fromDate, &toDate);
	float receipts = DataHelper::GetInstance().GetReceipts(&fromDate, &toDate);

	expensesLabel->SetLabel(wxString::Format("Expenses: %.2f", expenses));
	receiptsLabel->SetLabel(wxString::Format("Receipts: %.2f", receipts));

	accountsLabel->SetLabel("Accounts balance");
	accountsList->ClearAll();

	wxListItem col;

	col.SetId(0);
	col.SetText(_("Name"));
	col.SetWidth(200);

	accountsList->InsertColumn(0, col);

	wxListItem col2;

	col2.SetId(1);
	col2.SetText(_("Amount"));
	col2.SetWidth(100);
	col2.SetAlign(wxLIST_FORMAT_RIGHT);

	accountsList->InsertColumn(1, col2);

	int i = 0;

	for each (auto account in DataHelper::GetInstance().GetAccounts(AccountTypes::Deposit))
	{
		float amount = DataHelper::GetInstance().GetBalance(account.get());

		wxListItem listItem;

		listItem.SetId(i);

		accountsList->InsertItem(listItem);

		accountsList->SetItem(i, 0, *account->name);
		accountsList->SetItem(i, 1, wxString::Format("%.2f", amount));

		i++;
	}

	barChart->SetValues(DataHelper::GetInstance().GetExpensesByMonth());
	pieChart->SetValues(DataHelper::GetInstance().GetExpensesByAccount(&fromDate, &toDate));
}