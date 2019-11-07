#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../../Data/Alert.h"

class AlertsConfirmDialog : public wxFrame
{
public:
	AlertsConfirmDialog(wxFrame *parent, const wxChar *title, int xpos, int ypos, int width, int height);
	~AlertsConfirmDialog();

	void SetAlerts(std::vector<std::shared_ptr<Alert>> alerts);

	std::function<void()> OnClose;

private:
	wxListCtrl *list;
	wxButton *okButton;

	std::vector<std::shared_ptr<Alert>> alerts;

	void UpdateList();
	void OnOk(wxCommandEvent &event);
	void OnCloseWindow(wxCloseEvent &event);
};

