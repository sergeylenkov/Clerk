#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../PresentationModels/Types.h"
#include "../ViewModels/DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardExpensesPanel : public wxPanel
{
public:
	DashboardExpensesPanel(wxWindow *parent);

	void SetViewModel(DashboardViewModel* viewModel);

private:
	DashboardViewModel* _viewModel{};
	float _maxValue = 0;
	std::vector<std::shared_ptr<AccountPresentationModel>> _accounts;
	float _total = 0.0;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};
