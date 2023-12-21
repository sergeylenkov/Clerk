#include "DataViewProgressRender.h"

DataViewProgressRender::DataViewProgressRender(bool isReverse):
	wxDataViewCustomRenderer("string", wxDATAVIEW_CELL_INERT, wxDVR_DEFAULT_ALIGNMENT),
	_isReverse(isReverse)
{
}

bool DataViewProgressRender::Render(wxRect rect, wxDC *dc, int state)
{
	int x = rect.GetX() + 5;
	int width = rect.GetWidth() - 10;

	int percentWidth = (width / 100.0) * _value;
	int y = (rect.GetHeight() - 4) / 2;

	dc->SetPen(wxPen(wxColor(216, 216, 216), 1));
	dc->SetBrush(wxBrush(wxColor(216, 216, 216)));

	dc->DrawRectangle(x, rect.GetY() + y, width, 4);

	wxColor color = Colors::ColorForProgress(_value, _isReverse);

	dc->SetPen(wxPen(color, 1));
	dc->SetBrush(wxBrush(color));

	dc->DrawRectangle(x, rect.GetY() + y, percentWidth, 4);

	return true;
}

wxSize DataViewProgressRender::GetSize() const
{
	return wxDefaultSize;
}

bool DataViewProgressRender::SetValue(const wxVariant &value)
{
	_value = value.GetDouble();

	if (_value > 100) {
		_value = 100;
	}
	else if (_value < 1) {
		_value = 1;
	}

	return true;
}

bool DataViewProgressRender::GetValue(wxVariant &WXUNUSED(value)) const {
	return true;
}