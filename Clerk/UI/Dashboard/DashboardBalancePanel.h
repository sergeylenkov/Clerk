#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data//DataHelper.h"

class DashboardBalancePanel : public wxPanel
{
public:
	DashboardBalancePanel(wxWindow *parent, wxWindowID id);

	void SetBalance(float balance, std::map<wxString, float> ownFunds, std::map<wxString, float> creditFunds);
	void Update();

private:
	std::map<wxString, float> ownFunds;
	std::map<wxString, float> creditFunds;
	float totalBalance;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
