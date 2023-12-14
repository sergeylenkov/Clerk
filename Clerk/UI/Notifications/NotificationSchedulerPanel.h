#pragma once

#include "wx/wx.h"
#include "../PresentationModels/SchedulerPresentationModel.h"
#include "../../Utils/Types.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class NotificationSchedulerPanel : public wxPanel
{
public:
	NotificationSchedulerPanel(wxWindow* parent, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize);

	void SetScheduler(std::shared_ptr<SchedulerPresentationModel> scheduler);
	std::function<void(std::shared_ptr<SchedulerPresentationModel>)> OnDismiss;
	std::function<void(std::shared_ptr<SchedulerPresentationModel>)> OnSkip;
	std::function<void(std::shared_ptr<SchedulerPresentationModel>)> OnExec;

private:
	std::shared_ptr<SchedulerPresentationModel> _scheduler;

	wxStaticText* _label;

	void Update();
	void OnDismissClick(wxCommandEvent& event);
	void OnSkipClick(wxCommandEvent& event);
	void OnExecClick(wxCommandEvent& event);
};

