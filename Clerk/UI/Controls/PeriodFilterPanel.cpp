#include "PeriodFilterPanel.h"

PeriodFilterPanel::PeriodFilterPanel(wxWindow* parent, PeriodFilterType type):
	wxPanel(parent),
	_type(type)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* periodLabel = new wxStaticText(this, wxID_ANY, _("Period:"));

	wxArrayString* values = new wxArrayString();

	if (_type == PeriodFilterType::Report) {
		values->Add(_("3 Months"));
		values->Add(_("6 Months"));
		values->Add(_("Current Year"));
		values->Add(_("Previous Year"));
		values->Add(_("Custom"));
	}
	else {
		values->Add(_("Current Week"));
		values->Add(_("Previous Week"));
		values->Add(_("Current Month"));
		values->Add(_("Previous Month"));
		values->Add(_("Current Year"));
		values->Add(_("Previous Year"));
		values->Add(_("Custom"));
	}

	_periodList = new wxComboBox(this, wxID_ANY, "", wxDefaultPosition, FromDIP(wxSize(120, 20)), *values, wxCB_DROPDOWN | wxCB_READONLY);

	delete values;

	int indent = FromDIP(5);

	wxStaticText* fromLabel = new wxStaticText(this, wxID_ANY, _("From:"));
	_fromDatePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);
	wxStaticText* toLabel = new wxStaticText(this, wxID_ANY, _("To:"));
	_toDatePicker = new wxDatePickerCtrl(this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, FromDIP(wxSize(100, 20)), wxDP_DROPDOWN);

	mainSizer->Add(periodLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	mainSizer->Add(_periodList, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	mainSizer->Add(fromLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);
	mainSizer->Add(_fromDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);
	mainSizer->Add(toLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent * 2);
	mainSizer->Add(_toDatePicker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, indent);

	SetSizer(mainSizer);
	Layout();

	_periodList->Bind(wxEVT_COMBOBOX, &PeriodFilterPanel::OnPeriodSelect, this);
	_fromDatePicker->Bind(wxEVT_DATE_CHANGED, &PeriodFilterPanel::OnDateChanged, this);
	_toDatePicker->Bind(wxEVT_DATE_CHANGED, &PeriodFilterPanel::OnDateChanged, this);

	_periodList->Select(0);

	CalculatePeriod();
}

void PeriodFilterPanel::SetPeriod(int period) {
	_periodList->Select(period);
	CalculatePeriod();
}

int PeriodFilterPanel::GetPeriod() {
	return _periodList->GetSelection();
}

void PeriodFilterPanel::SetFromDate(wxDateTime date) {
	_fromDatePicker->SetValue(date);
	_periodFromDate = date;
}

wxDateTime PeriodFilterPanel::GetFromDate() {
	return _fromDatePicker->GetValue();
}

void PeriodFilterPanel::SetToDate(wxDateTime date) {
	_toDatePicker->SetValue(date);
	_periodToDate = date;
}

wxDateTime PeriodFilterPanel::GetToDate() {
	return _toDatePicker->GetValue();
}

void PeriodFilterPanel::CalculatePeriod() {
	int index = _periodList->GetSelection();

	wxDateTime fromDate = wxDateTime::Now();
	wxDateTime toDate = wxDateTime::Now();

	_fromDatePicker->Disable();
	_toDatePicker->Disable();

	if (_type == PeriodFilterType::Report) {
		switch (index)
		{
		case 0:
			Periods::Calculate(Periods::Type::ThreeMonths, fromDate, toDate);
			break;

		case 1:
			Periods::Calculate(Periods::Type::SixMonths, fromDate, toDate);
			break;

		case 2:
			Periods::Calculate(Periods::Type::CurrentYear, fromDate, toDate);
			break;

		case 3:
			Periods::Calculate(Periods::Type::PreviousYear, fromDate, toDate);
			break;

		default:
			fromDate = _periodFromDate;
			toDate = _periodToDate;

			_fromDatePicker->Enable();
			_toDatePicker->Enable();
			break;
		}
	}
	else {
		switch (index)
		{
		case 0:
			Periods::Calculate(Periods::Type::CurrentWeek, fromDate, toDate);
			break;

		case 1:
			Periods::Calculate(Periods::Type::CurrentWeek, fromDate, toDate);
			break;

		case 2:
			Periods::Calculate(Periods::Type::CurrentMonth, fromDate, toDate);
			break;

		case 3:
			Periods::Calculate(Periods::Type::PreviousMonth, fromDate, toDate);
			break;

		case 4:
			Periods::Calculate(Periods::Type::CurrentYear, fromDate, toDate);
			break;

		case 5:
			Periods::Calculate(Periods::Type::PreviousYear, fromDate, toDate);
			break;

		default:
			fromDate = _periodFromDate;
			toDate = _periodToDate;

			_fromDatePicker->Enable();
			_toDatePicker->Enable();
			break;
		}
	}

	_fromDatePicker->SetValue(fromDate);
	_toDatePicker->SetValue(toDate);
}

void PeriodFilterPanel::OnPeriodSelect(wxCommandEvent& event) {
	CalculatePeriod();

	if (OnChange) {
		OnChange();
	}
}

void PeriodFilterPanel::OnDateChanged(wxDateEvent& event) {
	if (OnChange) {
		OnChange();
	}
}