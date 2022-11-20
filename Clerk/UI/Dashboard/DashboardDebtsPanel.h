#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../PresentationModels/AccountPresentationModel.h"
#include "../ViewModels/DashboardViewModel.h"
#include "../PresentationModels/Types.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardDebtsPanel : public wxPanel
{
public:
	DashboardDebtsPanel(wxWindow *parent);

	void SetViewModel(DashboardViewModel* viewModel);	

private:	
	DashboardViewModel* _viewModel{};
	std::vector<DashboardProgressValue> _values;
	float _totalValue = 0.0;

	void Update();
	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};