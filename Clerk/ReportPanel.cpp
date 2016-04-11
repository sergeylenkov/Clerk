#include "ReportPanel.h"

ReportPanel::ReportPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {		
	chart = new BarChart(this, wxID_ANY);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(chart, 1, wxEXPAND);

	this->SetSizer(sizer);
	this->Layout();

	this->SetBackgroundColour(*(wxColor *)wxWHITE);
}

ReportPanel::~ReportPanel() {
		
}

void ReportPanel::Update() {
	chart->SetValues(DataHelper::GetInstance().GetExpensesByMonth());
}