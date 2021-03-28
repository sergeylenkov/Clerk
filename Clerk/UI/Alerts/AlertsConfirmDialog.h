#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../../Data/ViewModels/AlertViewModel.h"
#include "../../Utils/Utils.h"

using namespace Clerk::Data;
using namespace Clerk::Utils;

class AlertsConfirmDialog : public wxFrame
{
public:
	AlertsConfirmDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~AlertsConfirmDialog();

	void SetAlerts(std::vector<std::shared_ptr<AlertViewModel>> alerts);

	std::function<void()> OnClose;

private:
	wxListCtrl *list;
	wxButton *okButton;

	std::vector<std::shared_ptr<AlertViewModel>> alerts;

	void UpdateList();
	void OnOk(wxCommandEvent &event);
	void OnCloseWindow(wxCloseEvent &event);
};

