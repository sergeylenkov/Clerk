#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Data/DataHelper.h"
#include "DataPanel.h"

enum class SchedulersPanelMenuTypes {
	Edit = 1,
	Delete = 2,
};

class SchedulersPanel : public DataPanel
{
public:
	SchedulersPanel(wxWindow *parent, wxWindowID id);
	
	std::shared_ptr<Scheduler> GetScheduler();
	void Update();
	void EditScheduler();
	void DeleteScheduler();

	std::function<void(std::shared_ptr<Scheduler> scheduler)> OnEditScheduler;

private:
	wxListCtrl *list;
	std::vector<std::shared_ptr<Scheduler>> schedulers;

	void OnListItemClick(wxListEvent &event);
	void OnListItemDoubleClick(wxListEvent &event);
	void OnMenuSelect(wxCommandEvent &event);
};