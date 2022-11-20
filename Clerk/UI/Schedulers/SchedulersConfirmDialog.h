#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../PresentationModels/SchedulerPresentationModel.h"

using namespace Clerk::Data;
using namespace Clerk::UI;

class SchedulersConfirmDialog : public wxFrame
{
public:
	SchedulersConfirmDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~SchedulersConfirmDialog();

	void SetSchedulers(std::vector<std::shared_ptr<SchedulerPresentationModel>> schedulers);

	std::function<void()> OnClose;

private:
	wxListCtrl *list;
	wxButton *skipAllButton;
	wxButton *skipButton;
	wxButton *applyAllButton;

	std::vector<std::shared_ptr<SchedulerPresentationModel>> _schedulers;

	void UpdateList();
	int SelectedItemIndex();
	void OnSkipAll(wxCommandEvent &event);
	void OnSkip(wxCommandEvent &event);
	void OnApplyAll(wxCommandEvent &event);
	void OnCloseWindow(wxCloseEvent &event);
};

