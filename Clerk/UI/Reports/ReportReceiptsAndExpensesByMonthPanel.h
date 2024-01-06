#pragma once

#include <wx/wx.h>
#include <algorithm>
#include <map>
#include "../../Data/PresentationModels/Types.h"
#include "../../Utils/Settings/Settings.h"
#include "../Charts/BarChart.h"
#include "../DataPanel/DataPanel.h"
#include "../Controls/PeriodFilterPanel.h"
#include "Enums.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class ReportReceiptsAndExpensesByMonthPanel : public DataPanel
{
public:
	ReportReceiptsAndExpensesByMonthPanel(wxWindow* parent, DataContext& context, Icons& icons);
	~ReportReceiptsAndExpensesByMonthPanel();

	void Update();

private:
	BarChart* _chart;
	PeriodFilterPanel* _periodFilterPanel;
	std::vector<DateValueViewModel> _values = {};

	void RestoreFilterSettings();
	void SaveFilterSettings();
};

