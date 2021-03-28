#include "GoalsProgressRender.h"

GoalsProgressRender::GoalsProgressRender() : wxDataViewCustomRenderer("string", wxDATAVIEW_CELL_INERT, wxDVR_DEFAULT_ALIGNMENT)
{
}

GoalsProgressRender::~GoalsProgressRender()
{
}

bool GoalsProgressRender::Render(wxRect rect, wxDC *dc, int state)
{
	int percentWidth = (rect.GetWidth() / 100.0) * _value;
	int y = (rect.GetHeight() - 4) / 2;

	dc->SetPen(wxPen(wxColor(216, 216, 216), 1));
	dc->SetBrush(wxBrush(wxColor(216, 216, 216)));

	dc->DrawRectangle(rect.GetX(), rect.GetY() + y, rect.GetWidth(), 4);

	wxColor color = Colors::ColorForGoal(_value);

	dc->SetPen(wxPen(color, 1));
	dc->SetBrush(wxBrush(color));

	dc->DrawRectangle(rect.GetX(), rect.GetY() + y, percentWidth, 4);

	return true;
}

wxSize GoalsProgressRender::GetSize() const
{
	return wxDefaultSize;
}

bool GoalsProgressRender::SetValue(const wxVariant &value)
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

bool GoalsProgressRender::GetValue(wxVariant &WXUNUSED(value)) const
{
	return true;
}