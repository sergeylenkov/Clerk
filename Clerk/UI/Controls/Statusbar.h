#pragma once

#include <wx/wx.h>

class Statusbar : public wxPanel
{
public:
	Statusbar(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	void SetPeriod(wxString value);
	void SetRecepipts(wxString value);
	void SetExpenses(wxString value);
	void SetBalance(wxString value);
	void SetExchangeRates(wxString value);

private:
	wxStaticText *periodLabel;
	wxStaticText *receiptsLabel;
	wxStaticText *expensesLabel;
	wxStaticText *balanceLabel;
	wxStaticText *exchangeRatesLabel;
};