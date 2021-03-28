#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <map>
#include "../../Data/ViewModels/SchedulerViewModel.h"

using namespace Clerk::Data;

class DashboardSchedulersPanel : public wxPanel
{
public:
	DashboardSchedulersPanel(wxWindow *parent);	

	void SetSchedulers(std::vector<std::shared_ptr<SchedulerViewModel>> schedulers);
	void Update();

private:
	std::vector<std::shared_ptr<SchedulerViewModel>> _schedulers;
	std::map<wxString, float> _values;
	float _maxValue = 0;

	void Draw(wxPaintDC &dc);
	void DrawCalendar(wxPaintDC &dc);
	void DrawTable(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};


