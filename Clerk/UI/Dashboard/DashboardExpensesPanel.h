#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/AccountViewModel.h"
#include "../../Data/ViewModels/CurrencyValueViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardExpensesPanel : public wxPanel
{
public:
	DashboardExpensesPanel(wxWindow *parent);

	void SetExpenses(std::vector<std::shared_ptr<AccountViewModel>> expenses);
	void SetTotal(CurrencyValueViewModel value);
	void Update();

private:
	std::vector<std::shared_ptr<AccountViewModel>> _expenses;
	CurrencyValueViewModel _total;
	float maxValue;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
