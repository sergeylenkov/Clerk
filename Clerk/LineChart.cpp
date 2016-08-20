#include "LineChart.h"

LineChart::LineChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(LineChart::OnPaint));
	this->Bind(wxEVT_MOTION, &LineChart::OnMouseMove, this);
	this->Bind(wxEVT_ENTER_WINDOW, &LineChart::OnMouseEnter, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &LineChart::OnMouseExit, this);
}

LineChart::~LineChart() {

}

void LineChart::SetValues(std::vector<DateValue> values) {
	_values = values;
	Draw();
}

void LineChart::Draw() {
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
		[](const DateValue p1, const DateValue p2) {
		return p1.value < p2.value;
	});

	float maxValue = ptr->value;
	int maxX = round(maxValue);

	if (maxX <= 1000) {
		maxX = ceil(maxValue / 100) * 100;
	}
	else if (maxX > 1000 && maxX <= 10000) {
		maxX = ceil(maxValue / 1000) * 1000;
	}
	else if (maxX > 10000 && maxX <= 100000) {
		maxX = ceil(maxValue / 10000) * 10000;
	} else if (maxX > 10000 && maxX <= 100000) {
		maxX = ceil(maxValue / 100000) * 100000;
	}
	else {
		maxX = ceil(maxValue / 100000) * 100000;
	}

	int offsetX = 100;
	int offsetY = 20;

	float stepX = (width - offsetX) / _values.size();
	float stepY = (height - offsetY) / (float)maxX;
	wxLogDebug("max - %f %d", maxValue, maxX);
	dc.SetPen(wxPen(wxColor(0, 0, 0), 0));

	dc.DrawLabel(wxString::Format("%d", maxX), wxRect(0, 0, 100, 20), 0);
	dc.DrawLabel("0", wxRect(0, height - 20, 100, 20), 0);

	dc.SetBrush(wxColor(10, 110, 170));
	dc.SetPen(wxPen(wxColor(10, 110, 170), 0));

	int x = 0;
	int y = 0;
	int x2 = 0;
	int y2 = 0;

	for (unsigned int i = 0; i < _values.size() - 1; i++) {
		x = round(i * stepX) + offsetX;
		y = round(_values[i].value * stepY);
		x2 = round((i + 1) * stepX) + offsetX;
		y2 = round(_values[i + 1].value * stepY);
		wxLogDebug("value %f %f", _values[i].value, _values[i].value * stepY);
		dc.DrawLine(x, height - y, x2, height - y2);
		dc.DrawCircle(x, height - y, 5);
	}

	dc.DrawCircle(x2, height - y2, 5);
}

void LineChart::OnPaint(wxPaintEvent& event) {
	Draw();
}

void LineChart::OnMouseMove(wxMouseEvent& event) {
	wxLogDebug("move %d %d", event.GetX(), event.GetY());
}

void LineChart::OnMouseEnter(wxMouseEvent& event) {
	wxLogDebug("enter");
}

void LineChart::OnMouseExit(wxMouseEvent& event) {
	wxLogDebug("leave");
}