#include "PieChart.h"

PieChart::PieChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	Connect(wxEVT_PAINT, wxPaintEventHandler(PieChart::OnPaint));
}

PieChart::~PieChart() {

}

void PieChart::SetValues(map<wxString, float> values) {
	_values = values;
	Draw();
}

void PieChart::Draw() {
	if (_values.size() == 0) {
		return;
	}

	wxClientDC dc(this);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	int width = 0;
	int height = 0;

	DoGetSize(&width, &height);

	auto sum = accumulate(_values.begin(), _values.end(), 0.0f,
		[](const float previous, const std::pair<wxString, float>& p) { 
			return previous + p.second; 
		});

	std::vector<std::pair<wxString, float>> items;

	for (auto it = _values.begin(); it != _values.end(); it++) {
		items.push_back(std::pair<wxString, float>(it->first, it->second));
	}

	sort(items.begin(), items.end(), [](std::pair<wxString, float> const &a, std::pair<wxString, float> const &b) {
			return a.second > b.second;
		});

	float step = 360 / sum;

	int centerX = width / 2;
	int centerY = height / 2;
	int radius = min(centerX, centerY);

	int startAngle = 0;
	int color = 0;
	int y = 0;

	for (unsigned int i = 0; i < items.size(); i++) {
		int angle = round(items[i].second * step);

		dc.SetBrush(wxColor(10, 100 + color, 150 + color));
		dc.SetPen(wxPen(wxColor(10, 110, 170), 1));

		dc.DrawEllipticArc(0, 0, radius * 2, radius * 2, startAngle, startAngle + angle);

		int x = radius * 2 + 20;

		dc.DrawRectangle(x, y + 3, 10, 10);
		dc.DrawText(items[i].first, x + 20, y);

		startAngle = startAngle + angle;
		color += 15;
		y += 20;
	}
}

void PieChart::OnPaint(wxPaintEvent& event) {
	Draw();
}