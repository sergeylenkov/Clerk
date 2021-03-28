#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>

class Statusbar : public wxPanel
{
public:
	Statusbar(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
	void SetPeriod(wxDateTime& value);
	void SetReceipts(float value);
	void SetExpenses(float value);
	void SetBalance(float value);
	void SetExchangeRates(wxString value);

private:
	wxStaticText *periodLabel;
	wxStaticText *receiptsLabel;
	wxStaticText *expensesLabel;
	wxStaticText *balanceLabel;
	wxStaticText *exchangeRatesLabel;
};