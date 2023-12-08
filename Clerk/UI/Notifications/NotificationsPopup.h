#pragma once
#include <wx/wx.h>
#include <wx/popupwin.h>
#include <vector>
#include <memory>
#include "../../Utils/Types.h"
#include "../PresentationModels/AlertPresentationModel.h"
#include "NotificationAlertPanel.h"

using namespace Clerk::UI;

class NotificationsPopup : public wxPopupTransientWindow
{
public:
	NotificationsPopup(wxWindow* parent);

	void Position(wxPoint position, wxSize size);
	void Update(shared_vector<AlertPresentationModel> alerts);
	std::function<void(std::shared_ptr<AlertPresentationModel>)> OnDismiss;

private:
	wxScrolledWindow* _panel;	
	wxBoxSizer* _notificationsSizer;
};