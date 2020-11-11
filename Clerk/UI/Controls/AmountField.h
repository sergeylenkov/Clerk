#pragma once

#include <wx/wx.h>
#include <wx/tokenzr.h>

class AmountField: public wxTextCtrl
{
public:
	AmountField(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	~AmountField();

	float GetFloatValue();

private:
	wxString ClearAmountValue(wxString& value);
	float GetValueFromString(wxString& value);
};

