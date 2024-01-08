#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <vector>
#include <algorithm>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class BarChart : public wxPanel {
public:
	BarChart(wxWindow *parent);
	~BarChart();

	void SetValues(std::vector<StringValueViewModel> values);
	void Draw();

private:
	std::vector<StringValueViewModel> _values;

	void OnPaint(wxPaintEvent& event);
};