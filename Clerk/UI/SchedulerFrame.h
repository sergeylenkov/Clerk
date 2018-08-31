#pragma once

#include <wx/wx.h>
#include <wx/bmpcbox.h>
#include <wx/valnum.h>
#include "../Data/DataHelper.h"

class SchedulerFrame : public wxFrame
{
public:
	SchedulerFrame(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~SchedulerFrame();

	void SetScheduler(std::shared_ptr<Scheduler> scheduler);

	std::function<void()> OnClose;

private:	
	wxButton *okButton;
	wxButton *cancelButton;
	std::shared_ptr<Scheduler> scheduler;

	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);
};
