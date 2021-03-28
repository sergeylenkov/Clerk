#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Utils/Utils.h"
#include "../../Data//ViewModels/AccountViewModel.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class DashboardAccountsPanel : public wxPanel
{
public:
	DashboardAccountsPanel(wxWindow *parent);	

	void SetAccounts(std::vector<std::shared_ptr<AccountViewModel>> accounts);
	void Update();

private:
	std::vector<std::shared_ptr<AccountViewModel>> _accounts;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};