#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../PresentationModels/AlertPresentationModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;
using namespace Clerk::UI;

class AlertsConfirmDialog : public wxFrame
{
public:
	AlertsConfirmDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~AlertsConfirmDialog();

	void SetAlerts(std::vector<std::shared_ptr<AlertPresentationModel>> alerts);

	std::function<void()> OnClose;

private:
	wxListCtrl *list;
	wxButton *okButton;

	std::vector<std::shared_ptr<AlertPresentationModel>> alerts;

	void UpdateList();
	void OnOk(wxCommandEvent &event);
	void OnCloseWindow(wxCloseEvent &event);
};

