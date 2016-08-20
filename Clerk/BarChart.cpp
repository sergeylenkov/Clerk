#include "BarChart.h"

BarChart::BarChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(BarChart::OnPaint));
}

BarChart::~BarChart() {

}

void BarChart::SetValues(map<wxString, float> values) {
	_values = values;
	Draw();
}

void BarChart::Draw() {
	if (_values.size() == 0) {
		return;
	}

	wxClientDC dc(this);
	
	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	auto ptr = max_element(_values.begin(), _values.end(),
		[](const pair<wxString, float>& p1, const pair<wxString, float>& p2) {
			return p1.second < p2.second; 
		});

	int offsetX = 100;
	int offsetY = 20;

	int stepX = (width - offsetX) / _values.size();
	float stepY = (height - offsetY) / ptr->second;	

	dc.SetPen(wxPen(wxColor(0, 0, 0), 0));

	dc.DrawLabel("100", wxRect(0, 0, 100, 20), 0);

	dc.SetBrush(wxColor(10, 110, 170));
	dc.SetPen(wxPen(wxColor(10, 110, 170), 0));

	int i = 0;

	for (auto it = _values.begin(); it != _values.end(); it++) {
		int x = round(i * stepX) + offsetX;
		int y = round(it->second * stepY);

		dc.DrawRectangle(x, height - y, stepX - 1, height);
		i++;
	}
}

void BarChart::OnPaint(wxPaintEvent& event) {
	Draw();
}