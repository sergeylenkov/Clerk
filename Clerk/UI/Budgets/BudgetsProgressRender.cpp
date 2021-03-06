#include "BudgetsProgressRender.h"

BudgetsProgressRender::BudgetsProgressRender() : wxDataViewCustomRenderer("string", wxDATAVIEW_CELL_INERT, wxDVR_DEFAULT_ALIGNMENT)
{
}

BudgetsProgressRender::~BudgetsProgressRender()
{
}

bool BudgetsProgressRender::Render(wxRect rect, wxDC *dc, int state)
{
	int percentWidth = (rect.GetWidth() / 100.0) * _value;
	int y = (rect.GetHeight() - 4) / 2;

	dc->SetPen(wxPen(wxColor(216, 216, 216), 1));
	dc->SetBrush(wxBrush(wxColor(216, 216, 216)));

	dc->DrawRectangle(rect.GetX(), rect.GetY() + y, rect.GetWidth(), 4);

	wxColor color = Colors::ColorForBudget(_value);

	dc->SetPen(wxPen(color, 1));
	dc->SetBrush(wxBrush(color));

	dc->DrawRectangle(rect.GetX(), rect.GetY() + y, percentWidth, 4);

	return true;
}

wxSize BudgetsProgressRender::GetSize() const
{
	return wxDefaultSize;
}

bool BudgetsProgressRender::SetValue(const wxVariant &value)
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

bool BudgetsProgressRender::GetValue(wxVariant &WXUNUSED(value)) const
{
	return true;
}