#include "LineChart.h"

LineChart::LineChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	currentPopupIndex = -1;

	this->Bind(wxEVT_PAINT, &LineChart::OnPaint, this);
	this->Bind(wxEVT_MOTION, &LineChart::OnMouseMove, this);	
}

void LineChart::SetValues(std::vector<StringValue> values) {
	currentPopupIndex = -1;
	this->values = values;

	auto ptr = max_element(values.begin(), values.end(),
		[](const StringValue p1, const StringValue p2) {
		return p1.value < p2.value;
	});

	maxValue = ptr->value;

	Draw();
}

void LineChart::Draw() {
	wxClientDC dc(this);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	if (values.size() == 0) {
		return;
	}

	width = 0;
	height = 0;

	this->DoGetSize(&width, &height);
		
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

	wxSize size = dc.GetTextExtent(values[values.size() - 1].string);

	graphWidth = width - offsetX - (size.GetWidth() + 20);

	stepX = 0;
	stepY = (height - offsetY - 10) / (float)maxY;
	
	if (values.size() > 1) {
		stepX = graphWidth / (values.size() - 1);
	}

	for (int i = 0; i <= maxY; i += labelStepY) {
		int y = (height - offsetY) - round(i * stepY);

		wxString label = wxNumberFormatter::ToString((float)i, 0);
		wxSize size = dc.GetTextExtent(label);

		int x = maxSize.GetWidth() - size.GetWidth();

		dc.SetPen(wxPen(wxColor(0, 0, 0), 0));
		dc.DrawText(label, wxPoint(x, y - 10));

		dc.SetPen(wxPen(wxColor(240, 240, 240), 0));
		dc.DrawLine(offsetX, y, offsetX + graphWidth, y);
	}
	
	if (values.size() > 1) {
		for (unsigned int i = 0; i < values.size(); i++) {
			int x = round(i * stepX) + offsetX;

			wxSize size = dc.GetTextExtent(values[i].string);
			dc.DrawText(values[i].string, wxPoint(x - (size.GetWidth() / 2), height - 20));
		}
	}
	else {
		int x = offsetX + (graphWidth / 2);

		wxSize size = dc.GetTextExtent(values[0].string);
		dc.DrawText(values[0].string, wxPoint(x - (size.GetWidth() / 2), height - 20));
	}

	DrawGraph();
}

void LineChart::DrawGraph() {
	wxClientDC dc(this);

	dc.SetBrush(wxColor(10, 110, 170));
	dc.SetPen(wxPen(wxColor(10, 110, 170), 0));

	points.clear();

	if (values.size() > 1) {
		int x = 0;
		int y = 0;
		int x2 = 0;
		int y2 = 0;

		for (unsigned int i = 0; i < values.size() - 1; i++) {
			x = offsetX + (i * stepX);
			y = (height - offsetY) - round(values[i].value * stepY);
			x2 = offsetX + ((i + 1) * stepX);
			y2 = (height - offsetY) - round(values[i + 1].value * stepY);

			dc.DrawLine(x, y, x2, y2);
			dc.DrawCircle(x, y, 3);

			points.push_back(make_pair(x - (stepX / 2), x + (stepX / 2)));
		}

		dc.DrawCircle(x2, y2, 3);

		points.push_back(make_pair(x2 - (stepX / 2), x2 + (stepX / 2)));
	}
	else {
		int x = offsetX + (graphWidth / 2);
		int y = (height - offsetY) - round(values[0].value * stepY);

		dc.DrawCircle(x, y, 3);

		points.push_back(make_pair(x - 20, x + 20));
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

	if (mouseX < offsetX || mouseX > offsetX + graphWidth || mouseY < offsetY || mouseY > height - offsetY) {
		if (OnHidePopup) {
			OnHidePopup();
		}

		return;
	}

	if (OnShowPopup) {
		OnShowPopup();
	}
	
	unsigned int index = 0;
	
	for (unsigned int i = 0; i < points.size(); i++) {
		int x = points[i].first;
		int x2 = points[i].second;

		if (mouseX >= x && mouseX <= x2) {
			index = i;
			break;
		}
	}

	if (index < 0) {
		index = 0;
	}
	else if (index >= values.size()) {
		index = values.size() - 1;
	}

	if (index != currentPopupIndex) {
		currentPopupIndex = index;
		int x = offsetX + (index * stepX);
		int y = height - offsetY - round(values[index].value * stepY);

		if (OnUpdatePopup) {
			OnUpdatePopup(x, y, index);
		}
	}	
}