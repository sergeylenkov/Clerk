#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include "../../Data/DataHelper.h"

class DashboardCreditsPanel : public wxPanel
{
public:
	DashboardCreditsPanel(wxWindow *parent, wxWindowID id);

	void SetCredits(std::vector<std::shared_ptr<Account>> credits);
	void Update();

private:
	std::vector<std::shared_ptr<Account>> credits;
	std::vector<DahboardProgressValue> values;

	void Draw();
	void OnPaint(wxPaintEvent& event);
};