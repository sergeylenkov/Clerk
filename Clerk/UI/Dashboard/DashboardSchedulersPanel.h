#pragma once

#include <wx/wx.h>
#include <wx/numformatter.h>
#include <map>
#include "../../Data/Scheduler.h"

class DashboardSchedulersPanel : public wxPanel
{
public:
	DashboardSchedulersPanel(wxWindow *parent, wxWindowID id);	

	void SetSchedulers(std::vector<std::shared_ptr<Scheduler>> schedulers);
	void Update();

private:
	std::vector<std::shared_ptr<Scheduler>> schedulers;
	std::map<wxString, float> values;
	float maxValue;

	void Draw(wxPaintDC &dc);
	void DrawCalendar(wxPaintDC &dc);
	void DrawTable(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};


