#pragma once

#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <wx/numformatter.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "../../Data/DataHelper.h"

using namespace std;

class GraphPopup : public wxPopupWindow
{
public:
	GraphPopup(wxWindow *parent);
	~GraphPopup();

	void Update(vector<StringValue> values);

private:
	wxScrolledWindow *panel;
	wxListCtrl *list;
};
