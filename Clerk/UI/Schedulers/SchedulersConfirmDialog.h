#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../../Data/ViewModels/SchedulerViewModel.h"

using namespace Clerk::Data;

class SchedulersConfirmDialog : public wxFrame
{
public:
	SchedulersConfirmDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~SchedulersConfirmDialog();

	void SetSchedulers(std::vector<std::shared_ptr<SchedulerViewModel>> schedulers);

	std::function<void()> OnClose;

private:
	wxListCtrl *list;
	wxButton *skipAllButton;
	wxButton *skipButton;
	wxButton *applyAllButton;

	std::vector<std::shared_ptr<SchedulerViewModel>> _schedulers;

	void UpdateList();
	int SelectedItemIndex();
	void OnSkipAll(wxCommandEvent &event);
	void OnSkip(wxCommandEvent &event);
	void OnApplyAll(wxCommandEvent &event);
	void OnCloseWindow(wxCloseEvent &event);
};

