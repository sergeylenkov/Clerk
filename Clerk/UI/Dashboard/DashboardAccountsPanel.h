#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data//ViewModels/AccountViewModel.h"
#include "../../Data/ViewModels/DashboardViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardAccountsPanel : public wxPanel
{
public:
	DashboardAccountsPanel(wxWindow *parent);	

	void SetViewModel(DashboardViewModel* viewModel);

private:
	DashboardViewModel* _viewModel{};
	std::vector<std::shared_ptr<AccountViewModel>> _accounts;

	void Update();

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};