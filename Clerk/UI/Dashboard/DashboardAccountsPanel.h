#pragma once

#include <wx/wx.h>
#include "../../Utils/Utils.h"
#include "../../Utils/Types.h"
#include "../../Data/PresentationModels/AccountPresentationModel.h"
#include "../../Data/Services/CurrenciesService.h"
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
	float _total;
	int _width;
	int _height;

	void Update();

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
	void DrawValue(wxPaintDC& dc, wxString value, int y);
};