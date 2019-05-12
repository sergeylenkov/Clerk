#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/DataHelper.h"

class DashboardAccountsPanel : public wxPanel
{
public:
	DashboardAccountsPanel(wxWindow *parent, wxWindowID id);	

	void SetAccounts(std::vector<StringValue> accounts);
	void Update();

private:
	std::vector<StringValue> accounts;	

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};