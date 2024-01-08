#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/numformatter.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Utils;
using namespace Clerk::Data;

class ReportChartTooltipPanel : public wxPanel
{
public:
	ReportChartTooltipPanel(wxWindow *parent);

	void SetTitle(wxString title);
	void SetTotal(float total);
	void SetValues(std::vector<StringValueViewModel> values);
	void Update();

private:
	wxString title;
	float total;
	std::vector<StringValueViewModel> values;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};