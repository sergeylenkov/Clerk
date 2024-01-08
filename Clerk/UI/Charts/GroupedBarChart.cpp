#include "GroupedBarChart.h"

GroupedBarChart::GroupedBarChart(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
	_currentPopupIndex = -1;
	_width = 0;
	_height = 0;
	_graphWidth = 0;
	_barGraphWidth = 0;
	_totalValue = 0.0f;
	_maxValue = 0;
	_stepX = 0;
	_stepY = 0;
	_barStepX = 0;
	_offsetX = 0;
	_offsetY = 0;
	_barOffsetX = 0;
	_barSize = FromDIP(10);
	_barOffset = FromDIP(5);

	Bind(wxEVT_PAINT, &GroupedBarChart::OnPaint, this);
	Bind(wxEVT_MOTION, &GroupedBarChart::OnMouseMove, this);
}

void GroupedBarChart::SetValues(std::vector<StringValuesViewModel> values) {
	_currentPopupIndex = -1;
	_values = values;

	_maxValue = 0;
	_totalValue = 0;

	if (_values.size() > 0) {
		for (auto& value : _values) {
			float max = 0;

			for (float val : value.values) {
				if (val > _maxValue) {
					_maxValue = val;
				}

				if (val > max) {
					max = val;
				}
			}

			_totalValue = _totalValue + max;
		}
	}

	Draw();
}

void GroupedBarChart::Draw() {
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
	}
	else if (maxY > 10000 && maxY <= 100000) {
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

	_offsetX = maxSize.GetWidth() + FromDIP(20);
	_offsetY = FromDIP(40);

	wxSize size = dc.GetTextExtent(_values[_values.size() - 1].string);

	_graphWidth = _width - _offsetX - (size.GetWidth() + FromDIP(20));

	_stepX = 0;
	_stepY = (_height - _offsetY - FromDIP(10)) / (float)maxY;

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
		_barOffsetX = (_values[0].values.size() * (_barSize + _barOffset)) / 2;
		_barOffsetX = _barOffsetX - _barOffset;
		_barGraphWidth = _graphWidth - (_barOffsetX * 2);
		_barStepX = (float)_barGraphWidth / (_values.size() - 1);

		for (unsigned int i = 0; i < _values.size(); i++) {
			int x = round(i * _barStepX) + _barOffsetX + _offsetX;

			wxSize size = dc.GetTextExtent(_values[i].string);
			dc.DrawText(_values[i].string, wxPoint(x - (size.GetWidth() / 2), _height - FromDIP(20)));
		}
	}
	else {
		int x = _offsetX + (_graphWidth / 2);

		wxSize size = dc.GetTextExtent(_values[0].string);
		dc.DrawText(_values[0].string, wxPoint(x - (size.GetWidth() / 2), _height - FromDIP(20)));
	}

	DrawGraph();
}

void GroupedBarChart::DrawGraph() {
	wxClientDC dc(this);

	dc.SetBrush(wxColor(10, 110, 170));
	dc.SetPen(wxPen(wxColor(10, 110, 170), 0));

	_points.clear();

	if (_values.size() > 1) {
		for (unsigned int i = 0; i < _values.size(); i++) {
			int x = round(i * _barStepX) + _barOffsetX + _offsetX;
			
			DrawBars(dc, _values[i].values, x);
		}
	}
	else {
		int x = _offsetX + (_graphWidth / 2);

		DrawBars(dc, _values[0].values, x);
	}
}

void GroupedBarChart::DrawBars(wxClientDC& dc, std::vector<float> values, int x) {
	int pointX = x;
	int pointY = _height;	

	for (unsigned int i = 0; i < values.size(); i++) {		
		int height = round(values[i] * _stepY);

		if (height < 5) {
			height = 5;
		}

		int y = (_height - _offsetY) - height;		

		if (y < pointY) {
			pointY = y;
		}

		wxColor color = Colors::ColorForBarIndex(i);

		dc.SetBrush(color);
		dc.SetPen(wxPen(color, 0));

		dc.DrawRectangle((x - _barOffsetX), y, _barSize, height);

		x = x + _barSize + _barOffset;
	}

	_points.push_back({ pointX, pointY });
}

void GroupedBarChart::OnPaint(wxPaintEvent& event) {
	Draw();
}

void GroupedBarChart::OnMouseMove(wxMouseEvent& event) {
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

	unsigned int index = -1;

	for (unsigned int i = 0; i < _points.size(); i++) {
		int x = _points[i].first;
		int y = _points[i].second;

		if (mouseX >= x - _barOffsetX && mouseX <= x + _barOffsetX
			&& mouseY >= y && mouseY <= height - _offsetY) {
			index = i;
			break;
		}
	}

	if (index < 0 || index >= _values.size()) {
		if (OnHidePopup) {
			OnHidePopup();
		}

		return;
	}

	if (OnShowPopup) {
		OnShowPopup();
	}

	if (index != _currentPopupIndex) {
		_currentPopupIndex = index;
		
		int x = _points[index].first;
		int y = _points[index].second;

		if (OnUpdatePopup) {
			OnUpdatePopup(x, y, index);
		}
	}
}