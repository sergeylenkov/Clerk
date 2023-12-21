#pragma once

#include <wx/wx.h>
#include <map>
#include "../PresentationModels/SchedulerPresentationModel.h"
#include "../ViewModels/DashboardViewModel.h"

using namespace Clerk::Data;

class DashboardSchedulersPanel : public wxPanel
{
public:
	DashboardSchedulersPanel(wxWindow *parent);	

	void SetViewModel(DashboardViewModel* viewModel);	

private:
	DashboardViewModel* _viewModel{};
	std::vector<std::shared_ptr<SchedulerPresentationModel>> _schedulers;
	std::map<wxString, float> _values;
	float _maxValue = 0;

	void Update();
	void Draw(wxPaintDC &dc);
	void DrawCalendar(wxPaintDC &dc);
	void DrawTable(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};


