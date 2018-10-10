#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../Data/Scheduler.h"

class SchedulersConfirmFrame : public wxFrame
{
public:
	SchedulersConfirmFrame(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~SchedulersConfirmFrame();

	void SetSchedulers(std::vector<std::shared_ptr<Scheduler>> schedulers);

	std::function<void()> OnClose;

private:
	wxListCtrl *list;
	wxButton *skipAllButton;
	wxButton *skipButton;
	wxButton *applyAllButton;

	std::vector<std::shared_ptr<Scheduler>> schedulers;

	void UpdateList();
	int SelectedItemIndex();
	void OnSkipAll(wxCommandEvent &event);
	void OnSkip(wxCommandEvent &event);
	void OnApplyAll(wxCommandEvent &event);
};

