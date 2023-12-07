#include "LineChart.h"

LineChart::LineChart(wxWindow *parent, wxWindowID id) : wxPanel(parent, id) {
	_currentPopupIndex = -1;
	_drawAverage = false;
	_width = 0;
	_height = 0;
	_graphWidth = 0;
	_totalValue = 0.0f;
	_maxValue = 0;
	_stepX = 0;
	_stepY = 0;
	_offsetX = 0;
	_offsetY = 0;

	Bind(wxEVT_PAINT, &LineChart::OnPaint, this);
	Bind(wxEVT_MOTION, &LineChart::OnMouseMove, this);	
}

void LineChart::SetValues(std::vector<StringValueViewModel> values) {
	_currentPopupIndex = -1;
	_values = values;

	_maxValue = 0;
	_totalValue = 0;

	if (_values.size() > 0) {
		auto ptr = max_element(_values.begin(), _values.end(),
			[](const StringValueViewModel p1, const StringValueViewModel p2) {
			return p1.value < p2.value;
		});

		_maxValue = ptr->value;
	}

	_totalValue = std::accumulate(_values.begin(), _values.end(), 0, [](float accumulator, StringValueViewModel b) {
		return accumulator + b.value;
	});

	Draw();
}

void LineChart::SetDrawAverage(bool value) {
	_drawAverage = value;
}

void LineChart::Draw() {
	wxClientDC dc(this);

	dc.SetBackground(wxColor(255, 255, 255));
	dc.Clear();

	if (_values.size() == 0) {
		return;
	}

	_width = 0;
	_height = 0;

	DoGetSize(&_width, &_height);
		
	int maxY = round(_maxValue);
	int labelStepY = 10;

	if (maxY <= 1000) {
		maxY = ceil(_maxValue / 100) * 100;
		labelStepY = 100;
	}
	else if (maxY > 1000 && maxY <= 10000) {
		maxY = ceil(_maxValue / 1000) * 1000;
		labelStepY = 1000;
	}
	else if (maxY > 10000 && maxY <= 100000) {
		maxY = ceil(_maxValue / 10000) * 10000;
		labelStepY = 10000;
	} else if (maxY > 10000 && maxY <= 100000) {
		maxY = ceil(_maxValue / 100000) * 100000;
		labelStepY = 20000;
	}
	else if (maxY > 100000 && maxY <= 1000000) {
		maxY = ceil(_maxValue / 100000) * 100000;
		labelStepY = 50000;
	}
	else {
		maxY = ceil(_maxValue / 100000) * 100000;
		labelStepY = 100000;
	}

	wxString maxValueString = wxNumberFormatter::ToString(_maxValue, 2);
	wxSize maxSize = dc.GetTextExtent(maxValueString);

	_offsetX = maxSize.GetWidth() + 20;
	_offsetY = 40;

	wxSize size = dc.GetTextExtent(_values[_values.size() - 1].string);

	_graphWidth = _width - _offsetX - (size.GetWidth() + 20);

	_stepX = 0;
	_stepY = (_height - _offsetY - 10) / (float)maxY;
	
	if (_values.size() > 1) {
		_stepX = (float)_graphWidth / (_values.size() - 1);
	}

	dc.SetTextForeground(wxColor(0, 0, 0));
	dc.SetPen(wxPen(wxColor(240, 240, 240), 0));

	for (int i = 0; i <= maxY; i += labelStepY) {
		int y = (_height - _offsetY) - round(i * _stepY);

		wxString label = wxNumberFormatter::ToString((float)i, 0);
		wxSize size = dc.GetTextExtent(label);

		int x = maxSize.GetWidth() - size.GetWidth();
		
		dc.DrawText(label, wxPoint(x, y - 10));		
		dc.DrawLine(_offsetX, y, _offsetX + _graphWidth, y);
	}
	
	if (_values.size() > 1) {
		for (unsigned int i = 0; i < _values.size(); i++) {
			int x = round(i * _stepX) + _offsetX;

			wxSize size = dc.GetTextExtent(_values[i].string);
			dc.DrawText(_values[i].string, wxPoint(x - (size.GetWidth() / 2), _height - 20));
		}
	}
	else {
		int x = _offsetX + (_graphWidth / 2);

		wxSize size = dc.GetTextExtent(_values[0].string);
		dc.DrawText(_values[0].string, wxPoint(x - (size.GetWidth() / 2), _height - 20));
	}

	if (_drawAverage) {
		float average = _totalValue / _values.size();

		int y = (_height - _offsetY) - round(average * _stepY);

		wxString label = wxNumberFormatter::ToString(average, 0);
		wxSize size = dc.GetTextExtent(label);

		int x = maxSize.GetWidth() - size.GetWidth();

		dc.SetTextForeground(wxColor(40, 167, 70));
		dc.DrawText(label, wxPoint(x, y - 10));

		dc.SetPen(wxPen(wxColor(40, 167, 70), 0));
		dc.DrawLine(_offsetX, y, _offsetX + _graphWidth, y);
	}

	DrawGraph();
}

void LineChart::DrawGraph() {
	wxClientDC dc(this);

	dc.SetBrush(wxColor(10, 110, 170));
	dc.SetPen(wxPen(wxColor(10, 110, 170), 0));

	_points.clear();

	if (_values.size() > 1) {
		int x = 0;
		int y = 0;
		int x2 = 0;
		int y2 = 0;

		for (unsigned int i = 0; i < _values.size() - 1; i++) {
			x = _offsetX + (i * _stepX);
			y = (_height - _offsetY) - round(_values[i].value * _stepY);
			x2 = _offsetX + ((i + 1) * _stepX);
			y2 = (_height - _offsetY) - round(_values[i + 1].value * _stepY);

			dc.DrawLine(x, y, x2, y2);
			dc.DrawCircle(x, y, 3);

			_points.push_back(std::make_pair(x - (_stepX / 2), x + (_stepX / 2)));
		}

		dc.DrawCircle(x2, y2, 3);

		_points.push_back(std::make_pair(x2 - (_stepX / 2), x2 + (_stepX / 2)));
	}
	else {
		int x = _offsetX + (_graphWidth / 2);
		int y = (_height - _offsetY) - round(_values[0].value * _stepY);

		dc.DrawCircle(x, y, 3);

		_points.push_back(std::make_pair(x - 20, x + 20));
	}
}

void LineChart::OnPaint(wxPaintEvent& event) {
	Draw();
}

void LineChart::OnMouseMove(wxMouseEvent& event) {
	int width = 0;
	int height = 0;

	DoGetSize(&width, &height);

	int mouseX = event.GetX();
	int mouseY = event.GetY();

	if (mouseX < _offsetX || mouseX > _offsetX + _graphWidth || mouseY < _offsetY || mouseY > height - _offsetY) {
		if (OnHidePopup) {
			OnHidePopup();
		}

		return;
	}

	if (OnShowPopup) {
		OnShowPopup();
	}
	
	unsigned int index = 0;
	
	for (unsigned int i = 0; i < _points.size(); i++) {
		int x = _points[i].first;
		int x2 = _points[i].second;

		if (mouseX >= x && mouseX <= x2) {
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

	if (index != _currentPopupIndex) {
		_currentPopupIndex = index;
		int x = _offsetX + (index * _stepX);
		int y = height - _offsetY - round(_values[index].value * _stepY);

		if (OnUpdatePopup) {
			OnUpdatePopup(x, y, index);
		}
	}	
}