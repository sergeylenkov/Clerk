#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

class DashboardAccountsPanel : public wxPanel
{
public:
	DashboardAccountsPanel(wxWindow *parent, wxWindowID id);	

	void SetAccounts(std::vector<AccountValue> accounts);
	void Update();

private:
	std::vector<AccountValue> accounts;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};