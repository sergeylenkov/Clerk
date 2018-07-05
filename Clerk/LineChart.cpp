#include "LineChart.h"

LineChart::LineChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	currentPopupIndex = -1;

	this->Connect(wxEVT_PAINT, wxPaintEventHandler(LineChart::OnPaint));
	this->Bind(wxEVT_MOTION, &LineChart::OnMouseMove, this);
	this->Bind(wxEVT_ENTER_WINDOW, &LineChart::OnMouseEnter, this);
	this->Bind(wxEVT_LEAVE_WINDOW, &LineChart::OnMouseExit, this);
}

LineChart::~LineChart() {

}

void LineChart::SetValues(std::vector<DateValue> values) {
	currentPopupIndex = -1;
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

	offsetX = 100;
	offsetY = 20;

	stepX = (width - offsetX) / _values.size();
	stepY = (height - offsetY) / (float)maxX;

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

		dc.DrawLine(x, height - y, x2, height - y2);
		dc.DrawCircle(x, height - y, 3);
	}

	dc.DrawCircle(x2, height - y2, 3);
}

void LineChart::OnPaint(wxPaintEvent& event) {
	Draw();
}

void LineChart::OnMouseMove(wxMouseEvent& event) {
	if (event.GetX() < offsetX) {
		if (OnHidePopup) {
			OnHidePopup();
		}

		return;
	}

	if (OnShowPopup) {
		OnShowPopup();
	}

	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	int mouseX = event.GetX();
	int index = 0;
	
	for (unsigned int i = 0; i < _values.size(); i++) {
		int x = round(i * stepX) + offsetX;

		if (mouseX > x - (stepX / 2) && mouseX < x + (stepX / 2)) {
			index = i;
			break;
		}
	}

	if (index < 0) {
		index = 0;
	}
	else if (index >= _values.size()) {
		index = _values.size() - 1;
	}

	if (index != currentPopupIndex) {
		currentPopupIndex = index;
		int x = round(index * stepX) + offsetX;
		int y = height - round(_values[index].value * stepY);

		if (OnUpdatePopup) {
			OnUpdatePopup(x, y, index);
		}
	}	
}

void LineChart::OnMouseEnter(wxMouseEvent& event) {
	if (OnShowPopup) {
		//OnShowPopup();
	}
}

void LineChart::OnMouseExit(wxMouseEvent& event) {
	if (OnHidePopup) {
		OnHidePopup();
	}
}