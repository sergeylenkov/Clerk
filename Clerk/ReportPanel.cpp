#include "ReportPanel.h"

ReportPanel::ReportPanel(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	//wxPanel *panel = new wxPanel(this, wxID_ANY);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	
	chart = new BarChart(parent, wxID_ANY);
	chart->SetSize(wxSize(400, 200));

	sizer->Add(chart);

	this->SetSizer(sizer);
	this->Layout();

	this->SetBackgroundColour(*(wxColor *)wxWHITE);
}

ReportPanel::~ReportPanel() {
		
}

void ReportPanel::Update() {
	map<wxString, float> values = DataHelper::GetInstance().GetExpensesByMonth();

	chart->SetValues(values);
	chart->Draw();
}