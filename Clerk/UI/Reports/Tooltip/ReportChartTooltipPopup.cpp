#include "ReportChartTooltipPopup.h"

ReportChartTooltipPopup::ReportChartTooltipPopup(wxWindow* parent) : wxPopupWindow(parent) {
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	_panel = new ReportChartTooltipPanel(this);
	mainSizer->Add(_panel, 1, wxEXPAND, 0);

	SetSizer(mainSizer);
	Layout();
}

void ReportChartTooltipPopup::Update(wxString title, float total, std::vector<StringValueViewModel> values) {
	_panel->SetTotal(total);
	_panel->SetTitle(title);
	_panel->SetValues(values);

	_panel->Update();

	SetClientSize(_panel->GetMinSize());
}