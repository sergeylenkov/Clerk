#include "BarChart.h"

BarChart::BarChart(wxWindow *parent) : wxPanel(parent, wxID_ANY) {
	Bind(wxEVT_PAINT, &BarChart::OnPaint, this);
}

BarChart::~BarChart() {

}

void BarChart::SetValues(std::vector<StringValueViewModel> values) {
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

	DoGetSize(&width, &height);

	auto ptr = max_element(_values.begin(), _values.end(),
		[](const StringValueViewModel p1, const StringValueViewModel p2) {
		return p1.value < p2.value;
	});

	float maxValue = ptr->value;
	int maxX = round(maxValue);
	int labelStepX = 10;

	if (maxX <= 1000) {
		maxX = ceil(maxValue / 100) * 100;
		labelStepX = 100;
	}
	else if (maxX > 1000 && maxX <= 10000) {
		maxX = ceil(maxValue / 1000) * 1000;
		labelStepX = 1000;
	}
	else if (maxX > 10000 && maxX <= 100000) {
		maxX = ceil(maxValue / 10000) * 10000;
		labelStepX = 10000;
	}
	else if (maxX > 10000 && maxX <= 100000) {
		maxX = ceil(maxValue / 100000) * 100000;
		labelStepX = 20000;
	}
	else if (maxX > 100000 && maxX <= 1000000) {
		maxX = ceil(maxValue / 100000) * 100000;
		labelStepX = 50000;
	}
	else {
		maxX = ceil(maxValue / 100000) * 100000;
		labelStepX = 100000;
	}

	wxString maxValueString = wxNumberFormatter::ToString(maxValue, 2);
	wxSize maxSize = dc.GetTextExtent(maxValueString);

	int offsetY = FromDIP(20);
	float stepY = (height - offsetY) / _values.size();	

	int maxLabelWidth = 0;

	for (unsigned int i = 0; i < _values.size(); i++) {
		wxSize size = dc.GetTextExtent(_values[i].string);

		if (size.GetWidth() > maxLabelWidth) {
			maxLabelWidth = size.GetWidth();
		}
	}

	int offsetX = maxLabelWidth + FromDIP(10);
	int barMaxWidth = width - offsetX;
	float stepX = barMaxWidth / (float)maxX;

	dc.SetPen(wxPen(wxColor(240, 240, 240), 0));
	dc.DrawLine(offsetX, height - offsetY, width, height - offsetY);

	for (int i = 0; i <= maxX; i += labelStepX) {
		wxString label = wxNumberFormatter::ToString((float)i, 0);
		wxSize size = dc.GetTextExtent(label);

		int x = offsetX + round(i * stepX);
		int labelX = x - size.GetWidth() / 2;
		int y = height - size.GetHeight();

		if (labelX > width - size.GetWidth()) {
			labelX = width - size.GetWidth();
		}

		dc.SetPen(wxPen(wxColor(0, 0, 0), 0));
		dc.DrawText(label, wxPoint(labelX, y));

		if (x >= width) {
			x = width - 1;
		}

		dc.SetPen(wxPen(wxColor(240, 240, 240), 0));
		dc.DrawLine(x, 0, x, height - offsetY);
	}

	int y = offsetY;
	int barSize = FromDIP(10);

	for (unsigned int i = 0; i < _values.size(); i++) {
		wxSize size = dc.GetTextExtent(_values[i].string);

		int x = maxLabelWidth - size.GetWidth();

		dc.SetPen(wxPen(wxColor(0, 0, 0), 0));
		dc.DrawText(_values[i].string, wxPoint(x, y - (size.GetHeight() / 2) - 2));

		wxColor color = Colors::ColorForBarIndex(0);

		dc.SetBrush(color);
		dc.SetPen(wxPen(color, 0));

		int barWidth = round(_values[i].value * stepX);

		if (barWidth < 5) {
			barWidth = 5;
		}

		dc.DrawRectangle(offsetX, y - (barSize / 2), barWidth, barSize);

		y = y + stepY;
	}
}

void BarChart::OnPaint(wxPaintEvent& event) {
	Draw();
}