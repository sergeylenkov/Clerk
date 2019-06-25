#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

class DashboardDebtsPanel : public wxPanel
{
public:
	DashboardDebtsPanel(wxWindow *parent, wxWindowID id);

	void SetDebts(std::vector<std::shared_ptr<Account>> debts);
	void Update();

private:
	std::vector<std::shared_ptr<Account>> debts;
	std::vector<DahboardProgressValue> values;
	float totalValue;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};