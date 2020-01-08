#pragma once

#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listctrl.h>
#include <wx/numformatter.h>
#include <vector>
#include <memory>
#include <algorithm>
#include "../../Data/DataHelper.h"
#include "../../Utils/Utils.h"

using namespace std;

class ExpensesTooltipPanel : public wxPanel
{
public:
	ExpensesTooltipPanel(wxWindow *parent);

	void SetTitle(wxString title);
	void SetTotal(float total);
	void SetValues(std::vector<StringValue> values);
	void Update();

private:
	wxString title;
	float total;
	std::vector<StringValue> values;

	void Draw(wxPaintDC &dc);
	void OnPaint(wxPaintEvent& event);
};

class ExpensesTooltipPopup : public wxPopupWindow
{
public:
	ExpensesTooltipPopup(wxWindow *parent);
	~ExpensesTooltipPopup();

	void Update(wxString title, std::vector<StringValue> values);

private:	
	ExpensesTooltipPanel *panel;
};
