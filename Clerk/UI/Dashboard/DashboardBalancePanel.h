#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data//DataHelper.h"

class DashboardBalancePanel : public wxPanel
{
public:
	DashboardBalancePanel(wxWindow *parent, wxWindowID id);
	~DashboardBalancePanel();

	void SetBalance(std::map<wxString, float> ownFunds, std::map<wxString, float> creditFunds);
	void Update();

private:
	std::map<wxString, float> ownFunds;
	std::map<wxString, float> creditFunds;

	void Draw();
	void OnPaint(wxPaintEvent& event);
};
