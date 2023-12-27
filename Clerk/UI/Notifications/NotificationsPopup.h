#pragma once
#include <wx/wx.h>
#include <wx/popupwin.h>
#include <vector>
#include <memory>
#include "../../Utils/Types.h"
#include "NotificationAlertPanel.h"
#include "NotificationSchedulerPanel.h"

class NotificationsPopup : public wxPopupTransientWindow
{
public:
	NotificationsPopup(wxWindow* parent);

	void Position(wxPoint position, wxSize size);
	void Update(shared_vector<AlertPresentationModel> alerts, shared_vector<SchedulerPresentationModel> schedulers);
	std::function<void(std::shared_ptr<AlertPresentationModel>)> OnDismissAlert;
	std::function<void(std::shared_ptr<SchedulerPresentationModel>)> OnDismissScheduler;
	std::function<void(std::shared_ptr<SchedulerPresentationModel>)> OnExecScheduler;
	std::function<void(std::shared_ptr<SchedulerPresentationModel>)> OnSkipScheduler;

private:
	wxScrolledWindow* _panel;	
	wxBoxSizer* _notificationsSizer;
};