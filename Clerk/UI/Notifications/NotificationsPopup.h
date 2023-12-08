#pragma once
#include <wx/wx.h>
#include <wx/popupwin.h>
#include <vector>
#include <memory>
#include "../../Utils/Types.h"

class NotificationsPopup : public wxPopupTransientWindow
{
public:
	NotificationsPopup(wxWindow* parent);

	void Position(wxPoint position, wxSize size);
	void Update();

private:
	wxScrolledWindow* _panel;
};