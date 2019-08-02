#include "TransactionsTagsRender.h"

TransactionsTagsRender::TransactionsTagsRender() : wxDataViewCustomRenderer("arrstring", wxDATAVIEW_CELL_INERT, wxDVR_DEFAULT_ALIGNMENT)
{
}

TransactionsTagsRender::~TransactionsTagsRender()
{
}

bool TransactionsTagsRender::Render(wxRect rect, wxDC *dc, int state)
{
	int x = rect.GetX();
	int y = rect.GetY();

	wxFont font = dc->GetFont();
	font.SetPointSize(8);

	dc->SetFont(font);

	for (unsigned int i = 0; i < _value.Count(); i++) {
		wxString text = _value[i];

		wxSize size = dc->GetTextExtent(text);

		dc->SetBrush(wxBrush(wxColor(241, 248, 255)));
		dc->DrawRectangle(x, y + 1, size.GetWidth() + 10, rect.GetHeight() - 2);

		int offset = (rect.GetHeight() - size.GetHeight()) / 2;

		dc->SetTextForeground(wxColor(100, 100, 100));
		dc->DrawText(text, wxPoint(x + 5, y + offset));

		x = x + size.GetWidth() + 5;
	}

	return true;
}

wxSize TransactionsTagsRender::GetSize() const
{
	return wxDefaultSize;
}

bool TransactionsTagsRender::SetValue(const wxVariant &value)
{
	_value = value.GetArrayString();

	return true;
}

bool TransactionsTagsRender::GetValue(wxVariant &WXUNUSED(value)) const
{
	return true;
}