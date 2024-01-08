#pragma once

#include <wx/wx.h>
#include <wx/popupwin.h>
#include "../../Data/PresentationModels/Types.h"
#include "ReportChartTooltipPanel.h"

class ReportChartTooltipPopup : public wxPopupWindow
{
public:
	ReportChartTooltipPopup(wxWindow* parent);

	void Update(wxString title, float total, std::vector<StringValueViewModel> values);

private:
	ReportChartTooltipPanel* _panel;
};

