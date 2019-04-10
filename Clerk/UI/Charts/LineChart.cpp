#include "LineChart.h"

LineChart::LineChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	currentPopupIndex = -1;

	this->Bind(wxEVT_PAINT, &LineChart::OnPaint, this);
	this->Bind(wxEVT_MOTION, &LineChart::OnMouseMove, this);	
}

LineChart::~LineChart() {

}

void LineChart::SetValues(std::vector<StringValue> values) {
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
		[](const StringValue p1, const StringValue p2) {
		return p1.value < p2.value;
	});

	float maxValue = ptr->value;
	int maxY = round(maxValue);
	int labelStepY = 10;

	if (maxY <= 1000) {
		maxY = ceil(maxValue / 100) * 100;
		labelStepY = 100;
	}
	else if (maxY > 1000 && maxY <= 10000) {
		maxY = ceil(maxValue / 1000) * 1000;
		labelStepY = 1000;
	}
	else if (maxY > 10000 && maxY <= 100000) {
		maxY = ceil(maxValue / 10000) * 10000;
		labelStepY = 10000;
	} else if (maxY > 10000 && maxY <= 100000) {
		maxY = ceil(maxValue / 100000) * 100000;
		labelStepY = 20000;
	}
	else if (maxY > 100000 && maxY <= 1000000) {
		maxY = ceil(maxValue / 100000) * 100000;
		labelStepY = 50000;
	}
	else {
		maxY = ceil(maxValue / 100000) * 100000;
		labelStepY = 100000;
	}

	wxString maxValueString = wxNumberFormatter::ToString(maxValue, 2);
	wxSize maxSize = dc.GetTextExtent(maxValueString);

	offsetX = maxSize.GetWidth() + 20;
	offsetY = 40;

	stepX = width / _values.size();
	stepY = (height - offsetY - 10) / (float)maxY;
	
	for (int i = 0; i <= maxY; i += labelStepY) {
		int y = (height - offsetY) - round(i * stepY);

		wxString label = wxNumberFormatter::ToString((float)i, 0);
		wxSize size = dc.GetTextExtent(label);

		int x = maxSize.GetWidth() - size.GetWidth();

		dc.SetPen(wxPen(wxColor(0, 0, 0), 0));
		dc.DrawText(label, wxPoint(x, y - 10));

		dc.SetPen(wxPen(wxColor(240, 240, 240), 0));
		dc.DrawLine(offsetX, y, width - offsetX, y);
	}
	
	if (_values.size() > 1) {
		for (unsigned int i = 0; i < _values.size(); i++) {
			int x = round(i * stepX) + offsetX;

			wxSize size = dc.GetTextExtent(_values[i].string);
			dc.DrawText(_values[i].string, wxPoint(x - (size.GetWidth() / 2), height - 20));
		}
	}
	else {
		int x = (width - offsetX) / 2;

		wxSize size = dc.GetTextExtent(_values[0].string);
		dc.DrawText(_values[0].string, wxPoint(x - (size.GetWidth() / 2), height - 20));
	}

	dc.SetBrush(wxColor(10, 110, 170));
	dc.SetPen(wxPen(wxColor(10, 110, 170), 0));

	if (_values.size() > 1) {
		int x = 0;
		int y = 0;
		int x2 = 0;
		int y2 = 0;

		for (unsigned int i = 0; i < _values.size() - 1; i++) {
			x = round(i * stepX) + offsetX;
			y = (height - offsetY) - round(_values[i].value * stepY);
			x2 = round((i + 1) * stepX) + offsetX;
			y2 = (height - offsetY) - round(_values[i + 1].value * stepY);

			dc.DrawLine(x, y, x2, y2);
			dc.DrawCircle(x, y, 3);
		}

		dc.DrawCircle(x2, y2, 3);
	}
	else {
		int x = (width - offsetX) / 2;
		int y = (height - offsetY) - round(_values[0].value * stepY);

		dc.DrawCircle(x, y, 3);
	}
}

void LineChart::OnPaint(wxPaintEvent& event) {
	Draw();
}

void LineChart::OnMouseMove(wxMouseEvent& event) {
	int width = 0;
	int height = 0;

	this->DoGetSize(&width, &height);

	int mouseX = event.GetX();
	int mouseY = event.GetY();

	if (mouseX < offsetX || mouseX > width - offsetX || mouseY < offsetY || mouseY > height - offsetY) {
		if (OnHidePopup) {
			OnHidePopup();
		}

		return;
	}

	if (OnShowPopup) {
		OnShowPopup();
	}
	
	unsigned int index = 0;
	
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
		int y = height - offsetY - round(_values[index].value * stepY);

		if (OnUpdatePopup) {
			OnUpdatePopup(x, y, index);
		}
	}	
}