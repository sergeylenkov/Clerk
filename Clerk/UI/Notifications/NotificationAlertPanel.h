#pragma once

#include "wx/wx.h"
#include "../../Data/PresentationModels/AlertPresentationModel.h"
#include "../../Utils/Types.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class NotificationAlertPanel : public wxPanel
{
public:
	NotificationAlertPanel(wxWindow* parent, const wxPoint& position, const wxSize& size);

	void SetAlert(std::shared_ptr<AlertPresentationModel> alert);
	std::function<void(std::shared_ptr<AlertPresentationModel>)> OnDismiss;

private:
	std::shared_ptr<AlertPresentationModel> _alert;

	wxStaticText* _label;

	void Update();
	void OnPaint(wxPaintEvent& event);
	void OnDismissClick(wxCommandEvent& event);
};
