#include "ReportReceiptsAndExpensesByMonthPanel.h"

ReportReceiptsAndExpensesByMonthPanel::ReportReceiptsAndExpensesByMonthPanel(wxWindow* parent, DataContext& context, Icons& icons) :
	DataPanel(parent, context, icons)
{
	SetBackgroundColour(*wxWHITE);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* accountsLabel = new wxStaticText(this, wxID_ANY, _("Accounts:"));
	filterSizer->Add(accountsLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	_accountsComboBox = new AccountsComboBox(this, wxDefaultPosition, FromDIP(wxSize(200, 20)), true);
	_accountsComboBox->OnChange = [&](std::set<int> ids) {
		_selectedIds = ids;
		Update();
	};

	filterSizer->Add(_accountsComboBox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	_periodFilterPanel = new PeriodFilterPanel(this, PeriodFilterType::Report);
	_periodFilterPanel->OnChange = [&]() {
		Update();
	};

	filterSizer->Add(_periodFilterPanel, 1, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(filterSizer, 0, wxEXPAND | wxALL, FromDIP(10));

	_chart = new GroupedBarChart(this);
	_chart->OnShowPopup = [&]() {
		ShowPopup();
	};

	_chart->OnHidePopup = [&]() {
		HidePopup();
	};

	_chart->OnUpdatePopup = [&](int x, int y, int index) {
		UpdatePopup(x, y, index);
	};

	_chart->SetMinSize(FromDIP(wxSize(-1, 600)));
	_chart->SetMaxSize(FromDIP(wxSize(-1, 600)));

	wxBoxSizer* chartSizer = new wxBoxSizer(wxHORIZONTAL);

	chartSizer->Add(_chart, 1, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(chartSizer, 1, wxEXPAND | wxALL, FromDIP(10));

	SetSizer(mainSizer);
	Layout();

	_chartPopup = new ExpensesTooltipPopup(this);

	_selectedIds = {};

	auto receipts = _context.GetAccountsService().GetReceipts();

	std::sort(receipts.begin(), receipts.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_accounts.insert(_accounts.end(), receipts.begin(), receipts.end());

	auto expenses = _context.GetAccountsService().GetExpenses();

	std::sort(expenses.begin(), expenses.end(), [](auto a, auto b) {
		return a->order < b->order;
	});

	_accounts.insert(_accounts.end(), expenses.begin(), expenses.end());

	_accountsComboBox->SetAccounts(_accounts);

	RestoreFilterSettings();
}

void ReportReceiptsAndExpensesByMonthPanel::ShowPopup() {
	_chartPopup->Show();
}

void ReportReceiptsAndExpensesByMonthPanel::HidePopup() {
	_chartPopup->Hide();
}

void ReportReceiptsAndExpensesByMonthPanel::UpdatePopup(int x, int y, int index) {
	auto receipt = _receipts[index];
	auto expense = _expenses[index];

	wxDateTime date = receipt.date;

	std::vector<StringValueViewModel> popupValues;

	popupValues = { { _("Receipts"), receipt.value }, { _("Expenses"), expense.value } };

	wxPoint position = _chart->ClientToScreen(wxPoint(x, y));
	_chartPopup->SetPosition(position);

	_chartPopup->Update(date.Format("%B"), popupValues);
}

ReportReceiptsAndExpensesByMonthPanel::~ReportReceiptsAndExpensesByMonthPanel() {
	SaveFilterSettings();
}

void ReportReceiptsAndExpensesByMonthPanel::Update() {
	_receipts = _context.GetReportingService().GetReceiptsByMonth(_selectedIds, _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());
	_expenses = _context.GetReportingService().GetExpensesByMonth(_selectedIds, _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());	

	std::vector<StringValuesViewModel> chartValues;

	for (int i = 0; i < _receipts.size(); i++) {
		auto receipt = _receipts[i];
		auto expense = _expenses[i];

		chartValues.push_back({ receipt.date.Format("%B"), { receipt.value, expense.value } });
	}

	_chart->SetValues(chartValues);
}

void ReportReceiptsAndExpensesByMonthPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(static_cast<int>(ReportType::ReceiptsAndExpensesByMonth));
	std::vector<int> ids = String::Split(settings.accountIds.ToStdWstring(), ',');

	std::set<int> s(ids.begin(), ids.end());
	_selectedIds = s;

	_periodFilterPanel->SetFromDate(settings.fromDate);
	_periodFilterPanel->SetToDate(settings.toDate);
	_periodFilterPanel->SetPeriod(settings.period);

	_accountsComboBox->SetSelection(_selectedIds);
}

void ReportReceiptsAndExpensesByMonthPanel::SaveFilterSettings() {
	Settings::GetInstance().SetReportFilterSettings(static_cast<int>(ReportType::ReceiptsAndExpensesByMonth), String::Join(_selectedIds, ","), _periodFilterPanel->GetPeriod(), _periodFilterPanel->GetFromDate(), _periodFilterPanel->GetToDate());
}