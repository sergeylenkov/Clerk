#include "TransactionsAmountRender.h"

TransactionsAmountRender::TransactionsAmountRender() : wxDataViewCustomRenderer("arrstring", wxDATAVIEW_CELL_INERT, wxDVR_DEFAULT_ALIGNMENT)
{
}

TransactionsAmountRender::~TransactionsAmountRender()
{
}

bool TransactionsAmountRender::Render(wxRect rect, wxDC *dc, int state)
{
	wxString text = _value[0];

	wxSize size = dc->GetTextExtent(text);
	int offset = (rect.GetHeight() - size.GetHeight()) / 2;

	int x = (rect.GetX() + rect.GetWidth()) - size.GetWidth();

	dc->SetTextForeground(wxColor(0, 0, 0));
	dc->DrawText(text, wxPoint(x, rect.GetY() + offset));

	if (_value.Count() > 1) {
		text = _value[1];

		size = dc->GetTextExtent(text);

		x = x - size.GetWidth() - 10;

		dc->SetTextForeground(wxColor(100, 100, 100));
		dc->DrawText(text, wxPoint(x, rect.GetY() + offset));
	}

	return true;
}

wxSize TransactionsAmountRender::GetSize() const
{
	return wxDefaultSize;
}

bool TransactionsAmountRender::SetValue(const wxVariant &value)
{
	_value = value.GetArrayString();

	return true;
}

bool TransactionsAmountRender::GetValue(wxVariant &WXUNUSED(value)) const
{
	return true;
}