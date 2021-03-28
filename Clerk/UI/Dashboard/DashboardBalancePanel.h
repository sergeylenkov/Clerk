#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/CurrencyValueViewModel.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;

class DashboardBalancePanel : public wxPanel
{
public:
	DashboardBalancePanel(wxWindow *parent);

	void SetBalance(CurrencyValueViewModel total, CurrencyValueViewModel own, CurrencyValueViewModel credit);
	void Update();

private:
	CurrencyValueViewModel _own;
	CurrencyValueViewModel _credit;
	CurrencyValueViewModel _total;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
