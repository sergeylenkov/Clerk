#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "../../Data/PresentationModels/AccountPresentationModel.h"
#include "DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class DashboardAccountsPanel : public wxPanel
{
public:
	DashboardAccountsPanel(wxWindow *parent);	

	void SetViewModel(DashboardViewModel* viewModel);

private:
	DashboardViewModel* _viewModel{};
	shared_vector<AccountPresentationModel> _accounts;
	float _total = 0.0;

	void Update();

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};