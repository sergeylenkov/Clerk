#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../../Data/PresentationModels/Types.h"
#include "DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

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