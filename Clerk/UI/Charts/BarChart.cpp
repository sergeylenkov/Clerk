#include "BarChart.h"

BarChart::BarChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	this->Connect(wxEVT_PAINT, wxPaintEventHandler(BarChart::OnPaint));
}

BarChart::~BarChart() {

}

void BarChart::SetValues(std::vector<StringValue> values) {
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
		[](const StringValue p1, const StringValue p2) {
		return p1.value < p2.value;
	});

	float maxValue = ptr->value;
	int maxY = round(maxValue);
	int labelStepX = 10;

	if (maxY <= 1000) {
		maxY = ceil(maxValue / 100) * 100;
		labelStepX = 100;
	}
	else if (maxY > 1000 && maxY <= 10000) {
		maxY = ceil(maxValue / 1000) * 1000;
		labelStepX = 1000;
	}
	else if (maxY > 10000 && maxY <= 100000) {
		maxY = ceil(maxValue / 10000) * 10000;
		labelStepX = 10000;
	}
	else if (maxY > 10000 && maxY <= 100000) {
		maxY = ceil(maxValue / 100000) * 100000;
		labelStepX = 20000;
	}
	else if (maxY > 100000 && maxY <= 1000000) {
		maxY = ceil(maxValue / 100000) * 100000;
		labelStepX = 50000;
	}
	else {
		maxY = ceil(maxValue / 100000) * 100000;
		labelStepX = 100000;
	}

	wxString maxValueString = wxNumberFormatter::ToString(maxValue, 2);
	wxSize maxSize = dc.GetTextExtent(maxValueString);

	//offsetX = maxSize.GetWidth() + 20;
	offsetY = 40;
	
	stepY = (height - offsetY) / _values.size(); //(height - offsetY - 10) / (float)maxY;

	int maxLabelWidth = 0;

	for (unsigned int i = 0; i < _values.size(); i++) {
		wxSize size = dc.GetTextExtent(_values[i].string);

		if (size.GetWidth() > maxLabelWidth) {
			maxLabelWidth = size.GetWidth();
		}
	}

	offsetX = maxLabelWidth;

	int barOffset = offsetX + 20;
	int barMaxWidth = width - barOffset;

	stepX = barMaxWidth / (float)maxY;

	dc.SetPen(wxPen(wxColor(240, 240, 240), 0));
	dc.DrawLine(barOffset, height - offsetY, width, height - offsetY);

	for (int i = 0; i <= maxY; i += labelStepX) {
		wxString label = wxNumberFormatter::ToString((float)i, 0);
		wxSize size = dc.GetTextExtent(label);

		int x = barOffset + round(i * stepX);
		x = x - size.GetWidth() / 2;

		if (x > barMaxWidth) {
			x = width - size.GetWidth();
		}

		int y = (height - offsetY) + 10;

		dc.SetPen(wxPen(wxColor(0, 0, 0), 0));
		dc.DrawText(label, wxPoint(x, y));

		x = barOffset + round(i * stepX);

		if (x >= width) {
			x = width - 1;
		}

		dc.SetPen(wxPen(wxColor(240, 240, 240), 0));
		dc.DrawLine(x, 0, x, height - offsetY);
	}	

	int y = offsetY;	

	for (unsigned int i = 0; i < _values.size(); i++) {
		wxSize size = dc.GetTextExtent(_values[i].string);

		int x = maxLabelWidth - size.GetWidth();

		dc.SetPen(wxPen(wxColor(0, 0, 0), 0));
		dc.DrawText(_values[i].string, wxPoint(x, y - 10));

		dc.SetBrush(wxColor(10, 110, 170));
		dc.SetPen(wxPen(wxColor(10, 110, 170), 0));

		int barWidth = round(_values[i].value * stepX);

		if (barWidth < 10) {
			barWidth = 10;
		}

		dc.DrawRectangle(barOffset, y - 10, barWidth, 10);

		y = y + stepY;
	}
}

void BarChart::OnPaint(wxPaintEvent& event) {
	Draw();
}