#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data//DataHelper.h"
#include "../../Utils/Utils.h"

class DashboardBalancePanel : public wxPanel
{
public:
	DashboardBalancePanel(wxWindow *parent, wxWindowID id);

	void SetBalance(CurrencyValue balance, std::vector<CurrencyValue> own, std::vector<CurrencyValue> credit);
	void Update();

private:
	std::vector<CurrencyValue> ownFunds;
	std::vector<CurrencyValue> creditFunds;
	CurrencyValue totalBalance;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
