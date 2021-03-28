#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data/ViewModels/AccountViewModel.h"
#include "../../Data/ViewModels/Types.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardDebtsPanel : public wxPanel
{
public:
	DashboardDebtsPanel(wxWindow *parent);

	void SetDebts(std::vector<std::shared_ptr<AccountViewModel>> debts);
	void Update();

private:
	std::vector<std::shared_ptr<AccountViewModel>> _debts;
	std::vector<DashboardProgressValue> _values;
	float _totalValue = 0.0;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};