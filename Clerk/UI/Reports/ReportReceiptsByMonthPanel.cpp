#include "ReportReceiptsByMonthPanel.h"

ReportReceiptsByMonthPanel::ReportReceiptsByMonthPanel(wxWindow* parent, DataContext& context) : DataPanel(parent, context) {
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* accountsLabel = new wxStaticText(this, wxID_ANY, _("Accounts:"));
	filterSizer->Add(accountsLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	_accountsComboBox = new AccountsComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, FromDIP(wxSize(200, 20)));
	filterSizer->Add(_accountsComboBox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	wxStaticText* periodLabel = new wxStaticText(this, wxID_ANY, _("Period:"));
	filterSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	wxArrayString* values = new wxArrayString();

	values->Add(_("3 Months"));
	values->Add(_("6 Months"));
	values->Add(_("Current Year"));
	values->Add(_("Previous Year"));
	values->Add(_("Custom"));

	_periodList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(120, 20)), *values, wxCB_DROPDOWN | wxCB_READONLY);
	filterSizer->Add(_periodList, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	delete values;

	wxStaticText* fromLabel = new wxStaticText(this, wxID_ANY, _("From:"));
	filterSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	_fromDatePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);
	filterSizer->Add(_fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	wxStaticText* toLabel = new wxStaticText(this, wxID_ANY, _("To:"));
	filterSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));

	_toDatePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);
	filterSizer->Add(_toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(10));

	filterSizer->Add(0, 0, 1, wxEXPAND);

	_averageCheckbox = new wxCheckBox(this, wxID_ANY, _("Show average"));

	filterSizer->Add(_averageCheckbox, 0, wxALIGN_CENTER_VERTICAL);

	mainSizer->Add(filterSizer, 0, wxEXPAND | wxALL, FromDIP(10));

	_chart = new LineChart(this, wxID_ANY);

	wxBoxSizer* chartSizer = new wxBoxSizer(wxHORIZONTAL);

	_chart->SetMinSize(FromDIP(wxSize(-1, 600)));
	_chart->SetMaxSize(FromDIP(wxSize(-1, 600)));

	chartSizer->Add(_chart, 1, wxALIGN_CENTER_VERTICAL | wxALL);

	mainSizer->Add(chartSizer, 1, wxEXPAND | wxALL, FromDIP(10));

	SetSizer(mainSizer);

	auto account = std::make_shared<AccountPresentationModel>();
	account->name = wxString("All");
	account->id = -1;
	account->icon = -1;

	_accounts.push_back(account);

	for (auto& account : _context.GetAccountsService().GetByType(AccountType::Receipt))
	{
		_accounts.push_back(account);
	}

	_chartPopup = new ExpensesTooltipPopup(this);

	_accountsComboBox->OnChange = std::bind(&ReportReceiptsByMonthPanel::OnAccountSelect, this, std::placeholders::_1);
	_periodList->Bind(wxEVT_COMBOBOX, &ReportReceiptsByMonthPanel::OnPeriodSelect, this);
	_fromDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportReceiptsByMonthPanel::OnDateChanged, this);
	_toDatePicker->Bind(wxEVT_DATE_CHANGED, &ReportReceiptsByMonthPanel::OnDateChanged, this);
	_averageCheckbox->Bind(wxEVT_CHECKBOX, &ReportReceiptsByMonthPanel::OnDrawAverageCheck, this);

	_chart->OnShowPopup = std::bind(&ReportReceiptsByMonthPanel::ShowPopup, this);
	_chart->OnHidePopup = std::bind(&ReportReceiptsByMonthPanel::HidePopup, this);
	_chart->OnUpdatePopup = std::bind(&ReportReceiptsByMonthPanel::UpdatePopup, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	_periodList->Select(3);
	_accountsComboBox->SetAccounts(_accounts);

	_selectedIds = {};

	RestoreFilterSettings();
}

ReportReceiptsByMonthPanel::~ReportReceiptsByMonthPanel() {
	SaveFilterSettings();
}

void ReportReceiptsByMonthPanel::Update() {
	wxDateTime fromDate = _fromDatePicker->GetValue();
	wxDateTime toDate = _toDatePicker->GetValue();

	_values = _context.GetReportingService().GetReceiptsByMonth(_selectedIds, fromDate, toDate);

	std::vector<StringValueViewModel> chartValues;

	for (auto& value : _values)
	{
		StringValueViewModel chartValue = { value.date.Format("%B"), value.value };
		chartValues.push_back(chartValue);
	}

	_chart->SetDrawAverage(_averageCheckbox->IsChecked());
	_chart->SetValues(chartValues);
}

void ReportReceiptsByMonthPanel::OnAccountSelect(std::set<int> ids) {
	_selectedIds = ids;
	Update();
}

void ReportReceiptsByMonthPanel::OnDateChanged(wxDateEvent& event) {
	_periodFromDate = _fromDatePicker->GetValue();
	_periodToDate = _toDatePicker->GetValue();

	SaveFilterSettings();
	Update();
}

void ReportReceiptsByMonthPanel::OnPeriodSelect(wxCommandEvent& event) {
	SaveFilterSettings();

	CalculatePeriod();
	Update();
}

void ReportReceiptsByMonthPanel::OnDrawAverageCheck(wxCommandEvent& event) {
	Update();
}

void ReportReceiptsByMonthPanel::ShowPopup() {
	_chartPopup->Show();
}

void ReportReceiptsByMonthPanel::HidePopup() {
	_chartPopup->Hide();
}

void ReportReceiptsByMonthPanel::UpdatePopup(int x, int y, int index) {
	wxDateTime date = _values[index].date;

	wxDateTime fromDate = date;
	wxDateTime toDate = wxDateTime(date);
	toDate.SetToLastMonthDay();

	std::vector<StringValueViewModel> popupValues;

	popupValues = _context.GetReportingService().GetReceiptsByAccount(_selectedIds, fromDate, toDate);

	wxPoint position = _chart->ClientToScreen(wxPoint(x, y));
	_chartPopup->SetPosition(position);

	_chartPopup->Update(date.Format("%B"), popupValues);
}

void ReportReceiptsByMonthPanel::RestoreFilterSettings() {
	ReportFilterSettings settings = Settings::GetInstance().GetReportFilterSettings(1);

	_selectedIds = {};

	std::string str = settings.accountIds.mb_str();

	std::stringstream ss(str);

	int i;

	while (ss >> i)
	{
		_selectedIds.insert(i);

		if (ss.peek() == ',') {
			ss.ignore();
		}
	}

	_periodList->SetSelection(settings.period);

	_periodFromDate = settings.fromDate;
	_periodToDate = settings.toDate;

	_accountsComboBox->SetSelection(_selectedIds);

	_averageCheckbox->SetValue(settings.average);

	CalculatePeriod();
}

void ReportReceiptsByMonthPanel::SaveFilterSettings() {

	Settings::GetInstance().SetReportFilterSettings(1, GetSelectedAccounsIds(), _periodList->GetSelection(), _periodFromDate, _periodToDate, _averageCheckbox->IsChecked());
}

void ReportReceiptsByMonthPanel::CalculatePeriod() {
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

wxString ReportReceiptsByMonthPanel::GetSelectedAccounsIds() {
	wxString ids = "";

	for (auto& account : _accounts) {
		if (_selectedIds.count(account->id) > 0) {
			ids = ids + wxString::Format("%i,", account->id);
		}
	}

	ids.RemoveLast();

	return ids;
}